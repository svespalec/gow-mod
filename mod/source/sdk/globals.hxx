#pragma once

struct entity;

namespace globals {
  inline HMODULE   module_handle = nullptr;
  inline uintptr_t base          = 0;
  inline bool      running       = true;
  inline entity*   local_player  = nullptr;

  namespace offsets {
    // char __fastcall goPlayer_checkCanDie(_QWORD *a1, _BYTE *a2)
    constexpr uintptr_t check_can_die = 0x8D21D0; // inline hook and ret 0 or xor eax,eax; ret patch
    // unsigned __int64 __fastcall goPlayer_updateHealthDisplay(__int64 a1)
    constexpr uintptr_t update_health_display = 0x6ECE30; 
  } // namespace offsets

  inline void init( ) {
    MH_Initialize( );
    base = reinterpret_cast< uintptr_t >( GetModuleHandleA( nullptr ) );
  }

  inline void* get_addr( uintptr_t offset ) {
    return reinterpret_cast< void* >( base + offset );
  }
} // namespace globals
