#include <source/features/godmode.hxx>
#include <source/features/menu.hxx>
#include <source/features/update_health_display.hxx>
#include <source/sdk/globals.hxx>
#include <source/sdk/structs.hxx>
#include <source/utils/utils.hxx>

void main_thread( ) {
  utils::open_console( );
  globals::init( );

  features::hooks::update_health_display::init( );
  features::hooks::check_can_die::init( );

  features::menu::init( );

  std::println( "[+] gow trainer loaded" );
  std::println( "[+] base: {:#x}", globals::base.get( ) );
  std::println( "[+] press END to unload" );

  while ( globals::running ) {
    if ( GetAsyncKeyState( VK_END ) & 1 ) {
      globals::running = false;
      break;
    }

    Sleep( 100 );
  }

  std::println( "[+] unloading..." );

  MH_DisableHook( MH_ALL_HOOKS );

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
