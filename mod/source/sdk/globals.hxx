#pragma once

struct entity;
struct game_instance;

namespace globals {
  inline HMODULE          module_handle  = nullptr;
  inline memory::address  base;
  inline bool             running        = true;
  inline game_instance*   game           = nullptr;
  inline entity*          local_player   = nullptr;

  namespace offsets {
    constexpr std::uintptr_t check_can_die = 0x8D21D0;
    constexpr std::uintptr_t update_health_display = 0x6ECE30;
  } // namespace offsets

  inline void init( ) {
    MH_Initialize( );
    base = memory::address( static_cast< void* >( GetModuleHandleA( nullptr ) ) );
  }
} // namespace globals
