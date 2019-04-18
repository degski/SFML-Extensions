
// MIT License
//
// Copyright (c) 2019 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include <SFML/System.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

#include <lz4stream.hpp>


#ifndef SFML_EXTENSIONS_BUILD
#ifdef _DEBUG
#pragma comment ( lib, "lz4d.lib" )
#pragma comment ( lib, "lz4stream-s-d.lib" )
#else
#pragma comment ( lib, "lz4.lib" )
#pragma comment ( lib, "lz4stream-s.lib" )
#endif
#endif


namespace fs = std::filesystem;


namespace sf {

// Number of (hyper-)cores...
Int32 numberOfProcessors ( ) noexcept;

using Path = fs::path;
// Call only once, afterwards call Path getAppDataPath ( ) noexcept...
Path setAppDataPath ( std::string && name_ ) noexcept;
// Call only after calling Path setAppDataPath ( std::string && name_ )
//  noexcept at least once. Note though that name is immutable after init...
Path getAppDataPath ( ) noexcept;
// The location of the apps' executable...
Path getAppExePath ( ) noexcept;


template<typename T>
void saveToFileLZ4 ( const T & t_, Path && path_, std::string && file_name_, const bool append_ = false, const int compression_level_ = 4 ) noexcept {
    std::ofstream compressed_ostream ( path_ / ( file_name_ + std::string ( ".lz4cereal" ) ), append_ ? std::ios::binary | std::ios::app | std::ios::out : std::ios::binary | std::ios::out );
    LZ4OutputStream lz4_ostream ( compressed_ostream, compression_level_ );
    {
        cereal::BinaryOutputArchive archive ( lz4_ostream );
        archive ( t_ );
    }
    lz4_ostream.flush ( );
    compressed_ostream.flush ( );
    lz4_ostream.close ( );
    compressed_ostream.close ( );
}

template<typename T>
void loadFromFileLZ4 ( T & t_, Path && path_, std::string && file_name_ ) noexcept {
    std::ifstream compressed_istream ( path_ / ( file_name_ + std::string ( ".lz4cereal" ) ), std::ios::binary );
    LZ4InputStream lz4_istream ( compressed_istream );
    {
        cereal::BinaryInputArchive archive ( lz4_istream );
        archive ( t_ );
    }
    compressed_istream.close ( );
}


template<typename T>
void saveToFileBin ( const T & t_, Path && path_, std::string && file_name_, const bool append_ = false ) noexcept {
    std::ofstream ostream ( path_ / ( file_name_ + std::string ( ".cereal" ) ), append_ ? std::ios::binary | std::ios::app | std::ios::out : std::ios::binary | std::ios::out );
    {
        cereal::BinaryOutputArchive archive ( ostream );
        archive ( t_ );
    }
    ostream.flush ( );
    ostream.close ( );
}

template<typename T>
void loadFromFileBin ( T & t_, Path && path_, std::string && file_name_ ) noexcept {
    std::ifstream istream ( path_ / ( file_name_ + std::string ( ".cereal" ) ), std::ios::binary );
    {
        cereal::BinaryInputArchive archive ( istream );
        archive ( t_ );
    }
    istream.close ( );
}


template<typename T>
void saveToFileXML ( const T & t_, Path && path_, std::string && file_name_, const bool append_ = false ) noexcept {
    std::ofstream ostream ( path_ / ( file_name_ + std::string ( ".xmlcereal" ) ), append_ ? std::ios::app | std::ios::out : std::ios::out );
    {
        cereal::XMLOutputArchive archive ( ostream );
        archive ( t_ );
    }
    ostream.flush ( );
    ostream.close ( );
}

template<typename T>
void loadFromFileXML ( T & t_, Path && path_, std::string && file_name_ ) noexcept {
    std::ifstream istream ( path_ / ( file_name_ + std::string ( ".xmlcereal" ) ) );
    {
        cereal::XMLInputArchive archive ( istream );
        archive ( t_ );
    }
    istream.close ( );
}
}
