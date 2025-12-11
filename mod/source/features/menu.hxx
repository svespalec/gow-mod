#pragma once

#include <source/features/godmode.hxx>
#include <source/sdk/globals.hxx>
#include <source/utils/utils.hxx>

#include <d3d11.h>
#include <dxgi.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace features::menu {
  using present_fn = HRESULT( __stdcall* )( IDXGISwapChain*, UINT, UINT );
  using wndproc_fn = LRESULT( __stdcall* )( HWND, UINT, WPARAM, LPARAM );

  inline present_fn              original_present = nullptr;
  inline wndproc_fn              original_wndproc = nullptr;
  inline ID3D11Device*           device           = nullptr;
  inline ID3D11DeviceContext*    context          = nullptr;
  inline ID3D11RenderTargetView* render_target    = nullptr;
  inline HWND                    hwnd             = nullptr;

  inline bool initialized = false;
  inline bool show_menu   = true;

  inline void create_render_target( IDXGISwapChain* swapchain ) {
    ID3D11Texture2D* back_buffer = nullptr;
    swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &back_buffer ) );

    if ( back_buffer ) {
      device->CreateRenderTargetView( back_buffer, nullptr, &render_target );
      back_buffer->Release( );
    }
  }

  inline void cleanup_render_target( ) {
    if ( render_target ) {
      render_target->Release( );
      render_target = nullptr;
    }
  }

  inline LRESULT __stdcall hooked_wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    // let imgui process the input first (mouse clicks, keyboard, etc)
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
      return true; // imgui handled it, don't pass to game

    // toggle menu with INSERT
    if ( msg == WM_KEYDOWN && wParam == VK_HOME )
      show_menu = !show_menu;

    // pass to original handler (so game receives input)
    return CallWindowProcA( reinterpret_cast< WNDPROC >( original_wndproc ), hWnd, msg, wParam, lParam );
  }

  inline HRESULT __stdcall hooked_present( IDXGISwapChain* swapchain, UINT sync_interval, UINT flags ) {
    if ( !initialized ) {
      if ( SUCCEEDED( swapchain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &device ) ) ) ) {
        device->GetImmediateContext( &context );

        create_render_target( swapchain );

        // get hwnd from swapchain
        DXGI_SWAP_CHAIN_DESC desc;
        swapchain->GetDesc( &desc );

        hwnd = desc.OutputWindow;

        // hook wndproc for input
        original_wndproc = reinterpret_cast< wndproc_fn >( SetWindowLongPtrA( hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( hooked_wndproc ) ) );

        // initialize imgui
        ImGui::CreateContext( );
        ImGui::StyleColorsDark( );

        auto& io        = ImGui::GetIO( );
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplWin32_Init( hwnd );
        ImGui_ImplDX11_Init( device, context );

        initialized = true;

        std::println( "[+] d3d11 + imgui initialized" );
      }
    }

    // render imgui
    ImGui_ImplDX11_NewFrame( );
    ImGui_ImplWin32_NewFrame( );

    ImGui::NewFrame( );

    if ( show_menu ) {
      ImGui::Begin( "gow trainer", &show_menu );
      ImGui::Checkbox( "god mode", &features::hooks::check_can_die::enabled );
      ImGui::End( );
    }

    ImGui::Render( );
    context->OMSetRenderTargets( 1, &render_target, nullptr );

    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

    return original_present( swapchain, sync_interval, flags );
  }

  inline void* get_present_addr( ) {
    // create dummy swapchain to get vtable
    DXGI_SWAP_CHAIN_DESC sd { };

    sd.BufferCount       = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow      = GetForegroundWindow( );
    sd.SampleDesc.Count  = 1;
    sd.Windowed          = TRUE;
    sd.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;

    IDXGISwapChain* dummy_swapchain = nullptr;
    ID3D11Device*   dummy_device    = nullptr;

    // clang-format off
    if ( FAILED( D3D11CreateDeviceAndSwapChain( 
      nullptr, D3D_DRIVER_TYPE_HARDWARE, 
      nullptr, 0, nullptr, 0,
      D3D11_SDK_VERSION, 
      &sd, &dummy_swapchain, 
      &dummy_device, nullptr, nullptr ) ) ) {
      return nullptr;
    }
    // clang-format on

    // get present from vtable (index 8)
    auto vtable  = *reinterpret_cast< void*** >( dummy_swapchain );
    auto present = vtable[ 8 ];

    dummy_swapchain->Release( );
    dummy_device->Release( );

    return present;
  }

  inline void init( ) {
    auto present_addr = get_present_addr( );

    if ( !present_addr ) {
      std::println( "[-] failed to get present address" );
      return;
    }

    if ( !MH_HOOK_ABS( present_addr, hooked_present, original_present ) ) {
      std::println( "[-] failed to hook present" );
      return;
    }

    std::println( "[+] present hooked @ {:p}", present_addr );
  }

  inline void shutdown( ) {
    // restore wndproc
    if ( original_wndproc && hwnd )
      SetWindowLongPtrA( hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( original_wndproc ) );

    // cleanup imgui
    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );

    ImGui::DestroyContext( );
    cleanup_render_target( );

    if ( context )
      context->Release( );

    if ( device )
      device->Release( );
  }
} // namespace features::menu
