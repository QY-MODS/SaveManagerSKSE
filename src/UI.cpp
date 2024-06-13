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

void MCP::Register() {
    SKSEMenuFramework::SetSection(Utilities::mod_name);
    SKSEMenuFramework::AddSectionItem("Settings", RenderSettings);
    SKSEMenuFramework::AddSectionItem("Status", RenderStatus);
    SKSEMenuFramework::AddSectionItem("Log", RenderLog);
}

void __stdcall MCP::RenderSettings() {

    // setting for freezing game when saving
    //ImGui::Checkbox("Freeze Game", &SaveSettings::freeze_game);
    //ImGui::SameLine();
    //HelpMarker("Freeze the game when saving");

    MCP::Settings::RenderCollapseExpandAll();

    // Default header state to true (open) if not already set
    for (auto& header_name : header_names) {
        if (headerStates.find(header_name) == headerStates.end()) {
            headerStates[header_name] = true;
        }
    }

    // Render each header
    MCP::Settings::RenderMenu();

    
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

void MCP::Settings::RenderMenu() {
    if (!headerStates["Menu"]) ImGui::SetNextItemOpen(false);
    else ImGui::SetNextItemOpen(true);
    if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
        headerStates["Menu"] = true;

        float maxTextWidth = 0.0f;
        for (const auto& [menu_name, _] : SaveSettings::Menu::Open) {
            maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(menu_name.c_str()).x);
        }

        for (auto& [menu_name, _] : SaveSettings::Menu::Open) {
            bool setting_open = SaveSettings::Menu::Open[menu_name].first;
            bool setting_close = SaveSettings::Menu::Close[menu_name].first;
            
            ImGui::Text(menu_name.c_str());
            ImGui::SameLine();
            ImGui::SetCursorPosX(maxTextWidth + 20);  // Adjust the 20 value to set spacing
            std::string checkbox_name_open = "Open##" + menu_name;
            ImGui::Checkbox(checkbox_name_open.c_str(), &setting_open);
            ImGui::SameLine();
            std::string checkbox_name_close = "Close##" + menu_name;
            ImGui::Checkbox(checkbox_name_close.c_str(), &setting_close);
            
            SaveSettings::Menu::Open[menu_name].first = setting_open;
            SaveSettings::Menu::Close[menu_name].first = setting_close;
        };
    } else headerStates["Menu"] = false;
}

void __stdcall MCP::RenderStatus() {}

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


