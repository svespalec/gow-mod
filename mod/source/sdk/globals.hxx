#pragma once

#include <MinHook.h>
#include <cstdint>
#include <print>
#include <windows.h>

namespace globals {
  inline HMODULE   module_handle = nullptr;
  inline uintptr_t base          = 0;
  inline bool      running       = true;

  namespace offsets {
    constexpr uintptr_t game_instance = 0x22E7D50;
    constexpr uintptr_t take_damage   = 0x7A0C60;
  } // namespace offsets

  inline void init( ) {
    MH_Initialize( );

    base = reinterpret_cast< uintptr_t >( GetModuleHandle( nullptr ) );
  }

  inline void *get_addr( uintptr_t offset ) {
    return reinterpret_cast< void * >( base + offset );
  }
} // namespace globals
