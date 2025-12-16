#pragma once

#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

namespace features::hooks::check_can_die {
  inline bool enabled = true;

  // char __fastcall goPlayer_checkCanDie(_QWORD *a1, _BYTE *a2)
  inline bool __fastcall detour( entity* local_player, void* a2 );
  inline decltype( &detour ) original = nullptr;

  inline bool __fastcall detour( entity* local_player, void* a2 ) {
    // prevent death
    if ( enabled ) {
      return false; 
    }

    return original( local_player, a2 );
  }

  inline void init( ) {
    MH_HOOK( globals::offsets::check_can_die, detour, original );
  }
} // namespace features::hooks::check_can_die
