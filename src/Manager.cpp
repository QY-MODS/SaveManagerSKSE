#include "Manager.h"


void Manager::Uninstall() {}

void Manager::DisableMod(){
    PluginSettings::running = false;
    ClearQueue();
}
void Manager::EnableMod(){
    PluginSettings::running = true;
};

void Manager::QueueSaveGame(int seconds, SaveSettings::Scenarios scenario) {
    // mutex lock
    //std::lock_guard<std::mutex> lock(mutex);

    if (SaveSettings::block) return;
    if (!PluginSettings::running) return;

    // if Timer is queued, dont allow a second one
    if (scenario == SaveSettings::Scenarios::Timer) {
		for (auto& entry : queue) {
			if (entry.second == SaveSettings::Scenarios::Timer) return;
		}
	}

    seconds = std::max(1, seconds);

    if (seconds > 0 && queue.size() < 100) {
        queue.insert(std::make_pair(seconds, scenario));
        const auto temp = std::format("Save queued for {} second(s).", seconds);
        if (SaveSettings::notifications && scenario != SaveSettings::Scenarios::QuitGame) RE::DebugNotification(temp.c_str());
        Start();
    }
}

const std::vector<std::pair<int, SaveSettings::Scenarios>> Manager::GetQueue() {
    // mutex lock
    std::lock_guard<std::mutex> lock(mutex);
    return std::vector<std::pair<int, SaveSettings::Scenarios>>(queue.begin(), queue.end());
}

bool Manager::DeleteQueuedSave(SaveSettings::Scenarios scenario){
    // mutex lock
	std::lock_guard<std::mutex> lock(mutex);
    bool deleted = false;
	for (auto it = queue.begin(); it != queue.end();) {
		if (it->second == scenario) {
			it = queue.erase(it);
            deleted = true;
		} else ++it;
	}
    return deleted;
}

void Manager::ClearQueue(){
    Stop();
    // mutex lock
    std::lock_guard<std::mutex> lock(mutex);
    queue.clear();
};

void Manager::UpdateLoop() {

    if (m_Busy.exchange(true)) { 
        logger::trace("Busy flag set, returning...");
        return;
    }

    auto clearBusy = [](std::atomic<bool>* busyFlag) {
        //logger::trace("Clearing busy flag");
        busyFlag->store(false);
    };

    std::unique_ptr<std::atomic<bool>, decltype(clearBusy)> guard(&m_Busy, clearBusy);

    // mutex lock
    //std::lock_guard<std::mutex> lock(mutex);
    

    if (queue.empty()) {
        logger::trace("Queue is empty, stopping...");
        Stop();
        return;
    }

    // deduct time from all entries
    bool save = false;
    SaveSettings::Scenarios reason;
    const auto deduct = SaveSettings::ticker_interval;
    // unpack the queue to a vector
    std::vector<std::pair<int, SaveSettings::Scenarios>> queue_vector(queue.begin(), queue.end());
    for (auto it = queue_vector.begin(); it != queue_vector.end();) {
        it->first -= deduct;
        /*if (SaveSettings::notifications && SaveSettings::close_game && it->first <= 10 && it->second == SaveSettings::Scenarios::Timer) {
            RE::DebugNotification("Quitting game after saving in less than 10 seconds...");
        }*/
        if (it->first <= 0) {
            reason = it->second;
            it = queue_vector.erase(it);
            save = true;
        } else ++it;
    }

    // repack the vector to the queue
    queue.clear();
    for (auto& entry : queue_vector) {
		queue.insert(entry);
	}

    if (save) {
        bool saved = SaveGame(reason);
        if (reason == SaveSettings::Scenarios::Timer) {
            if (saved && SaveSettings::close_game) {
                RE::DebugNotification("Closing game in 10 seconds!");
                SaveSettings::close_game = false;
                QueueSaveGame(10, SaveSettings::Scenarios::QuitGame);
            }
            else if (SaveSettings::timer_periodic) {
                QueueSaveGame(SaveSettings::timer_minutes * 60 + SaveSettings::timer_seconds, SaveSettings::Scenarios::Timer);
            }
            else SaveSettings::timer_running = false;
		    if (SaveSettings::close_game_warning) RE::DebugMessageBox("Time is up, close the game!");
        }
    }

}
void Manager::Init(){};

bool Manager::SaveGame(SaveSettings::Scenarios reason) {
    if (reason == SaveSettings::Scenarios::QuitGame) {
		Utilities::QuitGame();
        return true;
	}
    //if (auto ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) return QueueSaveGame(SaveSettings::queue_delay,reason);
    auto ui = RE::UI::GetSingleton();
    if (!ui) {
        QueueSaveGame(SaveSettings::queue_delay, reason);
        return false;
    }
    if (SaveSettings::block || 
        RE::PlayerCharacter::GetSingleton()->IsDead() ||
        RE::PlayerCharacter::GetSingleton()->IsOnMount() ||
        !RE::PlayerCharacter::GetSingleton()->GetParentCell() ||
        RE::PlayerCharacter::GetSingleton()->IsInCombat() ||
        ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::MainMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::StatsMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::MapMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::JournalMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::GiftMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::BarterMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME) ||
        ui->IsMenuOpen(RE::TweenMenu::MENU_NAME)) {
        
        QueueSaveGame(SaveSettings::queue_delay, reason);
        return false;
    }
    
    logger::info("Saving game...");
    SKSE::GetTaskInterface()->AddTask([]() {
        //auto slm = RE::BGSSaveLoadManager::GetSingleton();
        //if (!slm) return;
        //if (slm->thread.isBusy) return;
        ////if (SaveSettings::freeze_game) GameLock::SetState(GameLock::State::Locked);
        //const auto player = RE::PlayerCharacter::GetSingleton();
        //if (!player) return;
        //auto cell = player->GetParentCell();
        //if (!cell) return;
        //const auto ws = player->GetWorldspace(); 
        //if (!ws) return;
        //const auto player_name = std::string(player->GetName());
        //const std::string date = "_" + std::format("{:%y%m%d%H%M%S}", std::chrono::system_clock::now());
        //auto player_parentcell = std::string(cell->GetFullName());
        //auto player_ws = std::string(ws->GetFullName());
        //if (!player_parentcell.empty()) player_parentcell = "_" + player_parentcell;
        //if (!player_ws.empty()) player_ws = "_" + player_ws;
        //slm->Save((player_name + player_ws + player_parentcell + date).c_str());
        //GameLock::SetState(GameLock::State::Unlocked);

        MainSaveFunction();
	});
	return true;
}
