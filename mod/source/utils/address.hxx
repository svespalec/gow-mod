#pragma once

#include <source/utils/memory.hxx>

namespace memory {
  class address {
    std::uintptr_t m_addr = 0;

  public:
    address( ) = default;

    address( std::uintptr_t addr ) : m_addr( addr ) { }

    address( void* ptr ) : m_addr( reinterpret_cast< std::uintptr_t >( ptr ) ) { }

    static address from_pattern( std::string_view pattern, HMODULE module = nullptr ) {
      return address( find_pattern( module, pattern ) );
    }

    static address from_rva( std::uintptr_t rva, HMODULE module = nullptr ) {
      const auto base = reinterpret_cast< std::uintptr_t >( module ? module : GetModuleHandle( nullptr ) );
      return address( base + rva );
    }

    address offset( std::ptrdiff_t off ) const {
      return address( m_addr + off );
    }

    address deref( ) const {
      if ( !m_addr )
        return address( );
      return address( *reinterpret_cast< std::uintptr_t* >( m_addr ) );
    }

    address rel( ) const {
      if ( !m_addr )
        return address( );
      const auto rip_offset = *reinterpret_cast< std::int32_t* >( m_addr );
      return address( m_addr + sizeof( std::int32_t ) + rip_offset );
    }

    template < typename... Offsets >
    address chain( Offsets... offsets ) const {
      address result = *this;
      ( ( result = result.deref( ).offset( offsets ) ), ... );
      return result;
    }

    address rva( HMODULE module = nullptr ) const {
      const auto base = reinterpret_cast< std::uintptr_t >( module ? module : GetModuleHandle( nullptr ) );
      return address( m_addr - base );
    }

    explicit operator bool ( ) const {
      return m_addr != 0;
    }

    std::uintptr_t get( ) const {
      return m_addr;
    }

    template < typename T >
    T* as( ) const {
      return reinterpret_cast< T* >( m_addr );
    }

    template < typename T >
    T read( ) const {
      return *as< T >( );
    }

    template < typename T >
    void write( T value ) const {
      *as< T >( ) = value;
    }

    bool patch( std::string_view bytes ) const {
      if ( !m_addr )
        return false;

      std::vector< std::uint8_t > patch_bytes;

      for ( auto it = bytes.begin( ); it < bytes.end( ); ) {
        if ( *it == ' ' ) {
          ++it;
          continue;
        }

        char hex[ 3 ] = { *it++, ( it < bytes.end( ) && *it != ' ' ) ? *it++ : '\0', '\0' };
        patch_bytes.push_back( static_cast< std::uint8_t >( std::strtoul( hex, nullptr, 16 ) ) );
      }

      DWORD old_protect;
      if ( !VirtualProtect( as< void >( ), patch_bytes.size( ), PAGE_EXECUTE_READWRITE, &old_protect ) )
        return false;

      std::memcpy( as< void >( ), patch_bytes.data( ), patch_bytes.size( ) );

      VirtualProtect( as< void >( ), patch_bytes.size( ), old_protect, &old_protect );
      return true;
    }
  };
} // namespace memory
