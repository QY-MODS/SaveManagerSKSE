#include "Events.h"

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESContainerChangedEvent*,
                                                    RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESFurnitureEvent*,
                                                    RE::BSTEventSource<RE::TESFurnitureEvent>*) {
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* event,
													RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
    if (!event) return RE::BSEventNotifyControl::kContinue;

    const auto menu_name = std::string(event->menuName.c_str());

    if (const auto temp_setting_open = SaveSettings::Menu::Open[menu_name]; event->opening && temp_setting_open.first) {
        logger::trace("Menu opened: {}", menu_name);
        M->QueueSaveGame(1, temp_setting_open.second);
        return RE::BSEventNotifyControl::kContinue;
    } else if (const auto temp_setting_close = SaveSettings::Menu::Close[menu_name]; !event->opening && temp_setting_close.first) {
        logger::trace("Menu closed: {}", menu_name);
        M->QueueSaveGame(std::max(1, SaveSettings::Menu::After[menu_name]), temp_setting_close.second);
		return RE::BSEventNotifyControl::kContinue;
	}

	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESSleepStopEvent* event,
                                                    RE::BSTEventSource<RE::TESSleepStopEvent>*) {
    if (!event) return RE::BSEventNotifyControl::kContinue;

    const auto temp_time = std::max(1, SaveSettings::SleepWait::sleep_time);

    M->QueueSaveGame(temp_time,SaveSettings::Scenarios::SleepWaitStop);

    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESWaitStopEvent* event,
                                                    RE::BSTEventSource<RE::TESWaitStopEvent>*) {
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (!SaveSettings::SleepWait::wait) return RE::BSEventNotifyControl::kContinue;

    const auto temp_time = std::max(1, SaveSettings::SleepWait::wait_time);

    M->QueueSaveGame(temp_time,SaveSettings::Scenarios::SleepWaitStop);

    return RE::BSEventNotifyControl::kContinue;
}
