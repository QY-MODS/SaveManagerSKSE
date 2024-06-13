#include "Manager.h"


void Manager::Uninstall() {}

void Manager::QueueSaveGame(int seconds, SaveSettings::Scenarios scenario) {
    if (seconds > 0 && queue.size()<100) {
        queue.insert(std::make_pair(seconds, scenario));
        Start();
    }
}

void Manager::UpdateLoop() {
    if (queue.empty()) {
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

    if (queue.empty()) {
        Stop();
    }

    if (save) SaveGame(reason);
}
void Manager::Init(){};

void Manager::SaveGame(SaveSettings::Scenarios reason) {
    //if (auto ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) return QueueSaveGame(SaveSettings::queue_delay,reason);
    logger::info("Saving game...");
    if (SaveSettings::block) return QueueSaveGame(SaveSettings::queue_delay, reason);
    
    SKSE::GetTaskInterface()->AddTask([]() {
        auto slm = RE::BGSSaveLoadManager::GetSingleton();
        if (!slm) return;
        if (slm->thread.isBusy) return;
        //if (SaveSettings::freeze_game) GameLock::SetState(GameLock::State::Locked);
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;
        const auto player_name = std::string(player->GetName());
        const std::string date = "_" + std::format("{:%y%m%d%H%M%S}", std::chrono::system_clock::now());
        auto cell = player->GetParentCell();
        const auto ws = player->GetWorldspace(); 
        auto player_parentcell = cell ? std::string(cell->GetFullName()) : "";
        auto player_ws = ws ? std::string(ws->GetFullName()) : "";
        if (!player_parentcell.empty()) player_parentcell = "_" + player_parentcell;
        if (!player_ws.empty()) player_ws = "_" + player_ws;
        slm->Save((player_name + player_ws + player_parentcell + date).c_str());
        //GameLock::SetState(GameLock::State::Unlocked);
	});
}