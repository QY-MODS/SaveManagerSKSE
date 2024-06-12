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
    if (SaveSettings::block) return QueueSaveGame(SaveSettings::queue_delay, reason);
    SKSE::GetTaskInterface()->AddTask([]() {
        auto slm = RE::BGSSaveLoadManager::GetSingleton();
        const auto main = RE::Main::GetSingleton();
        if (!slm || !main) return;
        if (slm->thread.isBusy) return;
        //if (SaveSettings::freeze_game) GameLock::SetState(GameLock::State::Locked);
        slm->Save("asdasdasd");
        //GameLock::SetState(GameLock::State::Unlocked);
	});
}