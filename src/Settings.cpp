#include "Settings.h"

std::map<std::string, std::pair<bool, SaveSettings::Scenarios>> SaveSettings::Menu::Open {
    {std::string(RE::ContainerMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenContainerMenu}},
    {std::string(RE::BarterMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenBarterMenu}},
    {std::string(RE::CraftingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenCraftingMenu}},
    {std::string(RE::DialogueMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenDialogueMenu}},
    {std::string(RE::FavoritesMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenFavoritesMenu}},
    {std::string(RE::InventoryMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenInventoryMenu}},

    {std::string(RE::LockpickingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenLockpickingMenu}},
    {std::string(RE::MagicMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenMagicMenu}},
    {std::string(RE::MapMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenMapMenu}},
};



std::map<std::string, std::pair<bool,SaveSettings::Scenarios>> SaveSettings::Menu::Close{
    {std::string(RE::ContainerMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseContainerMenu}},
    {std::string(RE::BarterMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseBarterMenu}},
    {std::string(RE::CraftingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseCraftingMenu}},
    {std::string(RE::DialogueMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseDialogueMenu}},
    {std::string(RE::FavoritesMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseFavoritesMenu}},
    {std::string(RE::InventoryMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseInventoryMenu}},

    {std::string(RE::LockpickingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseLockpickingMenu}},
    {std::string(RE::MagicMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseMagicMenu}},
    {std::string(RE::MapMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseMapMenu}},
};

//void GameLock::SetState(GameLock::State currentState) {
//    if (GameLock::lastState == currentState || !SaveSettings::freeze_game) return;
//    GameLock::lastState = currentState;
//    const auto main = RE::Main::GetSingleton();
//    if (currentState == GameLock::State::Locked) main->freezeTime = true;
//    else main->freezeTime = false;
//};

std::map<std::string, int> SaveSettings::Menu::After = {
    {std::string(RE::ContainerMenu::MENU_NAME), 0},
    {std::string(RE::BarterMenu::MENU_NAME), 0},
    {std::string(RE::CraftingMenu::MENU_NAME), 0},
    {std::string(RE::DialogueMenu::MENU_NAME), 0},
    {std::string(RE::FavoritesMenu::MENU_NAME), 0},
    {std::string(RE::GiftMenu::MENU_NAME), 0},
    {std::string(RE::InventoryMenu::MENU_NAME), 0},
    {std::string(RE::JournalMenu::MENU_NAME), 0},
    {std::string(RE::LevelUpMenu::MENU_NAME), 0},
    {std::string(RE::LockpickingMenu::MENU_NAME), 0},
    {std::string(RE::MagicMenu::MENU_NAME), 0},
    {std::string(RE::MapMenu::MENU_NAME), 0},
    {std::string(RE::MessageBoxMenu::MENU_NAME), 0},
    {std::string(RE::SleepWaitMenu::MENU_NAME), 0},
    {std::string(RE::StatsMenu::MENU_NAME), 0},
    {std::string(RE::TrainingMenu::MENU_NAME), 0},
    {std::string(RE::TutorialMenu::MENU_NAME), 0},
    {std::string(RE::TweenMenu::MENU_NAME), 0},
    {std::string(RE::Console::MENU_NAME), 0},
};

