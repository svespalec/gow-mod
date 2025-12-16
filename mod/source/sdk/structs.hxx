#pragma once

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
  entity* local_player; // +0x08
};
