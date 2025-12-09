#pragma once

#include "../sdk/globals.hxx"
#include "../sdk/structs.hxx"
#include "../utils/utils.hxx"

namespace features::godmode
{
  using takedmg_fn = void( __fastcall* )( void* entity, float damage );
  inline takedmg_fn original_take_damage = nullptr;

  inline void __fastcall hooked_take_damage( void* entity, float damage )
  {
    const auto game = get_game_instance();

    if ( !game || !game->local_player )
      return original_take_damage( entity, damage );

    if ( entity == game->local_player )
    {
      std::println( "blocking {:.1f} damage", damage );
      return;
    }

    return original_take_damage( entity, damage );
  }

  inline void init()
  {
    MH_HOOK( globals::offsets::take_damage, hooked_take_damage, original_take_damage );
  }
}
