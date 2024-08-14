#include "UI.h"

void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


//std::vector<std::string> MCP::logLines;

void MCP::Register(Manager* manager) {
    
    M = manager;

    if (!SKSEMenuFramework::IsInstalled()) {
		logger::error("SKSEMenuFramework is not installed.");
        M->DisableMod();
		return;
	}

    SKSEMenuFramework::SetSection(Utilities::mod_name);
    SKSEMenuFramework::AddSectionItem("Settings", RenderSettings);
    SKSEMenuFramework::AddSectionItem("Status", RenderStatus);
    SKSEMenuFramework::AddSectionItem("Log", RenderLog);

}

void __stdcall MCP::RenderSettings() {
    // setting for freezing game when saving
    // ImGui::Checkbox("Freeze Game", &SaveSettings::freeze_game);
    // ImGui::SameLine();
    // HelpMarker("Freeze the game when saving");

    if (ImGui::Button("Save Settings")) SaveSettings::SaveJSON();
    ImGui::SameLine();
    ImGui::Checkbox("Auto Save", &SaveSettings::auto_save_to_json);
    ImGui::SameLine();
    HelpMarker("Save settings automatically upon game save");
    ImGui::SameLine();
    ImGui::Checkbox("Regular Saves", &SaveSettings::regular_saves);
    ImGui::SameLine();
    HelpMarker("Regular saves instead of auto saves.");

    ImGui::Checkbox("Notifications", &SaveSettings::notifications);
    ImGui::SameLine();
    HelpMarker("Show notifications when saving");

    ImGui::SameLine();

    // setting for blocking saves
    ImGui::Checkbox("Block Saves", &SaveSettings::block);
    ImGui::SameLine();
    HelpMarker("Block all saves");

    ImGui::SameLine();

    if (ImGui::Button("Clear")) M->ClearQueue();
    ImGui::SameLine();
    HelpMarker("Clear the save queue");

    ImGui::Text("");
    
    MCP::Settings::RenderTimer();

    ImGui::Text("");

    MCP::Settings::RenderCollapseExpandAll();

    // Default header state to true (open) if not already set
    for (auto& header_name : header_names) {
        if (headerStates.find(header_name) == headerStates.end()) {
            headerStates[header_name] = true;
        }
    }

    // Render each header
    MCP::Settings::RenderMenu();

    MCP::Settings::RenderSleepWait();
};

void MCP::Settings::RenderCollapseExpandAll() {
    // checkbox for collapse all below
    if (ImGui::Button("Collapse All")) {
        for (auto& header_name : header_names) {
            headerStates[header_name] = false;
        }
    }

    // checkbox for expand all below
    ImGui::SameLine();
    if (ImGui::Button("Expand All")) {
        for (auto& header_name : header_names) {
            headerStates[header_name] = true;
        }
    }
}

