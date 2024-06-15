#pragma once

#include "Manager.h"

class ourEventSink : public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                     public RE::BSTEventSink<RE::TESFurnitureEvent>,
                     public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
                     public RE::BSTEventSink<RE::TESSleepStopEvent>,
                     public RE::BSTEventSink<RE::TESWaitStopEvent>
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


    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* event,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent* event,
                                          RE::BSTEventSource<RE::TESFurnitureEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                          RE::BSTEventSource<RE::MenuOpenCloseEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent* event, RE::BSTEventSource<RE::TESSleepStopEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESWaitStopEvent* event, RE::BSTEventSource<RE::TESWaitStopEvent>*);



private:
    Manager* M = nullptr;

    ourEventSink(Manager* manager) : M(manager){};
};