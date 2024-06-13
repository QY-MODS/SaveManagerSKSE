#include "Settings.h"

std::map<std::string, std::pair<bool, SaveSettings::Scenarios>> SaveSettings::Menu::Open {
    {std::string(RE::ContainerMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenContainerMenu}},
    {std::string(RE::BarterMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenBarterMenu}},
    {std::string(RE::CraftingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenCraftingMenu}},
    {std::string(RE::DialogueMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenDialogueMenu}},
    {std::string(RE::FavoritesMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenFavoritesMenu}},
    {std::string(RE::GiftMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenGiftMenu}},
    {std::string(RE::InventoryMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenInventoryMenu}},
    {std::string(RE::JournalMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenJournalMenu}},
    {std::string(RE::LevelUpMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenLevelUpMenu}},
    {std::string(RE::LockpickingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenLockpickingMenu}},
    {std::string(RE::MagicMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenMagicMenu}},
    {std::string(RE::MapMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenMapMenu}},
    {std::string(RE::MessageBoxMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenMessageBoxMenu}},
    {std::string(RE::SleepWaitMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenSleepWaitMenu}},
    {std::string(RE::StatsMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenStatsMenu}},
    {std::string(RE::TrainingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenTrainingMenu}},
    {std::string(RE::TutorialMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenTutorialMenu}},
    {std::string(RE::TweenMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenTweenMenu}},
    {std::string(RE::Console::MENU_NAME), {false, SaveSettings::Scenarios::MenuOpenConsole}},
};



std::map<std::string, std::pair<bool,SaveSettings::Scenarios>> SaveSettings::Menu::Close{
    {std::string(RE::ContainerMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseContainerMenu}},
    {std::string(RE::BarterMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseBarterMenu}},
    {std::string(RE::CraftingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseCraftingMenu}},
    {std::string(RE::DialogueMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseDialogueMenu}},
    {std::string(RE::FavoritesMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseFavoritesMenu}},
    {std::string(RE::GiftMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseGiftMenu}},
    {std::string(RE::InventoryMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseInventoryMenu}},
    {std::string(RE::JournalMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseJournalMenu}},
    {std::string(RE::LevelUpMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseLevelUpMenu}},
    {std::string(RE::LockpickingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseLockpickingMenu}},
    {std::string(RE::MagicMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseMagicMenu}},
    {std::string(RE::MapMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseMapMenu}},
    {std::string(RE::MessageBoxMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseMessageBoxMenu}},
    {std::string(RE::SleepWaitMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseSleepWaitMenu}},
    {std::string(RE::StatsMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseStatsMenu}},
    {std::string(RE::TrainingMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseTrainingMenu}},
    {std::string(RE::TutorialMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseTutorialMenu}},
    {std::string(RE::TweenMenu::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseTweenMenu}},
    {std::string(RE::Console::MENU_NAME), {false, SaveSettings::Scenarios::MenuCloseConsole}},
};

//void GameLock::SetState(GameLock::State currentState) {
//    if (GameLock::lastState == currentState || !SaveSettings::freeze_game) return;
//    GameLock::lastState = currentState;
//    const auto main = RE::Main::GetSingleton();
//    if (currentState == GameLock::State::Locked) main->freezeTime = true;
//    else main->freezeTime = false;
//};

std::map<std::string, int> SaveSettings::Menu::After = {
    {std::string(RE::ContainerMenu::MENU_NAME), 0},
    {std::string(RE::BarterMenu::MENU_NAME), 0},
    {std::string(RE::CraftingMenu::MENU_NAME), 0},
    {std::string(RE::DialogueMenu::MENU_NAME), 0},
    {std::string(RE::FavoritesMenu::MENU_NAME), 0},
    {std::string(RE::GiftMenu::MENU_NAME), 0},
    {std::string(RE::InventoryMenu::MENU_NAME), 0},
    {std::string(RE::JournalMenu::MENU_NAME), 0},
    {std::string(RE::LevelUpMenu::MENU_NAME), 0},
    {std::string(RE::LockpickingMenu::MENU_NAME), 0},
    {std::string(RE::MagicMenu::MENU_NAME), 0},
    {std::string(RE::MapMenu::MENU_NAME), 0},
    {std::string(RE::MessageBoxMenu::MENU_NAME), 0},
    {std::string(RE::SleepWaitMenu::MENU_NAME), 0},
    {std::string(RE::StatsMenu::MENU_NAME), 0},
    {std::string(RE::TrainingMenu::MENU_NAME), 0},
    {std::string(RE::TutorialMenu::MENU_NAME), 0},
    {std::string(RE::TweenMenu::MENU_NAME), 0},
    {std::string(RE::Console::MENU_NAME), 0},
};