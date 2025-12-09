#pragma once

#include "globals.hxx"

// notes:
// make sure to use VEH debugger in CE or game will crash due to Denuvo

// patterns:
// SetHealth (?): F3 0F 11 73 ?? 74

struct entity {
  char     pad_0x00[ 0x10 ];
  uint32_t flags;             // +0x10
  char     pad_0x14[ 0x374 ]; // 0x388 - 0x14 = 0x374
  float    health;            // +0x388
};

struct game_instance {
  void*   unk_0x00;     // +0x00
  entity* local_player; // +0x08
};

inline game_instance* get_game_instance( ) {
  return *reinterpret_cast< game_instance** >( globals::base + globals::offsets::game_instance );
}
