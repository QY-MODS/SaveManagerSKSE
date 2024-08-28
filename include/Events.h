#pragma once

#include "Manager.h"

class ourEventSink : public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                     public RE::BSTEventSink<RE::TESFurnitureEvent>,
                     public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
                     public RE::BSTEventSink<RE::TESSleepStopEvent>,
                     public RE::BSTEventSink<RE::TESWaitStopEvent>,
                     public RE::BSTEventSink<RE::TESCombatEvent>,
                     public RE::BSTEventSink<RE::TESTrackedStatsEvent>
{
public:

    ourEventSink() = default;
    ourEventSink(const ourEventSink&) = delete;
    ourEventSink(ourEventSink&&) = delete;
    ourEventSink& operator=(const ourEventSink&) = delete;
    ourEventSink& operator=(ourEventSink&&) = delete;


    static ourEventSink* GetSingleton(Manager* manager) {
        static ourEventSink singleton(manager);
        return &singleton;
    }


    [[maybe_unused]] RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent*,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*);

    [[maybe_unused]] RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent*,
                                          RE::BSTEventSource<RE::TESFurnitureEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                          RE::BSTEventSource<RE::MenuOpenCloseEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent* event, RE::BSTEventSource<RE::TESSleepStopEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESWaitStopEvent* event, RE::BSTEventSource<RE::TESWaitStopEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent* event, RE::BSTEventSource<RE::TESCombatEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESTrackedStatsEvent* event,
                                          RE::BSTEventSource<RE::TESTrackedStatsEvent>*);

private:
    Manager* M = nullptr;

    std::map<std::string, datetime::time_point> menu_times;

    bool in_combat = false;

    ourEventSink(Manager* manager) : M(manager){ 
        if (SaveSettings::timer_periodic && SaveSettings::timer_running) {
            M->QueueSaveGame(SaveSettings::timer_minutes * 60 + SaveSettings::timer_seconds,
                             SaveSettings::Scenarios::Timer);
		}
    };
};