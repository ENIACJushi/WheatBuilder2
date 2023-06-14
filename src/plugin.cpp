/**
 * @file plugin.cpp
 * @brief The main file of the plugin
 */

#include <llapi/LoggerAPI.h>
#include <llapi/EventAPI.h>
#include <llapi/mc/Player.hpp>

#include "version.h"
#include "Config.h"
#include "BiomeManager.h"
#include "CommandManager.h"
#include "ChunkManager.h"

/* --------------------------------------- *\
 *  Name        :  BiomeEditor             *
 *  Description :  WheatBuilder series     *
 *  Version     :  1.0.0                   *
 *  Author      :  ENIAC_Jushi             *
\* --------------------------------------- */

// We recommend using the global logger.
extern Logger logger;

/**
 * @brief The entrypoint of the plugin. DO NOT remove or rename this function.
 *        
 */
void PluginInit()
{
    std::cout << 1111;
    FileTool::makeDir("plugins/WheatBuilder");
    FileTool::makeDir("plugins/WheatBuilder/Biomes");
    FileTool::makeDir("plugins/WheatBuilder/Chunks");
    // loadConfig("plugins/WheatBuilder", "config.json")
    std::cout << 2222;
    CommandManager::set();
    
    Event::ServerStartedEvent::subscribe([](const Event::ServerStartedEvent) {
            
            return true;
        });
    logger.info("WheatBuilder: BiomeEditor and MoveChunk loaded.");
}
