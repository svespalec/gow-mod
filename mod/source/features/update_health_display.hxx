#pragma once

#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

namespace features::hooks::update_health_display {
  inline std::uint64_t __fastcall detour( game_instance* instance );
  inline decltype( &detour ) original = nullptr;

  inline std::uint64_t __fastcall detour( game_instance* instance ) {
    globals::local_player = instance->local_player;

    return original( instance );
  }

  inline void init( ) {
    MH_HOOK( globals::offsets::update_health_display, detour, original );
  }
} // namespace features::hooks::update_health_display