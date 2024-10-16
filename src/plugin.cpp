#pragma once

#include "UI.h"
#include "windows.applicationmodel.datatransfer.sharetarget.h"

Manager* manager = nullptr;

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        // Start
        manager = Manager::GetSingleton();
        if (!manager) {
            logger::error("Manager is null.");
            PluginSettings::running = false;
            return;
        }
        auto* eventSink = ourEventSink::GetSingleton(manager);
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESFurnitureEvent>(eventSink);
        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESSleepStopEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESWaitStopEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESCombatEvent>(eventSink);
        eventSourceHolder->AddEventSink<RE::TESTrackedStatsEvent>(eventSink);
        logger::info("Event sinks added.");

        // MCP
        MCP::Register(manager);
        logger::info("MCP registered.");
    }
    else if (message->type == SKSE::MessagingInterface::kPreLoadGame)
    {
        game_is_loading.store(true);
	}
	else if (message->type == SKSE::MessagingInterface::kPostLoadGame)
	{
        game_is_loading.store(false);
	}
}

static void SetupLog() {
    const auto logFilePath = Utilities::GetLogPath();
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
#endif
    logger::info("Name of the plugin is {}.", SKSE::PluginDeclaration::GetSingleton()->GetName());
    logger::info("Version of the plugin is {}.", SKSE::PluginDeclaration::GetSingleton()->GetVersion());
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SetupLog();
    logger::info("Plugin loaded");
    SKSE::Init(skse);
    SaveSettings::LoadJSON();
    InitializeSerialization();
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    Hooks::Install();
    return true;
}