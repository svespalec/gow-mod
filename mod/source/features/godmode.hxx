#pragma once

#include "../sdk/globals.hxx"
#include "../sdk/structs.hxx"

namespace features::god_mode {
  inline float locked_health = 0.f;
  inline bool  enabled       = true;

  inline void tick( ) {
    if ( !enabled )
      return;

    const auto game = get_game_instance( );

    if ( !game || !game->local_player )
      return;

    auto& health = game->local_player->health;

    // update locked health if current is higher (healed/new max)
    if ( health > locked_health )
      locked_health = health;

    // restore if damaged
    if ( health < locked_health ) {
      std::println( "blocked damage: {:.1f} -> {:.1f}", locked_health, health );
      health = locked_health;
    }
  }
} // namespace features::god_mode
