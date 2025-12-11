#pragma once

#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

namespace features::hooks::update_health_display {
  // unsigned __int64 __fastcall goPlayer_updateHealthDisplay(__int64 a1)
  using update_health_display_fn = std::uint64_t( __fastcall* )( game_instance* instance );

  static update_health_display_fn original_update_health_display = nullptr;

  // v1 = *(_QWORD *)(this + 8);
  static std::uint64_t __fastcall update_health_display_hook( game_instance* instance ) {
    globals::local_player = instance->local_player;

    return original_update_health_display( instance );
  }

  void init( ) {
    MH_HOOK( globals::offsets::update_health_display, update_health_display_hook, original_update_health_display );
  }
} // namespace features