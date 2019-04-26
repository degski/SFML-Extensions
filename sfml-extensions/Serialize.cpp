
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <cassert>
#include <ciso646>


#include "Extensions/Serialize.hpp"


namespace sf {

static Int32 number_of_processors_impl ( ) noexcept {

    char *value;
    std::size_t len;

    _dupenv_s ( &value, &len, "NUMBER_OF_PROCESSORS" );

    return std::atoi ( value );
}

Int32 numberOfProcessors ( ) noexcept {

    static const Int32 nop = number_of_processors_impl ( );

    return nop;
}


#ifdef _DEBUG
static bool created_app_data_name = false;
#endif

static std::string set_app_data_path_impl ( std::string && name_ ) noexcept {

    // No error if directory exists...

    char *value;
    std::size_t len;

    _dupenv_s ( &value, &len, "USERPROFILE" );

    const std::string v ( value + std::string ( "\\AppData\\Roaming\\" + name_ ) );

    fs::create_directory ( v );
#ifdef _DEBUG
    created_app_data_name = true;
#endif
    return v;
}

Path setAppDataPath ( std::string && name_ ) noexcept {

    // Call only once...

    static const Path value ( set_app_data_path_impl ( std::move ( name_ ) ) );

    return value;
}

Path getAppDataPath ( ) noexcept {

    // Call only after calling Path appDataPath ( std::string && name_ ) noexcept.

#ifdef _DEBUG
    assert ( created_app_data_name );
#endif
    return setAppDataPath ( std::string ( ) );
}


static Path get_app_exe_path_impl ( ) noexcept {

    TCHAR exename [ 1024 ];

    GetModuleFileName ( nullptr, exename, 1024 );

    return Path ( exename ).parent_path ( );
}


Path getAppExePath ( ) noexcept {

    static const Path value ( get_app_exe_path_impl ( ) );

    return value;
}

}  // namespace sf