void MCP::Settings::RenderTimer(){
    
    // Add timer to save game after x minutes with Start/Stop/Reset buttons
    
    // show remaining time in seconds, where we dont know if the timer is running or not or if it is the first
    // element in the queue
    int temp_remaining = 0;
    for (const auto& [t, reason] : M->GetQueue()) {
        if (reason == SaveSettings::Scenarios::Timer) {
            temp_remaining = t;
            break;
        }
    }

    if (ImGui::CollapsingHeader("Timer##header", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Periodic", &SaveSettings::timer_periodic);
        ImGui::SameLine();
        ImGui::Text("Minutes:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputInt("##minutes", &SaveSettings::timer_minutes);
        ImGui::SameLine();
        ImGui::Text("Seconds:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputInt("##timer_seconds", &SaveSettings::timer_seconds);
        ImGui::SameLine();
        if (ImGui::Button("Start##timer")) {
            M->QueueSaveGame(SaveSettings::timer_minutes * 60 + SaveSettings::timer_seconds,
                             SaveSettings::Scenarios::Timer);
            SaveSettings::timer_running = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop##timer")) {
            M->DeleteQueuedSave(SaveSettings::Scenarios::Timer);
            SaveSettings::timer_running = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            if (M->DeleteQueuedSave(SaveSettings::Scenarios::Timer)) {
                M->QueueSaveGame(SaveSettings::timer_minutes * 60 + SaveSettings::timer_seconds,
                                 SaveSettings::Scenarios::Timer);
                SaveSettings::timer_running = true;
            } else {
                SaveSettings::timer_minutes = 0;
                SaveSettings::timer_seconds = 0;
                SaveSettings::timer_running = false;
            }
        }
        ImGui::SameLine();
        ImGui::Text("Remaining: %d", temp_remaining);

        // add checkbox for warning when time is up
        ImGui::Checkbox("Close Game Warning", &SaveSettings::close_game_warning);
        ImGui::SameLine();
        HelpMarker("Show warning when time is up");
        // add checkbox for closing the game when time is up
        ImGui::SameLine();
        ImGui::Checkbox("Close Game", &SaveSettings::close_game);
        ImGui::SameLine();
        HelpMarker("Close the game when time is up");
    }
};

void MCP::Settings::RenderMenu() {

    if (!headerStates["Menu"]) ImGui::SetNextItemOpen(false);
    else ImGui::SetNextItemOpen(true);
    if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Reset All")) {
            for (auto& [menu_name, _] : SaveSettings::Menu::Open) {
			    SaveSettings::Menu::Open[menu_name].first = false;
			    SaveSettings::Menu::Close[menu_name].first = false;
			    SaveSettings::Menu::After[menu_name] = 0;
		    }
        }
        headerStates["Menu"] = true;

        float maxTextWidth = 200.0f;
        //for (const auto& [menu_name, _] : SaveSettings::Menu::Open) {
            //maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(menu_name.c_str()).x);
        //}

        for (auto& [menu_name, _] : SaveSettings::Menu::Open) {
            bool setting_open = SaveSettings::Menu::Open[menu_name].first;
            bool setting_close = SaveSettings::Menu::Close[menu_name].first;
            int setting_after = SaveSettings::Menu::After[menu_name];
            
            ImGui::Text(menu_name.c_str());
            ImGui::SameLine();
            ImGui::SetCursorPosX(maxTextWidth + 20);  // Adjust the 20 value to set spacing
            std::string checkbox_name_open = "Open##" + menu_name;
            ImGui::Checkbox(checkbox_name_open.c_str(), &setting_open);
            ImGui::SameLine();
            std::string checkbox_name_close = "Close##" + menu_name;
            ImGui::Checkbox(checkbox_name_close.c_str(), &setting_close);
            // add text with field for seconds to wait after menu is closed
            ImGui::SameLine();
            ImGui::Text("After");
            ImGui::SameLine();
            std::string input_name = "##" + menu_name;
            ImGui::SetNextItemWidth(500);
            ImGui::InputInt(input_name.c_str(), &setting_after);
            ImGui::SameLine();
            HelpMarker("Seconds to wait after menu is closed before saving");
            
            SaveSettings::Menu::Open[menu_name].first = setting_open;
            SaveSettings::Menu::Close[menu_name].first = setting_close;
            SaveSettings::Menu::After[menu_name] = setting_after;
        };
    } else headerStates["Menu"] = false;
}

void MCP::Settings::RenderSleepWait(){

    if (!headerStates["SleepWait"]) ImGui::SetNextItemOpen(false);
    else ImGui::SetNextItemOpen(true);
    if (ImGui::CollapsingHeader("Sleep/Wait", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Reset All")) {
			SaveSettings::SleepWait::sleep = false;
			SaveSettings::SleepWait::wait = false;
			SaveSettings::SleepWait::sleep_time = 0;
			SaveSettings::SleepWait::wait_time = 0;
        }
        headerStates["SleepWait"] = true;

        float maxTextWidth = 200.0f;
        //for (const auto& temp_name : {"Sleep", "Wait"}) {
            //maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(temp_name).x);
        //}
            
        ImGui::Checkbox("Sleep", &SaveSettings::SleepWait::sleep);
        ImGui::SameLine();
        ImGui::SetCursorPosX(maxTextWidth + 20);  // Adjust the 20 value to set spacing
        ImGui::Text("After");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(500);
        ImGui::InputInt("##sleep_time", &SaveSettings::SleepWait::sleep_time);
        ImGui::SameLine();
        HelpMarker("Seconds to save after waking up");

        ImGui::Checkbox("Wait", &SaveSettings::SleepWait::wait);
        ImGui::SameLine();
        ImGui::SetCursorPosX(maxTextWidth + 20);  // Adjust the 20 value to set spacing
        ImGui::Text("After");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(500);
        ImGui::InputInt("##wait_time", &SaveSettings::SleepWait::wait_time);
        ImGui::SameLine();
        HelpMarker("Seconds to save after waiting");
    } else headerStates["SleepWait"] = false;
    
};

void __stdcall MCP::RenderStatus(){
    // Status of the plugin with color green if running, red if stopped
    ImGui::Text("Status: ");
    ImGui::SameLine();
    if (PluginSettings::running) {
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Running");
	} else {
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Stopped");
	}

    // add disable/enable button if enabled/disabled

    if (PluginSettings::running) {
		if (ImGui::Button("Disable Mod")) {
			M->DisableMod();
		}
	} else {
		if (ImGui::Button("Enable Mod")) {
			M->EnableMod();
		}
	}

    // add refresh button to reload the queue

    //FontAwesome::PushSolid();
    //if (ImGui::Button((FontAwesome::UnicodeToUtf8(0xf021) + " Refresh").c_str())) {
    //    queue_cache = M->GetQueue();
    //}
    //FontAwesome::Pop();
    // display the queue in table format
    ImGui::Columns(2, "Queue", true);
    ImGui::Text("Time");
    ImGui::NextColumn();
    ImGui::Text("Reason");
    ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& [t, reason] : M->GetQueue()) {
        ImGui::Text("%d", t);
		ImGui::NextColumn();
        if (!SaveSettings::scenario_names.count(reason)) ImGui::Text("Unknown");
        else ImGui::Text(SaveSettings::scenario_names[reason].c_str());
		ImGui::NextColumn();
	}
};

void __stdcall MCP::RenderLog() {

    // add checkboxes to filter log levels
    ImGui::Checkbox("Trace", &LogSettings::log_trace);
    ImGui::SameLine();
    ImGui::Checkbox("Info", &LogSettings::log_info);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &LogSettings::log_warning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &LogSettings::log_error);


    // if "Generate Log" button is pressed, read the log file
    if (ImGui::Button("Generate Log")) {
		logLines = Utilities::ReadLogFile();
	}

    // Display each line in a new ImGui::Text() element
    for (const auto& line : logLines) {
        if (line.find("trace") != std::string::npos && !LogSettings::log_trace) continue;
        if (line.find("info") != std::string::npos && !LogSettings::log_info) continue;
        if (line.find("warning") != std::string::npos && !LogSettings::log_warning) continue;
        if (line.find("error") != std::string::npos && !LogSettings::log_error) continue;
		ImGui::Text(line.c_str());
	}
    
}


