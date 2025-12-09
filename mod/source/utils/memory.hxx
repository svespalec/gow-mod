#pragma once

namespace memory {
  // inspiration: https://github.com/W1lliam1337/spotify_patcher/blob/master/DigitalSpotify/utils/utils.cpp#L27
  inline std::uint8_t* find_pattern( HMODULE module, std::string_view pattern ) {
    if ( !module )
      module = GetModuleHandle( nullptr );

    const auto dos = reinterpret_cast< PIMAGE_DOS_HEADER >( module );
    const auto nt  = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< std::uint8_t* >( module ) + dos->e_lfanew );

    const auto base = reinterpret_cast< std::uint8_t* >( module );
    const auto size = nt->OptionalHeader.SizeOfImage;

    // parse pattern
    std::vector< int > bytes { };

    for ( auto it = pattern.begin( ); it < pattern.end( ); ) {
      if ( *it == ' ' ) {
        ++it;
        continue;
      }

      if ( *it == '?' ) {
        bytes.push_back( -1 );
        ++it;

        if (it < pattern.end() && *it == '?') {
          ++it;
        }
      } else {
        char hex[ 3 ] = { *it++, ( it < pattern.end( ) && *it != ' ' && *it != '?' ) ? *it++ : '\0', '\0' };
        bytes.push_back( std::strtoul( hex, nullptr, 16 ) );
      }
    }

    // scan
    for ( size_t i = 0; i < size - bytes.size( ); ++i ) {
      bool found = true;

      for ( size_t j = 0; j < bytes.size( ); ++j ) {
        if ( bytes[ j ] != -1 && base[ i + j ] != bytes[ j ] ) {
          found = false;
          break;
        }
      }

      if ( found )
        return &base[ i ];
    }

    return nullptr;
  }
} // namespace memory
