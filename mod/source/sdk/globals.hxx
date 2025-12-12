#pragma once

struct entity;

namespace globals {
  inline HMODULE          module_handle = nullptr;
  inline memory::address  base;
  inline bool             running       = true;
  inline entity*          local_player  = nullptr;

  namespace offsets {
    // char __fastcall goPlayer_checkCanDie(_QWORD *a1, _BYTE *a2)
    constexpr std::uintptr_t check_can_die = 0x8D21D0;
    // unsigned __int64 __fastcall goPlayer_updateHealthDisplay(__int64 a1)
    constexpr std::uintptr_t update_health_display = 0x6ECE30;
  } // namespace offsets

  inline void init( ) {
    MH_Initialize( );
    base = memory::address( GetModuleHandleA( nullptr ) );
  }

  inline memory::address addr( std::uintptr_t offset ) {
    return base.offset( offset );
  }
} // namespace globals
