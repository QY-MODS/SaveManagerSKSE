#pragma once

#include "rapidjson/document.h"
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

//#include <chrono>
#include <windows.h>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "SimpleIni.h"
#include <iostream>
#include <string>
#include <codecvt>
#include <mutex>
#include <algorithm>
#include <ClibUtil/editorID.hpp>
#include <filesystem>
#include <fstream>



namespace Utilities {

    const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());
    const auto plugin_version = SKSE::PluginDeclaration::GetSingleton()->GetVersion();

	std::filesystem::path GetLogPath();

	std::vector<std::string> ReadLogFile();

	inline bool GetCombatState() { return RE::PlayerCharacter::GetSingleton()->IsInCombat(); }

	void AutoSave(uint32_t flag = 0xf0000040);
    void QuitGame();

	static void BuildSaveGameList(RE::BGSSaveLoadManager* a_manager) {
        using func_t = decltype(&BuildSaveGameList);
        REL::Relocation<func_t> func{RELOCATION_ID(34850, 35760)};
        return func(a_manager);
    }

};

static void denemelerr(){
    const auto manager = RE::BGSSaveLoadManager::GetSingleton();
    Utilities::BuildSaveGameList(manager);
    const auto& temp_map = manager->playerIDNameMap;
    logger::info("temp_map size {}", temp_map.size());

    auto& savelist = manager->saveGameList;
    size_t savelist_size = 0;
    for (auto& it : savelist) {
        savelist_size++;
        logger::info("Player Name {} location {} Filename {} ", it->playerName, it->location, it->fileName);
        /*logger::info("Player Name {} location {} Player title {}", (*it)->playerName.c_str(), (*it)->location,
                     (*it)->playerTitle.c_str());*/
    }
    logger ::info("PostLoadGame: SaveList size: {}", savelist_size);

};


