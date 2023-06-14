#pragma once
#include <Nlohmann/json.hpp>
#include <math.h>
#include <FMT/format.h>

#include <llapi/mc/Types.hpp>
#include <llapi/Global.h>
#include <llapi/LoggerAPI.h>
#include <llapi/mc/Level.hpp>
#include <llapi/mc/ChunkSource.hpp>
#include <llapi/mc/LevelChunk.hpp>
#include <llapi/mc/Biome.hpp>

#include <llapi/mc/Dimension.hpp>
#include <llapi/mc/ChunkPos.hpp>
#include <llapi/mc/BlockPos.hpp>
#include <llapi/mc/ChunkBlockPos.hpp>
#include <llapi/ScheduleAPI.h>
#include <direct.h>

#include "Tools.h"
#include "ScheduleManager.h"

extern Logger logger;

// MoveChunk


class ChunkManager
{
public:
    // ����Ⱥϵ, �����漰�ܶණ��, ��almulet����
	static bool startSaveAreaTask(int s_x, int s_z, int e_x, int e_z, string floder_name, int dim_id, bool block, bool entity, bool biome, bool save) {
        if (!SS::task.isFinished()) {
            return false;
        }
        FileTool::makeDir(fmt::format("plugins/WheatBuilder/Area/{}", floder_name));
		int start_x = s_x;
		int start_z = s_z;
		int end_x = e_x;
		int end_z = e_z;
		PositionTool::arrangeCoordinate_2D(start_x, start_z, end_x, end_z);

		SS::area_process_list = PositionTool::divideArea(start_x, start_z, end_x, end_z, dim_id);
        SS::total = SS::area_process_list.size();
        SS::floder_name = floder_name;
        SS::dim_id = dim_id;
        SS::block = block;
        SS::entity = entity;
        SS::biome = biome;
        SS::save = save;
        SS::status = 0; // ״̬, 0Ϊ��ʼ����, 1Ϊ�ȴ�����
        SS::onProcess_locate_x = start_x;
        SS::onProcess_locate_z = start_z;
        SS::onProcess_i = 0;

        SS::empty_try = 0;
        SS::empty_try_max = 3;
        // logger.info(std::to_string(area_process_list.size()));
        SS::task = Schedule::repeat([]() {
                // ��ȡ��ǰ��������
                if (SS::status == 0) {
                    if (SS::onProcess_i >= SS::total) {
                        logger.info("task finished.");
                        // �޷����ڲ��ر�, �鿴issue����Ҫ��Ϊȫ�ֱ���
                        SS::task.cancel();
                        return;
                    }
                    // �����ѱ��������
                    if (_access(fmt::format("plugins/WheatBuilder/Area/{}/{}.json", SS::floder_name, SS::onProcess_i).c_str(), 0) == 0) {
                        logger.info(fmt::format("Skip {}", SS::onProcess_i));
                        SS::onProcess_i++;
                        return;
                    }
                    else {
                        logger.info(fmt::format("{}/{}", SS::onProcess_i, SS::total));
                        SS::onProcess_start_x = SS::area_process_list.get(SS::onProcess_i).start_x;
                        SS::onProcess_start_z = SS::area_process_list.get(SS::onProcess_i).start_z;
                        SS::onProcess_end_x = SS::area_process_list.get(SS::onProcess_i).end_x;
                        SS::onProcess_end_z = SS::area_process_list.get(SS::onProcess_i).end_z;
                        string cmd = fmt::format("/tickingarea add {} -64 {} {} 320 {} movechunk{}"
                            , SS::onProcess_start_x * 16, SS::onProcess_start_z * 16, SS::onProcess_end_x * 16 + 8, SS::onProcess_end_z * 16 + 15, SS::onProcess_i);
                        logger.info(cmd);
                        Level::runcmd(cmd);
                        SS::status = 1;
                        SS::empty_try = 0;
                    }
                }
                else if (SS::status == 1) {
                    bool ticked = true;
                    try {
                        Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
                        if (dimension != nullptr) {
                            ChunkSource& chunkSource = dimension->getChunkSource();
                            for (int tick_x = SS::onProcess_start_x; tick_x <= SS::onProcess_end_x; tick_x++) {
                                for (int tick_z = SS::onProcess_start_z; tick_z <= SS::onProcess_end_z; tick_z++) {
                                    if (chunkSource.getAvailableChunk(ChunkPos(tick_x, tick_z)) == nullptr) {
                                        ticked = false;
                                        break;
                                    }
                                }
                                if (ticked == false) break;
                            }
                        }
                        else {
                            ticked = false;
                        }
                    }
                    catch (std::exception& e)
                    {
                        logger.error(fmt::format("Failed to check available chunk: {}", e.what()));
                        ticked = false;
                    }
                    SS::empty_try++;

                    // ����
                    if (ticked) {// �ж���ʽ�ı䣬��ȡ�� || SS::empty_try > SS::empty_try_max
                        nlohmann::json area = getArea(SS::onProcess_start_x, SS::onProcess_start_z, SS::onProcess_end_x
                            , SS::onProcess_end_z, SS::dim_id, SS::onProcess_locate_x, SS::onProcess_locate_z, SS::block, SS::entity, SS::biome, SS::save);
                        FileTool::saveJson(area, fmt::format("plugins/WheatBuilder/Area/{}/{}.json", SS::floder_name, SS::onProcess_i));
                        string cmd = fmt::format("tickingarea remove movechunk{}", SS::onProcess_i);
                        logger.info(cmd);
                        Level::runcmd(cmd);
                        SS::status = 0;
                        SS::onProcess_i++;
                    }
                }
            }, 20);// tick
		return true;
	}
	static bool startLoadAreaTask(int start_x, int start_z, int dim_id, string floder_name, bool block, bool entity, bool biome) {
        if (!LS::task.isFinished()) {
            return false;
        }
        if(_access(fmt::format("plugins/WheatBuilder/Area/{}/0.json", floder_name).c_str(), 0) == 0){
            // ����
            LS::status = 0; // ״̬, 0Ϊ��ʼ����, 1Ϊ�ȴ�����
            LS::onProcess_i = 0;
            LS::empty_try = 0;
            LS::empty_try_max = 10;
            LS::areaUnit = {};
            LS::emptyTryAmount = 0;

            LS::floder_name = floder_name;
            LS::dim_id = dim_id;
            LS::block = block;
            LS::entity = entity;
            LS::biome = biome;
            LS::start_x = start_x;
            LS::start_z = start_z;

            LS::task = Schedule::repeat([](){
                // ��ȡ��ǰ��������
                if(LS::status == 0){
                    // ������һ������Ԫ
                    if(_access(fmt::format("plugins/WheatBuilder/Area/{}/{}.json", LS::floder_name, LS::onProcess_i).c_str(), 0) == 0){
                        // ��������
                        LS::areaUnit = FileTool::loadJson(fmt::format("plugins/WheatBuilder/Area/{}/{}.json", LS::floder_name, LS::onProcess_i));
                        LS::onProcess_start_x = LS::start_x + LS::areaUnit[0]["position"][0];
                        LS::onProcess_start_z = LS::start_z + LS::areaUnit[0]["position"][1];
                        LS::onProcess_end_x   = LS::start_x + LS::areaUnit[LS::areaUnit.size() - 1]["position"][0];
                        LS::onProcess_end_z   = LS::start_z + LS::areaUnit[LS::areaUnit.size() - 1]["position"][1];
                        string cmd = fmt::format("tickingarea add {} 0 {} {} 0 {} movechunk{}",
                            LS::onProcess_start_x * 16 + 8, LS::onProcess_start_z * 16 + 8, LS::onProcess_end_x * 16 + 8, LS::onProcess_end_z * 16 + 8, LS::onProcess_i);
                        logger.info(cmd);
                        Level::runcmd(cmd);
                        LS::status = 1;
                    }
                    // û�и����ļ���ȡ, �������
                    else{
                        logger.info("task finished.");
                        LS::task.cancel();
                        return;
                    }
                }
                // �ȴ��������
                else if(LS::status == 1){
                    bool ticked = true;
                    try {
                        Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
                        if (dimension != nullptr) {
                            ChunkSource& chunkSource = dimension->getChunkSource();
                            for (int tick_x = LS::onProcess_start_x; tick_x <= LS::onProcess_end_x; tick_x++) {
                                for (int tick_z = LS::onProcess_start_z; tick_z <= LS::onProcess_end_z; tick_z++) {
                                    if (chunkSource.getAvailableChunk(ChunkPos(tick_x, tick_z)) == nullptr) {
                                        ticked = false;
                                        break;
                                    }
                                }
                                if (ticked == false) break;
                            }
                        }
                        else {
                            ticked = false;
                        }
                    }
                    catch (std::exception& e)
                    {
                        logger.error(fmt::format("Failed to check available chunk: {}", e.what()));
                        ticked = false;
                    }
                    LS::empty_try++;

                    // ����
                    if (ticked) {//  || LS::empty_try > LS::empty_try_max
                        LS::empty_try = 0;
                        setArea(LS::start_x, LS::start_z, LS::dim_id, FileTool::loadJson(fmt::format("plugins/WheatBuilder/Area/{}/{}.json",LS::floder_name, LS::onProcess_i)), LS::block, LS::entity, LS::biome);

                        string cmd = fmt::format("tickingarea remove movechunk{}", LS::onProcess_i);
                        logger.info(cmd);
                        Level::runcmd(cmd);
                        
                        LS::status = 0;
                        LS::onProcess_i++;
                    }
                }
            }, 20);
        }
        else{
            logger.info("Area floder not exist.");
        }
    }
    static bool startGenerateAreaTask(int s_x, int s_z, int e_x, int e_z, int dim_id) {
        if (!GS::task.isFinished()) {
            return false;
        }
        int start_x = s_x;
        int start_z = s_z;
        int end_x = e_x;
        int end_z = e_z;
        PositionTool::arrangeCoordinate_2D(start_x, start_z, end_x, end_z);

        GS::area_process_list = PositionTool::divideArea(start_x, start_z, end_x, end_z, dim_id);
        GS::total = GS::area_process_list.size();
        GS::dim_id = dim_id;
        GS::status = 0; // ״̬, 0Ϊ��ʼ����, 1Ϊ�ȴ�����
        GS::onProcess_locate_x = start_x;
        GS::onProcess_locate_z = start_z;
        GS::onProcess_i = 0;

        GS::empty_try = 0;
        GS::empty_try_max = 3;
        // logger.info(std::to_string(area_process_list.size()));
        GS::task = Schedule::repeat([]() {
            // ��ȡ��ǰ��������
            if (GS::status == 0) {
                if (GS::onProcess_i >= GS::total) {
                    logger.info("task finished.");
                    // �޷����ڲ��ر�, �鿴issue����Ҫ��Ϊȫ�ֱ���
                   GS::task.cancel();
                    return;
                }
                else {
                    logger.info(fmt::format("{}/{}", GS::onProcess_i , GS::total));
                    GS::onProcess_start_x =GS::area_process_list.get(GS::onProcess_i).start_x;
                    GS::onProcess_start_z =GS::area_process_list.get(GS::onProcess_i).start_z;
                    GS::onProcess_end_x =GS::area_process_list.get(GS::onProcess_i).end_x;
                    GS::onProcess_end_z =GS::area_process_list.get(GS::onProcess_i).end_z;
                    string cmd = fmt::format("/tickingarea add {} -64 {} {} 320 {} generatechunk{}"
                        ,GS::onProcess_start_x * 16,GS::onProcess_start_z * 16,GS::onProcess_end_x * 16 + 8,GS::onProcess_end_z * 16 + 15,GS::onProcess_i);
                    logger.info(cmd);
                    Level::runcmd(cmd);
                    GS::status = 1;
                    GS::empty_try = 0;
                }
            }
            else if (GS::status == 1) {
                bool ticked = true;
                try {
                    Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
                    if (dimension != nullptr) {
                        ChunkSource& chunkSource = dimension->getChunkSource();
                        for (int tick_x =GS::onProcess_start_x; tick_x <=GS::onProcess_end_x; tick_x++) {
                            for (int tick_z =GS::onProcess_start_z; tick_z <=GS::onProcess_end_z; tick_z++) {
                                if (chunkSource.getAvailableChunk(ChunkPos(tick_x, tick_z)) == nullptr) {
                                    ticked = false;
                                    break;
                                }
                            }
                            if (ticked == false) break;
                        }
                    }
                    else {
                        ticked = false;
                    }
                }
                catch (std::exception& e)
                {
                    logger.error(fmt::format("Failed to check available chunk: {}", e.what()));
                    ticked = false;
                }
                GS::empty_try++;

                // ����
                if (ticked) {// �ж���ʽ�ı䣬��ȡ�� ||GS::empty_try >GS::empty_try_max
                    Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
                    ChunkSource& chunkSource = dimension->getChunkSource();
                    for (int x = GS::onProcess_start_x; x <= GS::onProcess_end_x; x++) {
                        for (int z = GS::onProcess_start_z; z <= GS::onProcess_end_z; z++) {
                            chunkSource.getAvailableChunk(ChunkPos(x, z))->setUnsaved();
                        }
                    }
                    string cmd = fmt::format("tickingarea remove generatechunk{}",GS::onProcess_i);
                    logger.info(cmd);
                    Level::runcmd(cmd);
                    GS::status = 0;
                    GS::onProcess_i++;
                }
            }
            }, 5);// tick
        return true;
    }

