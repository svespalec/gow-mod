#pragma once

#include <source/sdk/globals.hxx>

// notes:
// make sure to use VEH debugger in CE or game will crash due to Denuvo

struct entity {
  char     pad_0x00[ 0x10 ];
  uint32_t flags; // +0x10
  char     pad_0x14[ 0x374 ];
  float    health; // +0x388
  char     pad_0x38C[ 0x658 ];
  float    speed; // +0xBE4 (3044)
  char     pad_0xBE8[ 0x0C ];
};

struct game_instance {
  void*   vtable;
  entity* local_player; // +0x08 (because every function accesses it like v1 = *(_QWORD *)(this + 8);)
};

// we dont have to use this because we have update health display hook 
inline game_instance* get_game_instance( ) {
  return *reinterpret_cast< game_instance** >( globals::base + 0x22E7D50 );
}
