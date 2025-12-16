#pragma once

#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

namespace features::hooks::update_health_display {
  inline std::uint64_t __fastcall detour( game_instance* instance );
  inline decltype( &detour ) original = nullptr;

  inline std::uint64_t __fastcall detour( game_instance* instance ) {
    auto local = instance->local_player;

    globals::game         = instance;
    globals::local_player = local;

    static bool _ = ( std::println( "[+] local: {:p}", static_cast< void* >( local ) ), true );

    // constantly update our health so we don't have a red screen on 1hp
    if ( features::hooks::check_can_die::enabled ) {
      local->health = 999.f;
    }

    return original( instance );
  }

  inline void init( ) {
    MH_HOOK( globals::offsets::update_health_display, detour, original );
  }
} // namespace features::hooks::update_health_display