    // 100 chunks at most
    static nlohmann::json getArea(int s_x, int s_z, int e_x, int e_z, int dim_id, int locate_x, int locate_z, bool block, bool entity, bool biome, bool save) {
        int start_x = s_x;
        int start_z = s_z;
        int end_x = e_x;
        int end_z = e_z;
        PositionTool::arrangeCoordinate_2D(start_x, start_z, end_x, end_z);

        // ��ȡ
        nlohmann::json area = nlohmann::json::array();
        Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
        ChunkSource& chunkSource = dimension->getChunkSource();
        for (int x = start_x; x <= end_x; x++) {
            for (int z = start_z; z <= end_z; z++) {
                area.push_back(getChunk(x - locate_x, z - locate_z, chunkSource.getAvailableChunk(ChunkPos(x,z)), block, entity, biome, save));
            }
        }
        return area;
    }
	static nlohmann::json getChunk(int position_x, int position_z, std::shared_ptr<class LevelChunk> levelChunk, bool block, bool entity, bool biome, bool save) {
        const ChunkPos& pos = levelChunk->getPosition();
        // logger.info(fmt::format("get chunk {}, {}", pos.x, pos.z));
        // Create chunk json
        nlohmann::json chunk = {
            {"position" , {position_x, position_z}},
            {"structure", {
                {"size"   , {16, 384, 16}} ,
                // "blocks" , {},
                // "entities" , {},
                // {"biomes", biomes},
                // {"height", heightMap}
            }}
        };
        // Get biomes
        if (biome) {
            nlohmann::json biomes;
            int y = -64;
            int blockI = -1;
            for (int iy = 0; iy < 384; iy++) {
                int x = 0;
                for (int ix = 0; ix < 16; ix++) {
                    int z = 0;
                    for (int iz = 0; iz < 16; iz++) {
                        string biomeName = levelChunk->getBiome(ChunkBlockPos(x, y, z)).getName();
                        // �洢
                        if (blockI != -1 && biomeName == biomes[blockI]["name"]) {
                            biomes[blockI]["amount"] = biomes[blockI]["amount"] + 1;
                        }
                        else {
                            blockI++;
                            biomes.push_back({
                                {"name", biomeName} ,
                                {"amount", 1 }
                                });
                        }
                        z++;
                    }
                    x++;
                }
                y++;
            }
            chunk["structure"]["biomes"] = biomes;
        }
        
        // Get height map
        if (false) {
            nlohmann::json heightMap;
            int i = 0;
            for (int ix = 0; ix < 16; ix++) {
                for (int iz = 0; iz < 16; iz++) {
                    // heightMap.push_back(i);
                    i++;
                    heightMap.push_back(levelChunk->getHeightmap(ChunkBlockPos(ix, 0, iz)));
                }
            }
            chunk["structure"]["height"] = heightMap;
        }

        if(save) levelChunk->setUnsaved();
        return chunk;
	}
    // 100 chunks at most
    static bool setArea(int start_x, int start_z, int dim_id, nlohmann::json area, bool block, bool entity, bool biome) {
        Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
        ChunkSource& chunkSource = dimension->getChunkSource();
        for (int i = 0; i < area.size(); i++) {
            setChunk(chunkSource.getAvailableChunk(ChunkPos(start_x + area[i]["position"][0], start_z + area[i]["position"][1])), area[i]["structure"], block, entity, biome);
        }
        return true;
    }
    static bool setChunk(std::shared_ptr<class LevelChunk> levelChunk, nlohmann::json structure, bool block, bool entity, bool biome) {
        const ChunkPos& pos = levelChunk->getPosition();
        // logger.info(fmt::format("set chunk {}, {}", pos.x, pos.z));
        
        // set biomes
        if (biome) {
            nlohmann::json biomes = structure["biomes"];
            int y = -64;
            int blockI = 0;
            int finish = 0;
            for (int iy = 0; iy < 384; iy++) {
                int x = 0;
                for (int ix = 0; ix < 16; ix++) {
                    int z = 0;
                    for (int iz = 0; iz < 16; iz++) {
                        levelChunk->setBiome2d(*Biome::fromName(biomes[blockI]["name"]), ChunkBlockPos(x, y, z));
                        finish++;
                        // ��һ��
                        if (finish >= biomes[blockI]["amount"]) {
                            blockI++;
                            finish = 0;
                        }
                        z++;
                    }
                    x++;
                }
                y++;
            }
        }
        
        // set height
        if (false) {
            nlohmann::json heightJson = structure["height"];
            if (heightJson != nullptr) {
                std::vector<short>* heightMap(new std::vector<short>);
                for (int i = 0; i < 256; i++) {
                    heightMap->push_back(heightJson[i]);
                }
                levelChunk->setPreWorldGenHeightMap(std::unique_ptr<std::vector<short>>(heightMap));
                levelChunk->recomputeHeightMap(false);
            }
        }

        levelChunk->setUnsaved();
        return true;
    }
	
    // 100 chunks at most
    static bool isAvailableChunk(int x, int z, int dim_id) {
        // getExistingChunk  ���ܻ�õ�ǰû�б����ص��Ѿ����ɵ�����
        // getAvailableChunk ���ܻ�õ�ǰû�б����ص��Ѿ����ɵ�����
        // getGeneratedChunk ���ܻ�õ�ǰû�б����ص��Ѿ����ɵ�����
        try {
            Dimension* dimension = Level::getDimensionPtr(SS::dim_id);
            if (dimension != nullptr) {
                if (dimension->getChunkSource().getAvailableChunk(ChunkPos(x, z)) != nullptr) {
                    return true;
                }
            }
            return false;
        }
        catch (std::exception& e)
        {
            logger.error(fmt::format("Failed to check available chunk: {}", e.what()));
            return false;
        }
    }
};