void SaveSettings::SaveJSON() {
    
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value version(rapidjson::kObjectType);
    version.AddMember("major", Utilities::plugin_version.major(), allocator);
    version.AddMember("minor", Utilities::plugin_version.minor(), allocator);
    version.AddMember("patch", Utilities::plugin_version.patch(), allocator);
    version.AddMember("build", Utilities::plugin_version.build(), allocator);

    doc.AddMember("plugin_version", version, allocator);

    doc.AddMember("main", SaveSettings::to_json_main_stuff(allocator), allocator);
    doc.AddMember("timer", SaveSettings::to_json_timer_stuff(allocator), allocator);
    doc.AddMember("menu", SaveSettings::Menu::to_json(allocator), allocator);
    doc.AddMember("sleep_wait", SaveSettings::SleepWait::to_json(allocator), allocator);
    

    // Convert JSON document to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Write JSON to file
    std::string filename = settings_save_path;
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        logger::error("Failed to open file for writing: {}", filename);
        return;
    }
    ofs << buffer.GetString() << std::endl;
    ofs.close();

}
void SaveSettings::LoadJSON(){

    // load the save file

    if (!std::filesystem::exists(settings_save_path)) {
        logger::info("Settings file does not exist. Creating default settings.");
        SaveSettings::SaveJSON();
        return;
    }

    std::ifstream ifs(settings_save_path);

    if (!ifs.is_open()) {
		logger::error("Failed to open file for reading: {}", settings_save_path);
		return;
	}

    rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);

	if (doc.HasParseError()) {
		logger::error("Failed to parse JSON file: {}", rapidjson::GetParseError_En(doc.GetParseError()));
		return;
	}

	if (!doc.HasMember("main") || !doc["main"].IsObject()) {
		logger::error("Failed to parse JSON file: main");
		return;
	}

	if (!doc.HasMember("timer") || !doc["timer"].IsObject()) {
		logger::error("Failed to parse JSON file: timer");
		return;
	}

	if (!doc.HasMember("menu") || !doc["menu"].IsObject()) {
		logger::error("Failed to parse JSON file: menu");
		return;
	}

	if (!doc.HasMember("sleep_wait") || !doc["sleep_wait"].IsObject()) {
		logger::error("Failed to parse JSON file: sleep_wait");
		return;
	}

	// Main
	const auto& main = doc["main"];
	if (main.HasMember("block") && main["block"].IsBool()) block = main["block"].GetBool();
	if (main.HasMember("queue_delay") && main["queue_delay"].IsInt()) queue_delay = main["queue_delay"].GetInt();
	if (main.HasMember("ticker_interval") && main["ticker_interval"].IsInt()) ticker_interval = main["ticker_interval"].GetInt();
	if (main.HasMember("notifications") && main["notifications"].IsBool()) notifications = main["notifications"].GetBool();

	// Timer
	const auto& timer = doc["timer"];
	if (timer.HasMember("timer_minutes") && timer["timer_minutes"].IsInt()) timer_minutes = timer["timer_minutes"].GetInt();
	if (timer.HasMember("timer_seconds") && timer["timer_seconds"].IsInt()) timer_seconds = timer["timer_seconds"].GetInt();
	if (timer.HasMember("timer_running") && timer["timer_running"].IsBool()) timer_running = timer["timer_running"].GetBool();
	if (timer.HasMember("timer_periodic") && timer["timer_periodic"].IsBool()) timer_periodic = timer["timer_periodic"].GetBool();
	if (timer.HasMember("close_game_warning") && timer["close_game_warning"].IsBool()) close_game_warning = timer["close_game_warning"].GetBool();

    // Menu
    const auto& menu = doc["menu"];
    if (menu.HasMember("Open") && menu["Open"].IsObject()) {
		const auto& open = menu["Open"];
		for (auto it = open.MemberBegin(); it != open.MemberEnd(); ++it) {
			const std::string name = it->name.GetString();
			if (SaveSettings::Menu::Open.find(name) != SaveSettings::Menu::Open.end()) {
				const auto& value = it->value;
				if (value.HasMember("enabled") && value["enabled"].IsBool()) SaveSettings::Menu::Open[name].first = value["enabled"].GetBool();
				if (value.HasMember("scenario") && value["scenario"].IsInt()) SaveSettings::Menu::Open[name].second = static_cast<SaveSettings::Scenarios>(value["scenario"].GetInt());
			}
		}
	}
    if (menu.HasMember("Close") && menu["Close"].IsObject()) {
        const auto& close = menu["Close"];
        for (auto it = close.MemberBegin(); it != close.MemberEnd(); ++it) {
			const std::string name = it->name.GetString();
			if (SaveSettings::Menu::Close.find(name) != SaveSettings::Menu::Close.end()) {
				const auto& value = it->value;
				if (value.HasMember("enabled") && value["enabled"].IsBool()) SaveSettings::Menu::Close[name].first = value["enabled"].GetBool();
				if (value.HasMember("scenario") && value["scenario"].IsInt()) SaveSettings::Menu::Close[name].second = static_cast<SaveSettings::Scenarios>(value["scenario"].GetInt());
			}
		}
    }

	if (menu.HasMember("After") && menu["After"].IsObject()) {
		const auto& after = menu["After"];
		for (auto it = after.MemberBegin(); it != after.MemberEnd(); ++it) {
			const std::string name = it->name.GetString();
			if (SaveSettings::Menu::After.find(name) != SaveSettings::Menu::After.end()) {
				const auto& value = it->value;
				if (value.IsInt()) SaveSettings::Menu::After[name] = value.GetInt();
			}
		}
	}

	// SleepWait
	const auto& sleep_wait = doc["sleep_wait"];
    if (sleep_wait.HasMember("sleep") && sleep_wait["sleep"].IsBool()) SleepWait::sleep = sleep_wait["sleep"].GetBool();
    if (sleep_wait.HasMember("wait") && sleep_wait["wait"].IsBool()) SleepWait::wait = sleep_wait["wait"].GetBool();
	if (sleep_wait.HasMember("sleep_time") && sleep_wait["sleep_time"].IsInt()) SleepWait::sleep_time = sleep_wait["sleep_time"].GetInt();
	if (sleep_wait.HasMember("wait_time") && sleep_wait["wait_time"].IsInt()) SleepWait::wait_time = sleep_wait["wait_time"].GetInt();

	logger::info("Settings loaded successfully.");


}
uint32_t SaveSettings::GetSaveFlag() { return regular_saves ? 0xf0000080 : 0xf0000040; }

