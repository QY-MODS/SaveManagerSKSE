#pragma once

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
#include "rapidjson/document.h"
#include <filesystem>
#include <fstream>



namespace Utilities {

	const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());

	std::filesystem::path GetLogPath();

	std::vector<std::string> ReadLogFile();

	namespace Menu {
        
		bool IsOpen(const std::string_view menuname);
	};

	void AutoSave();

};


