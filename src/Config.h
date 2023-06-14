#pragma once
#include <string>
#include <Nlohmann/json.hpp>
#include <fstream>
#include <llapi/LoggerAPI.h>
#include <direct.h>
using nlohmann::json;

nlohmann::json Config;

#include <llapi/LoggerAPI.h>
extern Logger logger;

const nlohmann::json defaultConfig = nlohmann::json{
    {"tool", {
		{"wand", "minecraft:wooden_hoe"},
		{"menu", "minecraft:wooden_shovel"}
	}},
    {"openWE", true},
	{"dataPath", "E:/[Data] WheatBuilderData/"}
};

bool loadConfig(string folderPath, string fileName) {
    if (_access(folderPath.c_str(), 0) == -1) {
        logger.info("Plugin path not exist, create..");
        if (_mkdir(folderPath.c_str()) == -1) {
            logger.error("Dir \"" + folderPath + "\" make failed.");
        }
        else {
            logger.info("Create successfully.");
        }
    }

    // load Config.json
    std::fstream file;
    file.open(folderPath + "/" + fileName, std::ios::in);
    // 不存在，创建
    if (!file){
        logger.info("Config file not exist, create..");
        std::fstream newFile;
        newFile.open(folderPath + "/" + fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
        newFile << defaultConfig.dump(1);
        logger.info(defaultConfig.dump(1));
        newFile.close();
        Config = defaultConfig;
        logger.info("Create successfully.");
        return true;
    }
    else {
        std::istreambuf_iterator<char> beg(file), end;
        string configString = std::string(beg, end);
        file.seekg(0, std::ios::end); //移动到文件尾部
        file.close();
        try {
            Config = nlohmann::json::parse(configString.c_str(), nullptr, true);
            logger.info(Config.dump(1));
        }
        catch (const std::exception& ex) {
            logger.error(ex.what());
            return false;
        }
        return true;
    }
    return true;
}
