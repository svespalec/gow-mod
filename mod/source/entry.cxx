#include <source/features/godmode.hxx>
#include <source/features/menu.hxx>
#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

void main_thread( ) {
  utils::open_console( );
  globals::init( );

  features::menu::init( );

  std::println( "[+] gow trainer loaded" );
  std::println( "[+] base: {:p}", reinterpret_cast< void* >( globals::base ) );
  std::println( "[+] press END to unload\n" );

  while ( globals::running ) {
    if ( GetAsyncKeyState( VK_END ) & 1 ) {
      globals::running = false;
      break;
    }

    features::god_mode::tick( );

    const auto game = get_game_instance( );

    if ( game && game->local_player )
      std::println( "health: {:.1f} @ {:p}", game->local_player->health, reinterpret_cast< void* >( &game->local_player->health ) );
    else
      std::println( "waiting for player..." );

    Sleep( 100 ); // faster tick for health lock
  }

  std::println( "[+] unloading..." );

  features::menu::shutdown( );

  Sleep( 500 );

  utils::close_console( );

  FreeLibraryAndExitThread( globals::module_handle, 0 );
}

BOOL APIENTRY DllMain( HMODULE handle, DWORD reason, [[maybe_unused]] LPVOID reserved ) {
  if ( reason == DLL_PROCESS_ATTACH ) {
    globals::module_handle = handle;

    DisableThreadLibraryCalls( handle );
    CreateThread( nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( main_thread ), nullptr, 0, nullptr );
  }

  return TRUE;
}
