#pragma once

#include "Utils.h"


//namespace GameLock {
//    enum State { None, Locked, Unlocked };
//    inline State lastState = State::None;
//    void SetState(State currentState);
//};

namespace PluginSettings {
	inline bool failed = false;
};

namespace LogSettings {
	inline bool log_trace = true;
	inline bool log_info = true;
	inline bool log_warning = true;
	inline bool log_error = true;
};

namespace SaveSettings {

	inline bool block = false;
    inline int queue_delay = 10;  // in seconds
    inline int ticker_interval = 1; // in seconds

	inline bool notifications = true;
	inline int timer_minutes = 0;
	inline int timer_seconds = 0;
	inline bool timer_running = false;
	inline bool close_game_warning = false;
	inline bool close_game = false;
	inline bool save_on_quit = false;

	//inline bool freeze_game = true;

	enum Scenarios {
		MenuOpenContainerMenu,
		MenuOpenBarterMenu,
		MenuOpenCraftingMenu,
		MenuOpenDialogueMenu,
		MenuOpenFavoritesMenu,
		MenuOpenGiftMenu,
		MenuOpenInventoryMenu,
		MenuOpenJournalMenu,
		MenuOpenLevelUpMenu,
		MenuOpenLockpickingMenu,
		MenuOpenMagicMenu,
		MenuOpenMapMenu,
		MenuOpenMessageBoxMenu,
		MenuOpenSleepWaitMenu,
		MenuOpenStatsMenu,
		MenuOpenTrainingMenu,
		MenuOpenTutorialMenu,
		MenuOpenTweenMenu,
		MenuOpenConsole,

		MenuCloseContainerMenu,
        MenuCloseBarterMenu,
        MenuCloseCraftingMenu,
        MenuCloseDialogueMenu,
        MenuCloseFavoritesMenu,
        MenuCloseGiftMenu,
		MenuCloseInventoryMenu,
        MenuCloseJournalMenu,
        MenuCloseLevelUpMenu,
        MenuCloseLockpickingMenu,
        MenuCloseMagicMenu,
        MenuCloseMapMenu,
        MenuCloseMessageBoxMenu,
        MenuCloseSleepWaitMenu,
        MenuCloseStatsMenu,
        MenuCloseTrainingMenu,
        MenuCloseTutorialMenu,
        MenuCloseTweenMenu,
        MenuCloseConsole,

		Timer,

		SleepWaitStop,

	};

	namespace Menu {

        extern std::map<std::string, std::pair<bool, Scenarios>> Open;

		extern std::map<std::string, std::pair<bool, Scenarios>> Close;

		extern std::map<std::string, int> After; // after menu is closed, wait x seconds before saving
	};

	namespace SleepWait {
		inline bool sleep = false;
		inline bool wait = false;
		inline int sleep_time = 0; // save after in seconds
		inline int wait_time = 0; // save after in seconds
	};

	static std::map<Scenarios, std::string> scenario_names = {
        {MenuOpenContainerMenu,"MenuOpenContainerMenu"},
		{MenuOpenBarterMenu,"MenuOpenBarterMenu"},
		{MenuOpenCraftingMenu,"MenuOpenCraftingMenu"},
		{MenuOpenDialogueMenu,"MenuOpenDialogueMenu"},
		{MenuOpenFavoritesMenu,"MenuOpenFavoritesMenu"},
		{MenuOpenGiftMenu,"MenuOpenGiftMenu"},
		{MenuOpenInventoryMenu,"MenuOpenInventoryMenu"},
		{MenuOpenJournalMenu,"MenuOpenJournalMenu"},
		{MenuOpenLevelUpMenu,"MenuOpenLevelUpMenu"},
		{MenuOpenLockpickingMenu,"MenuOpenLockpickingMenu"},
		{MenuOpenMagicMenu,"MenuOpenMagicMenu"},
		{MenuOpenMapMenu,"MenuOpenMapMenu"},
		{MenuOpenMessageBoxMenu,"MenuOpenMessageBoxMenu"},
		{MenuOpenSleepWaitMenu,"MenuOpenSleepWaitMenu"},
		{MenuOpenStatsMenu,"MenuOpenStatsMenu"},
		{MenuOpenTrainingMenu,"MenuOpenTrainingMenu"},
		{MenuOpenTutorialMenu,"MenuOpenTutorialMenu"},
		{MenuOpenTweenMenu,"MenuOpenTweenMenu"},
		{MenuOpenConsole,"MenuOpenConsole"},

		{MenuCloseContainerMenu, "MenuCloseContainerMenu"},
        {MenuCloseBarterMenu, "MenuCloseBarterMenu"},
        {MenuCloseCraftingMenu, "MenuCloseCraftingMenu"},
        {MenuCloseDialogueMenu, "MenuCloseDialogueMenu"},
        {MenuCloseFavoritesMenu, "MenuCloseFavoritesMenu"},
        {MenuCloseGiftMenu, "MenuCloseGiftMenu"},
        {MenuCloseInventoryMenu, "MenuCloseInventoryMenu"},
        {MenuCloseJournalMenu, "MenuCloseJournalMenu"},
        {MenuCloseLevelUpMenu, "MenuCloseLevelUpMenu"},
        {MenuCloseLockpickingMenu, "MenuCloseLockpickingMenu"},
        {MenuCloseMagicMenu, "MenuCloseMagicMenu"},
        {MenuCloseMapMenu, "MenuCloseMapMenu"},
        {MenuCloseMessageBoxMenu, "MenuCloseMessageBoxMenu"},
        {MenuCloseSleepWaitMenu, "MenuCloseSleepWaitMenu"},
        {MenuCloseStatsMenu, "MenuCloseStatsMenu"},
        {MenuCloseTrainingMenu, "MenuCloseTrainingMenu"},
        {MenuCloseTutorialMenu, "MenuCloseTutorialMenu"},
        {MenuCloseTweenMenu, "MenuCloseTweenMenu"},
        {MenuCloseConsole, "MenuCloseConsole"},

		{Timer,"Timer"},

		{SleepWaitStop, "SleepWaitStop"}
    };
};