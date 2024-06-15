#pragma once
#include "Events.h"
#include "SKSEMenuFramework.h"

static void HelpMarker(const char* desc);

namespace MCP {

	inline Manager* M = nullptr;

	inline std::string log_path = Utilities::GetLogPath().string();
    inline std::vector<std::string> logLines;

	inline std::unordered_map<std::string, bool> headerStates;
    inline std::vector header_names = {"Menu", "SleepWait"};

	void Register(Manager* manager);

	void __stdcall RenderSettings();
    void __stdcall RenderStatus();
    void __stdcall RenderLog();

	namespace Settings {
		void RenderCollapseExpandAll();
		void RenderTimer();
		void RenderMenu();

		void RenderSleepWait();

	};
};