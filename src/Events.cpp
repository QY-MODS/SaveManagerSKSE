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
    if (event->opening) return RE::BSEventNotifyControl::kContinue;

    const auto menu_name = std::string(event->menuName.c_str());

    if (!SaveSettings::Menu::Close.contains(menu_name)) return RE::BSEventNotifyControl::kContinue;

    if (const auto temp_setting_close = SaveSettings::Menu::Close[menu_name]; temp_setting_close.first) {
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
