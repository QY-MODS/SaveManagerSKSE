#pragma once

#include "Utils.h"


//namespace GameLock {
//    enum State { None, Locked, Unlocked };
//    inline State lastState = State::None;
//    void SetState(State currentState);
//};

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

	//inline bool freeze_game = true;

	enum Scenarios {
		MenuOpenContainerMenu,
		MenuOpenBarterMenu,
		MenuOpenCraftingMen,
		MenuOpenDialogueMenu,
		MenuOpenFavoritesMenu,
		MenuOpenGiftMenu,
		MenuOpenInventoryMenu,
		MenuOpenJournalMenu,
		MenuOpenLevelUpMenu,
		MenuOpenLockpickingMenu,
		MenuOpenMagicMenu,
		MenuOpenMapMenu,
		MenuOpenMessageBoxMen,
		MenuOpenSleepWaitMenu,
		MenuOpenStatsMenu,
		MenuOpenTrainingMenu,
		MenuOpenTutorialMenu,
		MenuOpenTweenMenu,
		MenuOpenConsole,

		MenuCloseContainerMenu,
        MenuCloseBarterMenu,
        MenuCloseCraftingMen,
        MenuCloseDialogueMenu,
        MenuCloseFavoritesMenu,
        MenuCloseGiftMenu,
		MenuCloseInventoryMenu,
        MenuCloseJournalMenu,
        MenuCloseLevelUpMenu,
        MenuCloseLockpickingMenu,
        MenuCloseMagicMenu,
        MenuCloseMapMenu,
        MenuCloseMessageBoxMen,
        MenuCloseSleepWaitMenu,
        MenuCloseStatsMenu,
        MenuCloseTrainingMenu,
        MenuCloseTutorialMenu,
        MenuCloseTweenMenu,
        MenuCloseConsole,
	};

	namespace Menu {

        extern std::map<std::string, std::pair<bool, Scenarios>> Open;

		extern std::map<std::string, std::pair<bool, Scenarios>> Close;
	};


	static std::map<std::string, Scenarios> scenario_names = {
        {"MenuOpenContainerMenu", MenuOpenContainerMenu},
		{"MenuOpenBarterMenu",MenuOpenBarterMenu},
		{"MenuOpenCraftingMen",MenuOpenCraftingMen},
		{"MenuOpenDialogueMenu",MenuOpenDialogueMenu},
		{"MenuOpenFavoritesMenu",MenuOpenFavoritesMenu},
		{"MenuOpenGiftMenu",MenuOpenGiftMenu},
		{"MenuOpenJournalMenu",MenuOpenJournalMenu},
		{"MenuOpenLevelUpMenu",MenuOpenLevelUpMenu},
		{"MenuOpenLockpickingMenu",MenuOpenLockpickingMenu},
		{"MenuOpenMagicMenu",MenuOpenMagicMenu},
		{"MenuOpenMapMenu",MenuOpenMapMenu},
		{"MenuOpenMessageBoxMen",MenuOpenMessageBoxMen},
		{"MenuOpenSleepWaitMenu",MenuOpenSleepWaitMenu},
		{"MenuOpenStatsMenu",MenuOpenStatsMenu},
		{"MenuOpenTrainingMenu",MenuOpenTrainingMenu},
		{"MenuOpenTutorialMenu",MenuOpenTutorialMenu},
		{"MenuOpenTweenMenu",MenuOpenTweenMenu},
		{"MenuOpenConsole",MenuOpenConsole},

		{"MenuCloseContainerMenu",MenuCloseContainerMenu},
        {"MenuCloseBarterMenu",MenuCloseBarterMenu},
        {"MenuCloseCraftingMen",MenuCloseCraftingMen},
        {"MenuCloseDialogueMenu",MenuCloseDialogueMenu},
        {"MenuCloseFavoritesMenu",MenuCloseFavoritesMenu},
        {"MenuCloseGiftMenu",MenuCloseGiftMenu},
        {"MenuCloseJournalMenu",MenuCloseJournalMenu},
        {"MenuCloseLevelUpMenu",MenuCloseLevelUpMenu},
        {"MenuCloseLockpickingMenu",MenuCloseLockpickingMenu},
        {"MenuCloseMagicMenu",MenuCloseMagicMenu},
        {"MenuCloseMapMenu",MenuCloseMapMenu},
        {"MenuCloseMessageBoxMen",MenuCloseMessageBoxMen},
        {"MenuCloseSleepWaitMenu",MenuCloseSleepWaitMenu},
        {"MenuCloseStatsMenu",MenuCloseStatsMenu},
        {"MenuCloseTrainingMenu",MenuCloseTrainingMenu},
        {"MenuCloseTutorialMenu",MenuCloseTutorialMenu},
        {"MenuCloseTweenMenu",MenuCloseTweenMenu},
        {"MenuCloseConsole",MenuCloseConsole}
    };
};