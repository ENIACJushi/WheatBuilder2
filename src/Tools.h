#pragma once
#include <Nlohmann/json.hpp>
#include <llapi/LoggerAPI.h>
#include <llapi/mc/ChunkPos.hpp>
#include <llapi/mc/BlockPos.hpp>
#include <llapi/mc/ChunkBlockPos.hpp>
#include <direct.h>

#include "Config.h"
using nlohmann::json;

extern Logger logger;

namespace wb {
    struct Area
    {
        Area(int sx, int sz, int ex, int ez, int dim)
            :start_x(sx), start_z(sz), end_x(ex), end_z(ez), dim_id(dim) {}
        int start_x;
        int start_z;
        int end_x;
        int end_z;
        int dim_id;
    };
    class AreaList {
    public:
        AreaList() {};
        vector<Area> list;
        Area get(int index) { return list[index]; }
        void push(Area area) { list.push_back(area); }
        int  size() { return list.size(); }
    };
}

class PositionTool
{
public:
	static std::pair<ChunkPos*, ChunkBlockPos> block2chunkPos(int x, int y, int z) {
		ChunkPos* chunk = new ChunkPos(floor((double)x / 16), floor((double)z / 16));
		ChunkBlockPos block(x - chunk->x * 16, y, z - chunk->z * 16);
		// std::cout << x << " " << y << "" << z << ":" << chunk->x << " " << chunk->z << " ." << block.x << " " << " " << block.y << block.z << std::endl;
		return std::pair(chunk, block);
	}
    // start <= end
	static void arrangeCoordinate_2D(int& start_x, int& start_z, int& end_x, int& end_z) {
        if (start_x > end_x) {
            int temp = start_x;
            start_x = end_x;
            end_x = temp;
        }
        if (start_z > start_z) {
            int temp = start_z;
            start_z = end_z;
            end_z = temp;
        }
	}
    // Divide a big area to small area with 100 chunks at most
    // must ensure start_x/z < end_x/z before use
    static wb::AreaList divideArea(int& start_x, int start_z, int end_x, int end_z, int dim_id) {
        int x_offset = end_x - start_x + 1;
        int z_offset = end_z - start_z + 1;

        // �滮����ִ��, һ�����������������100������
        // ÿ�����������ṹ:[start_x, start,z, end_x, end_z, dim_id]
        wb::AreaList area_process_list;
        // �ȸ���10*10������
        int x_amount = floor(x_offset / 10); // x����߳�Ϊ10���������� ��
        int z_amount = floor(z_offset / 10); // z����߳�Ϊ10���������� ��
        for (int ix = 0; ix < x_amount; ix++) {
            for (int iz = 0; iz < z_amount; iz++) {
                int sx = start_x + ix * 10;
                int sz = start_z + iz * 10;
                area_process_list.push(wb::Area(sx, sz, sx + 9, sz + 9, dim_id));
            }
        }
        // Ȼ�󸲸Ǳ߽�����, ��x��z
        int side_offset_z = z_offset - z_amount * 10; // z����ı߽����鳤��
        // ����x ��
        if (side_offset_z > 0) {
            int x_start = start_x;                 // ��ǰ�����������ʼ��������x
            int z_start = start_z + z_amount * 10; // ��ǰ�����������ʼ��������z
            int x_left = x_offset;                // �������x���򳤶�
            int x_max = floor(100 / side_offset_z); // һ���ƽ�����󳤶�
            while (true) {
                if (x_left <= 0) {
                    break;
                }
                else {
                    if (x_left >= x_max) {
                        area_process_list.push(wb::Area(x_start, z_start, x_start + x_max - 1, end_z, dim_id));
                        x_start += x_max;
                        x_left -= x_max;
                    }
                    else {
                        area_process_list.push(wb::Area(x_start, z_start, x_start + x_left - 1, end_z, dim_id));
                        x_start += x_left;
                        x_left -= x_left;
                    }
                }
            }
        }
        // ����z ��
        int side_offset_x = x_offset - x_amount * 10; // x����ı߽����鳤��
        if (side_offset_x > 0) {
            int x_start = start_x + x_amount * 10; // ��ǰ�����������ʼ��������x
            int z_start = start_z;                 // ��ǰ�����������ʼ��������z
            int z_left = z_amount * 10;           // �������z���򳤶�
            int z_max = floor(100 / side_offset_x);     // һ���ƽ�����󳤶�
            while (true) {
                if (z_left <= 0) {
                    break;
                }
                else {
                    if (z_left >= z_max) {
                        area_process_list.push(wb::Area(x_start, z_start, end_x, z_start + z_max - 1, dim_id));
                        z_start += z_max;
                        z_left -= z_max;
                    }
                    else {
                        area_process_list.push(wb::Area(x_start, z_start, end_x, z_start + z_left - 1, dim_id));
                        z_start += z_left;
                        z_left -= z_left;
                    }
                }
            }
        }
        return area_process_list;
    }
};

class FileTool {
public:
    static void makeDir(string name) {
        if (_access(name.c_str(), 0) != 0) {
            if (!_mkdir(name.c_str())) {
                logger.fatal("Dir \"" + name + "\" make failed.");
            }
        }
    }
    static bool saveJson(nlohmann::json json, string filePath) {
        std::ofstream file;
        file.open(filePath, std::ios::out | std::ios::_Noreplace);
        if (file.is_open()) {
            file << json.dump();
            return true;
        }
        return false;
    }

    static nlohmann::json loadJson(string filePath) {
        nlohmann::json result;
        std::ifstream inputFile;
        inputFile.open(filePath);
        if (inputFile.is_open()) {
            std::istreambuf_iterator<char> beg(inputFile), end;
            string strFileData = std::string(beg, end);
            inputFile.seekg(0, std::ios::end);//�ƶ����ļ�β��
            inputFile.close();
            try
            {
                result = json::parse(strFileData.c_str(), nullptr, false);
                if (result.is_discarded())
                {
                    logger.error("Parse config json data failed.");
                    return nullptr;
                }
                return result;
            }
            catch (std::exception& e)
            {
                logger.error(fmt::format("Failed to load json file: {}, {}", e.what(), strFileData));
                return nullptr;
            }
        }
        else {
            return nullptr;
        }
    }
};