rapidjson::Value SaveSettings::to_json_main_stuff(Document::AllocatorType& a) {
    Value main_stuff(kObjectType);

    main_stuff.AddMember("block", block, a);
    main_stuff.AddMember("queue_delay", queue_delay, a);
    main_stuff.AddMember("ticker_interval", ticker_interval, a);
    main_stuff.AddMember("notifications", notifications, a);
    
    return main_stuff;
};

rapidjson::Value SaveSettings::to_json_timer_stuff(Document::AllocatorType& a) {
	Value timer_stuff(kObjectType);

	timer_stuff.AddMember("timer_minutes", timer_minutes, a);
	timer_stuff.AddMember("timer_seconds", timer_seconds, a);
	timer_stuff.AddMember("timer_running", timer_running, a);
	timer_stuff.AddMember("timer_periodic", timer_periodic, a);
	timer_stuff.AddMember("close_game_warning", close_game_warning, a);
	timer_stuff.AddMember("close_game", close_game, a);
	timer_stuff.AddMember("save_on_quit", save_on_quit, a);

	return timer_stuff;
};

rapidjson::Value SaveSettings::Menu::to_json(Document::AllocatorType& a){
    rapidjson::Value menu(rapidjson::kObjectType);
    
    // Open
    rapidjson::Value open(rapidjson::kObjectType);
    for (auto& [name, value] : Open) {
        rapidjson::Value menu_name(rapidjson::kObjectType);
        menu_name.AddMember("enabled", value.first, a);
        menu_name.AddMember("scenario", static_cast<int>(value.second), a);
        open.AddMember(rapidjson::Value().SetString(name.c_str(), name.size(), a), menu_name, a);
    }
    menu.AddMember("Open", open, a);
    
    // Close
    rapidjson::Value close(rapidjson::kObjectType);
    for (auto& [name, value] : Close) {
        rapidjson::Value menu_name(rapidjson::kObjectType);
        menu_name.AddMember("enabled", value.first, a);
        menu_name.AddMember("scenario", static_cast<int>(value.second), a);
        close.AddMember(rapidjson::Value().SetString(name.c_str(), name.size(), a), menu_name, a);
    }
    menu.AddMember("Close", close, a);
    
    // After
    rapidjson::Value after(rapidjson::kObjectType);
    for (auto& [name, value] : After) {
        after.AddMember(rapidjson::Value().SetString(name.c_str(), name.size(), a), value, a);
    }
    menu.AddMember("After", after, a);
    
    return menu;
}

rapidjson::Value SaveSettings::SleepWait::to_json(Document::AllocatorType& a){
    Value sleep_wait(kObjectType);
    sleep_wait.AddMember("sleep", sleep, a);
    sleep_wait.AddMember("wait", wait, a);
    sleep_wait.AddMember("sleep_time", sleep_time, a);
    sleep_wait.AddMember("wait_time", wait_time, a);
    return sleep_wait;
};
