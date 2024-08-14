#pragma once
#include "Settings.h"

namespace Hooks {

    const uint8_t n_hooks = 1;
    const size_t trampoline_size = n_hooks * 14;

    void Install();

    template <typename MenuType>
    class MenuHook : public MenuType {
        using ProcessMessage_t = decltype(&MenuType::ProcessMessage);
        static inline REL::Relocation<ProcessMessage_t> _ProcessMessage;
        RE::UI_MESSAGE_RESULTS ProcessMessage_Hook(RE::UIMessage& a_message);
    public:
        static void InstallHook(REL::VariantID varID);
    };

    struct SaveDebugNotifHook {
        static void thunk(const char* a1, uint64_t a2, char a3);
        static inline REL::Relocation<decltype(thunk)> func;
		static void InstallHook();
    };

    //struct MenuControlsHook {
    //public:
    //    static void InstallHook() {
    //        REL::Relocation<std::uintptr_t> MenuControlsVtbl{RE::VTABLE_MenuControls[0]};
    //        _ProcessEvent = MenuControlsVtbl.write_vfunc(0x1, ProcessEvent);
    //    }

    //private:
    //    static RE::BSEventNotifyControl ProcessEvent(RE::MenuControls* a_menuControls, RE::InputEvent* const* a_event,
    //                                                 RE::BSTEventSource<RE::InputEvent*>* a_eventSource);

    //    static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
    //};

};