#include "features/godmode.hxx"
#include "sdk/globals.hxx"
#include "sdk/structs.hxx"
#include "utils/utils.hxx"

void main_thread( ) {
  open_console( );

  globals::init( );

  features::godmode::init( );

  std::println( "[+] gow trainer loaded" );
  std::println( "[+] base: {:p}", reinterpret_cast< void * >( globals::base ) );
  std::println( "[+] press END to unload\n" );

  while ( globals::running ) {
    if ( GetAsyncKeyState( VK_END ) & 1 ) {
      globals::running = false;
      break;
    }

    const auto game = get_game_instance( );

    if ( game && game->local_player )
      std::println( "health: {:.1f}", game->local_player->health );
    else
      std::println( "waiting for player..." );

    Sleep( 1000 );
  }

  std::println( "[+] unloading..." );

  MH_DisableHook( MH_ALL_HOOKS );
  MH_Uninitialize( );

  Sleep( 500 );

  close_console( );
  FreeLibraryAndExitThread( globals::module_handle, 0 );
}

BOOL APIENTRY DllMain( HMODULE handle, DWORD reason, LPVOID reserved ) {
  if ( reason == DLL_PROCESS_ATTACH ) {
    globals::module_handle = handle;

    DisableThreadLibraryCalls( handle );
    CreateThread( nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( main_thread ), nullptr, 0, nullptr );
  }

  return TRUE;
}
