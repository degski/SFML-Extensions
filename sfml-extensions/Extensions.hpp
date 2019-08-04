
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

#ifndef SFML_EXTENSIONS_BUILD
    #ifdef _DEBUG
        #pragma comment ( lib, "sfml-main-d.lib" )
        #pragma comment ( lib, "sfml-system-s-d.lib" )
        #pragma comment ( lib, "sfml-window-s-d.lib" )
        #pragma comment ( lib, "sfml-graphics-s-d.lib" )
        #pragma comment ( lib, "sfml-audio-s-d.lib" )
        #pragma comment ( lib, "sfml-network-s-d.lib" )
        #pragma comment ( lib, "sfml-extensions-s-d.lib" )
        #pragma comment ( lib, "thor-s-d.lib" )
        #pragma comment ( lib, "opengl32.lib" )
        #pragma comment ( lib, "dwmapi.lib" )
        #pragma comment ( lib, "winmm.lib" )
        #pragma comment ( lib, "gdi32.lib" )
        #pragma comment ( lib, "ws2_32.lib" )
        #pragma comment ( lib, "freetyped.lib" )
        #pragma comment ( lib, "turbojpegd.lib" )
        #pragma comment ( lib, "libpng16d.lib" )
        #pragma comment ( lib, "openal32.lib" )
        #pragma comment ( lib, "flac.lib" )
        #pragma comment ( lib, "vorbisenc.lib" )
        #pragma comment ( lib, "vorbisfile.lib" )
        #pragma comment ( lib, "vorbis.lib" )
        #pragma comment ( lib, "ogg.lib" )
        #pragma comment ( lib, "bz2d.lib" )
        #pragma comment ( lib, "zlibd.lib" )
    #else
        #pragma comment ( lib, "sfml-main.lib" )
        #pragma comment ( lib, "sfml-system-s.lib" )
        #pragma comment ( lib, "sfml-window-s.lib" )
        #pragma comment ( lib, "sfml-graphics-s.lib" )
        #pragma comment ( lib, "sfml-audio-s.lib" )
        #pragma comment ( lib, "sfml-network-s.lib" )
        #pragma comment ( lib, "sfml-extensions-s.lib" )
        #pragma comment ( lib, "thor-s.lib" )
        #pragma comment ( lib, "opengl32.lib" )
        #pragma comment ( lib, "dwmapi.lib" )
        #pragma comment ( lib, "winmm.lib" )
        #pragma comment ( lib, "gdi32.lib" )
        #pragma comment ( lib, "ws2_32.lib" )
        #pragma comment ( lib, "freetype.lib" )
        #pragma comment ( lib, "turbojpeg.lib" )
        #pragma comment ( lib, "libpng16.lib" )
        #pragma comment ( lib, "openal32.lib" )
        #pragma comment ( lib, "flac.lib" )
        #pragma comment ( lib, "vorbisenc.lib" )
        #pragma comment ( lib, "vorbisfile.lib" )
        #pragma comment ( lib, "vorbis.lib" )
        #pragma comment ( lib, "ogg.lib" )
        #pragma comment ( lib, "bz2.lib" )
        #pragma comment ( lib, "zlib.lib" )
    #endif
#endif

#include "Extensions/Extensions.hpp"
#include "Extensions/LZ4Stream.hpp"
#include "Extensions/Box.hpp"
#include "Extensions/Animation.hpp"
#include "Extensions/Nanotimer.hpp"
#include "Extensions/Owningptr.hpp"
#include "Extensions/Serialize.hpp"
#include "Extensions/CatmullRom.hpp"
#include "Extensions/Z85.hpp"
#include "Extensions/ParticleSystem.hpp"
