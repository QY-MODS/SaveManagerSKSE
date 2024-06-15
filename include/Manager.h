#pragma once
#include "Ticker.h"

struct PairFirstComparator {
    bool operator()(const std::pair<int, SaveSettings::Scenarios>& lhs,
                    const std::pair<int, SaveSettings::Scenarios>& rhs) const {
        return lhs.first < rhs.first;
    }
};

class Manager : public Ticker {
    
private:

    std::mutex mutex;

    std::set<std::pair<int, SaveSettings::Scenarios>, PairFirstComparator> queue;
    
    void UpdateLoop();
    void Init();
    void SaveGame(SaveSettings::Scenarios reason);
    void QuitGame();

public:
    Manager()
        : Ticker([this]() { UpdateLoop(); }) {
        Init();
    }

    static Manager* GetSingleton() {
        static Manager singleton;
        return &singleton;
    }

    void Uninstall();

    void QueueSaveGame(int seconds, SaveSettings::Scenarios scenario);

    const std::vector<std::pair<int, SaveSettings::Scenarios>> GetQueue();

    bool DeleteQueuedSave(SaveSettings::Scenarios scenario);

    void ClearQueue();

};