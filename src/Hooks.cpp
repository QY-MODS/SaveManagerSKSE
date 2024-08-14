#include "Hooks.h"

using namespace Hooks;

template <typename MenuType>
void MenuHook<MenuType>::InstallHook(REL::VariantID varID) {
    REL::Relocation<std::uintptr_t> vTable(varID);
    _ProcessMessage = vTable.write_vfunc(0x4, &MenuHook<MenuType>::ProcessMessage_Hook);
}

template <typename MenuType>
RE::UI_MESSAGE_RESULTS MenuHook<MenuType>::ProcessMessage_Hook(RE::UIMessage& a_message) {
    const std::string_view menuName = MenuType::MENU_NAME;
    if (_strcmpi(a_message.menu.c_str(), std::string(menuName).c_str()) == 0) {
        logger::info("menu name {}", a_message.menu.c_str());
        logger::info("type {}", static_cast<int>(a_message.type.get()));

        if (auto _menu = RE::UI::GetSingleton()->GetMenu<MenuType>(menuName)) {
            if (const auto msg_type = static_cast<int>(a_message.type.get());
                msg_type == 1 && SaveSettings::Menu::Open.contains(std::string(menuName)) &&
                SaveSettings::Menu::Open[std::string(menuName)].first) {
                const auto old_flag = _menu->menuFlags.get();
                _menu->menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);
                MainSaveFunction();
                _menu->menuFlags.set(old_flag);
            }
        } else logger::error("MenuHook: {}, menu not found", menuName);
    }
    return _ProcessMessage(this, a_message);
}

void Hooks::Install(){
    MenuHook<RE::ContainerMenu>::InstallHook(RE::VTABLE_ContainerMenu[0]);
    MenuHook<RE::BarterMenu>::InstallHook(RE::VTABLE_BarterMenu[0]);
    MenuHook<RE::CraftingMenu>::InstallHook(RE::VTABLE_CraftingMenu[0]);
    MenuHook<RE::DialogueMenu>::InstallHook(RE::VTABLE_DialogueMenu[0]);
    MenuHook<RE::FavoritesMenu>::InstallHook(RE::VTABLE_FavoritesMenu[0]);
	MenuHook<RE::InventoryMenu>::InstallHook(RE::VTABLE_InventoryMenu[0]);
    MenuHook<RE::LockpickingMenu>::InstallHook(RE::VTABLE_LockpickingMenu[0]);
    MenuHook<RE::MagicMenu>::InstallHook(RE::VTABLE_MagicMenu[0]);
    MenuHook<RE::MapMenu>::InstallHook(RE::VTABLE_MapMenu[0]);

    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(Hooks::trampoline_size);
    SaveDebugNotifHook::InstallHook();
};

void Hooks::SaveDebugNotifHook::thunk(const char* a1, uint64_t a2, char a3) {
    if ((_strcmpi(a1, "Autosaving...") == 0 || _strcmpi(a1, "Saving...") == 0) &&
        SaveSettings::block_autosaving_notif) {
        SaveSettings::block_autosaving_notif = false;
        return;
	}
    func(a1, a2, a3);
};

void Hooks::SaveDebugNotifHook::InstallHook(){
    auto& trampoline = SKSE::GetTrampoline();
    REL::Relocation<std::uintptr_t> originalFunc{RELOCATION_ID(50737, 51632)};
    func = trampoline.write_call<5>(originalFunc.address() + REL::Relocate(0xaf, 0xaf), thunk);
};
