#pragma once

#include <source/sdk/globals.hxx>

template < typename Fn >
inline bool hook( std::uintptr_t offset, void* detour, Fn& original ) {
  auto target = globals::addr( offset ).as< void >( );
  return MH_CreateHook( target, detour, reinterpret_cast< void** >( &original ) ) == MH_OK && MH_EnableHook( target ) == MH_OK;
}

template < typename Fn >
inline bool hook( memory::address target, void* detour, Fn& original ) {
  return MH_CreateHook( target.as< void >( ), detour, reinterpret_cast< void** >( &original ) ) == MH_OK &&
         MH_EnableHook( target.as< void >( ) ) == MH_OK;
}

template < typename Fn >
inline bool hook_abs( void* target, void* detour, Fn& original ) {
  return MH_CreateHook( target, detour, reinterpret_cast< void** >( &original ) ) == MH_OK && MH_EnableHook( target ) == MH_OK;
}

#define MH_HOOK( offset, detour, original )     hook( offset, reinterpret_cast< void* >( detour ), original )
#define MH_HOOK_ABS( target, detour, original ) hook_abs( target, reinterpret_cast< void* >( detour ), original )

namespace utils {
  inline void open_console( ) {
    AllocConsole( );

    FILE* f { };
    freopen_s( &f, "CONOUT$", "w", stdout );
    freopen_s( &f, "CONIN$", "r", stdin );
  }

  inline void close_console( ) {
    fclose( stdout );
    FreeConsole( );
  }
} // namespace utils
