
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <Dwmapi.h>
#include <Mmsystem.h>
#include <intrin.h>

#include <cassert>
#include <ctime>

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#ifndef SFML_EXTENSIONS_BUILD
#pragma comment ( lib, "Dwmapi.lib" )
#pragma comment ( lib, "Winmm.lib" )
#ifdef NDEBUG
#pragma comment ( lib, "sfml-extensions-s.lib" )
#else
#pragma comment ( lib, "sfml-extensions-s-d.lib" )
#endif
#endif


namespace sf {

namespace sc = std::chrono;

using HrClock = sc::high_resolution_clock;
using FloatDuration = sc::duration<float>;
using IntDuration = sc::duration<Int64, std::nano>;
using HrTimePoint = HrClock::time_point;

enum class ScreenSizeType : Int32 { Small = 576, Medium = 614, Large = 864 };

// Small  : 1280 x 720                  -> height 576...
// Medium : 1024 x 768 (or 1366 � 768 ) -> height 614...
// Large  : 1920 x 1080                 -> height 864...


using Point = Vector2f;
using Force = Vector2f;

using RenderTexturePtr = RenderTexture *;
using RenderTextureRef = RenderTexture &;

using RenderWindowPtr = RenderWindow *;
using RenderWindowRef = RenderWindow &;

using FloatRectRef = FloatRect &;

using TextureRef = Texture &;
using TextureConstRef = const Texture &;

using VertexPtr = Vertex *;

using LineSegment = std::array<Point, 2>;

using UserId = boost::uuids::uuid;


namespace detail {

template<typename T>
using is_real = std::disjunction<std::is_same<T, float>, std::is_same<T, double>>;

template<typename T>
using is_unsigned_integral = std::conjunction<std::is_unsigned<T>, std::is_integral<T>>;

template<typename R, typename I>
using are_valid_types = std::conjunction<is_real<R>, is_unsigned_integral<I>>;
}


template<typename RType, typename Type = RType>
struct Factory {

    template<typename ... Args>
    RType * operator ( ) ( Args && ... args ) { return new Type ( std::forward<Args> ( args )... ); }
};


// Dealing with radians...

constexpr float pi = 3.1415926535897932384626433832795;
constexpr float half_pi = 1.5707963267948966192313216916398;
constexpr float one_half_pi = 4.7123889803846898576939650749193;
constexpr float two_pi = 6.283185307179586476925286766559;

template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real radianToDegree ( const real radian_ ) noexcept {

    return real { 180 } / pi * radian_;
}


template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real degreeToRadian ( const real degree_ ) noexcept {

    return pi / real { 180 } * degree_;
}

// Between 0 and 2 pi...
float clampRadians ( float r_ ) noexcept;
// The second parameter is a boolean value, true for round-up,
//  false for round-down...
float makeOdd ( const float v_, const bool round_up_ = true ) noexcept;
// Sleep for a number of milliseconds...
void sleepForMilliseconds ( const Int32 milliseconds_ ) noexcept;
void sleepForMicroseconds ( const Int32 microseconds_ ) noexcept;

std::string systemTime ( ) noexcept;

inline void timeBeginPeriod ( ) noexcept {

    ::timeBeginPeriod ( 1 );
}

inline void timeEndPeriod ( ) noexcept {

    ::timeEndPeriod ( 1 );
}

void makeWindowSeeThrough ( RenderWindowRef window ) noexcept;
void makeWindowTransparent ( RenderWindowRef window ) noexcept;
void makeWindowOpaque ( RenderWindowRef window ) noexcept;
void setWindowAlpha ( RenderWindowRef window, Uint8 alpha = 255 ) noexcept;

Int32 getTitlebarHeight ( RenderWindowRef window ) noexcept;
Int32 getWindowTop ( RenderWindowRef window ) noexcept;
Int32 getScreenRefreshRate ( ) noexcept;
ScreenSizeType getScreenSizeType ( ) noexcept;

struct SquareShape : RectangleShape {

    SquareShape ( ) : RectangleShape ( ) { }
    SquareShape ( const float s_ ) : RectangleShape ( Vector2f ( s_, s_ ) ) { }
};

Vector2f centreOrigin ( RectangleShape & shape_ ) noexcept;
Vector2f centreOrigin ( Text & text_ ) noexcept;
Vector2f centreRightOrigin ( Text & text_ ) noexcept;
Vector2f centreLeftOrigin ( Text & text_ ) noexcept;

bool segmentIntersectsRectangle ( const Point & p1_, const Point & p2_, const RectangleShape & rectangle_ ) noexcept;
std::optional<Point> lineSegmentIntersection ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept;
std::optional<Point> lineSegmentIntersectionStrict ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept;

// If ( rv > 0 ) then C is to the left...
// If ( rv = 0 ) then C is on the same line...
// If ( rv < 0 ) then C is to the right...
template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real perpDotProduct ( const Vector2<real> & a_, const Vector2<real> & b_ ) noexcept {

    // 2D Cross Product...

    return a_.x * b_.y - a_.y * b_.x;
}

// If ( rv > 0 ) then C is to the left...
// If ( rv = 0 ) then C is on the same line...
// If ( rv < 0 ) then C is to the right...
template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real perpDotProduct ( const Vector2<real> & a_, const Vector2<real> & b_, const Vector2<real> & c_ ) noexcept {

    // 2D Cross Product...

    return perpDotProduct ( b_ - a_, c_ - a_ );
}


template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real dotProduct ( const Vector2<real> & a_, const sf::Vector2<real> & b_ ) noexcept {

    return a_.x * b_.x + a_.y * b_.y;
}

template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real squaredLength ( const Vector2<real> & v_ ) noexcept {

    return v_.x * v_.x + v_.y * v_.y;
}

template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
real length ( const Vector2<real> & v_ ) noexcept {

    return std::sqrt ( v_.x * v_.x + v_.y * v_.y );
}


template<typename real, typename sfinae = typename std::enable_if<detail::is_real<real>::value>::type>
Vector2<real> normalize ( const Vector2<real> & v_ ) noexcept {

    const real length = length ( v_ );

    return { v_.x / length, v_.y / length };
}


template<typename S>
S & operator << ( S & out_, const Vector2f & v_ ) {

    std::printf ( "<%+0.1f %+0.1f>", v_.x, v_.y );

    return out_;
}

template<typename S>
S & operator << ( S & out_, const Vector2i & v_ ) {

    std::printf ( "<%+i %+i>", v_.x, v_.y );

    return out_;
}


// Loading resources...

// Loads T (Image, Texture, Font, SoundBuffer)
// with image data from a resource (.rc) file...
template<typename T>
void loadFromResource ( T &destination_, const Int32 name_ ) {

    HRSRC rsrc_data = FindResource ( NULL, MAKEINTRESOURCE ( name_ ), L"FILEDATA" );

    if ( ! ( rsrc_data ) ) {

        throw std::runtime_error ( "Failed to find resource." );
    }

    DWORD rsrc_data_size = SizeofResource ( NULL, rsrc_data );

    if ( rsrc_data_size <= 0 ) {

        throw std::runtime_error ( "Size of resource is 0." );
    }

    HGLOBAL grsrc_data = LoadResource ( NULL, rsrc_data );

    if ( ! ( grsrc_data ) ) {

        throw std::runtime_error ( "Failed to load resource." );
    }

    LPVOID first_byte = LockResource ( grsrc_data );

    if ( ! ( first_byte ) ) {

        throw std::runtime_error ( "Failed to lock resource." );
    }

    if ( ! ( destination_.loadFromMemory ( first_byte, rsrc_data_size ) ) ) {

        throw std::runtime_error ( "Failed to load resource from memory." );
    }
}


void loadFromResource ( sf::Music & destination_, const Int32 name_ );
std::string loadFromResource ( const Int32 name );
void loadFromResource ( Shader &shader_, const Shader::Type type_, const Int32 name_ );
void loadFromResource ( Shader &shader_, const Int32 vertex_name_, const Int32 fragment_name_ );


// Vector2 casting...

template<typename T>
Vector2f castVector2f ( const T x_, const T y_ ) {

    return Vector2f ( static_cast< float > ( x_ ), static_cast< float > ( y_ ) );
}

template<typename T>
Vector2f castVector2f ( const Vector2<T> &v_ ) {

    return castVector2f ( v_.x, v_.y );
}

template<typename T>
Vector2i castVector2i ( const T x_, const T y_ ) {

    return Vector2i ( static_cast< Int32 > ( x_ ), static_cast< Int32 > ( y_ ) );
}

template<typename T>
Vector2i castVector2i ( const Vector2<T> &v_ ) {

    return castVector2i ( v_.x, v_.y );
}


// Apply same sort to more than 1 vector...
template<typename T>
std::vector<std::size_t> sortPermutation ( const std::vector<T> &vector_ ) {

    std::vector<std::size_t> permutation ( vector_.size ( ) );

    std::iota ( permutation.begin ( ), permutation.end ( ), 0 );
    std::sort ( permutation.begin ( ), permutation.end ( ), [ & ] ( std::size_t i, std::size_t j ) { return vector_ [ i ] < vector_ [ j ]; } );

    return permutation;
}

template <typename T>
std::vector<T> applyPermutation ( const std::vector<T> &vector_, const std::vector<std::size_t> &permutation_ ) {

    std::vector<T> sorted_vector ( permutation_.size ( ) );

    std::transform ( permutation_.begin ( ), permutation_.end ( ), sorted_vector.begin ( ), [ & ] ( std::size_t i ) { return vector_ [ i ]; } );

    return sorted_vector;
}


template <typename T>
void negateAbs ( T &x_ ) noexcept {

    if ( x_ > T { 0 } ) {

        x_ = -x_;
    }
}


template <typename T>
T clamp ( const T x_, const T a_, const T b_ ) noexcept {

    if ( x_ >= b_ ) {

        return b_;
    }

    if ( x_ <= a_ ) {

        return a_;
    }

    return x_;
}


IntRect getResourceRect ( const float square_size_, const std::array<Int32, 4> & size_ );

template<typename T>
inline float min ( const T a, const T b, const T c ) noexcept { return std::min ( std::min ( a, b ), c ); }
template<typename T>
inline float max ( const T a, const T b, const T c ) noexcept { return std::max ( std::max ( a, b ), c ); }


// Pacer



struct Pacer {

    private:

    IntDuration m_duration;
    HrTimePoint m_start, m_time;

    unsigned int m_ui;

    int c_start;

    const Int64 m_frequency;

    Int64 qpfrequency ( ) const noexcept {

        timeBeginPeriod ( );
        LARGE_INTEGER f;
        QueryPerformanceFrequency ( &f );

        return ( Int64 ) std::ceil ( ( double ) f.QuadPart / ( double ) getScreenRefreshRate ( ) );
    }

    Int64 qpcounter ( ) const noexcept {

        LARGE_INTEGER c;
        QueryPerformanceCounter ( &c );

        return c.QuadPart;
    }

    public:

    void reset ( const Int64 frames_per_second_ ) noexcept;

    Pacer ( ) noexcept : m_frequency ( qpfrequency ( ) ) { reset ( getScreenRefreshRate ( ) ); }
    Pacer ( const Int32 frames_per_second_ ) noexcept : m_frequency ( qpfrequency ( ) ) { reset ( frames_per_second_ ); }

    ~Pacer ( ) { timeEndPeriod ( ); }

    HrTimePoint pace ( ) noexcept;

    bool haveElapsed ( sc::milliseconds ms_ ) const noexcept;
    bool haveNotElapsed ( sc::milliseconds ms_ ) const noexcept;

    IntDuration now ( ) noexcept;

    Int64 frequency ( ) const noexcept {

        return m_frequency;
    }
};


}


#if 0
namespace detail {

LineSegment getBoundingBox ( const Point & a0_, const Point & a1_ ) noexcept {

    return {

        Vector2f { std::min ( a0_.x, a1_.x ), std::min ( a0_.y, a1_.y ) },
        Vector2f { std::max ( a0_.x, a1_.x ), std::max ( a0_.y, a1_.y ) }
    };
}

bool doBoundingBoxesIntersect ( const LineSegment & a_, const LineSegment & b_ ) noexcept {

    return a_ [ 0 ].x <= b_ [ 1 ].x and a_ [ 1 ].x >= b_ [ 0 ].x and a_ [ 0 ].y <= b_ [ 1 ].y and a_ [ 1 ].y >= b_ [ 0 ].y;
}


bool doLineSegmentsIntersect ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept {

    return doBoundingBoxesIntersect ( getBoundingBox ( p0_, p1_ ), getBoundingBox ( p2_, p3_ ) );
}


bool equal ( const float a_, const float b_ ) noexcept {

    return std::abs ( b_ - a_ ) < 4.0f * FLT_EPSILON;
}

bool equal ( const Point & a_, const Point & b_ ) noexcept {

    return equal ( a_.x, b_.x ) and equal ( a_.y, b_.y );
}
}


LineSegment getBoundingBox ( const LineSegment & a_ ) noexcept {

    return {

        Vector2f { std::min ( a_ [ 0 ].x, a_ [ 1 ].x ), std::min ( a_ [ 0 ].y, a_ [ 1 ].y ) },
        Vector2f { std::max ( a_ [ 0 ].x, a_ [ 1 ].x ), std::max ( a_ [ 0 ].y, a_ [ 1 ].y ) }
    };
}


bool isPointOnLine ( const LineSegment & a_, const Point & b_ ) noexcept {

    // Move the image, so that a_.first is on (0|0)...

    const float r = perpDotProduct<float> ( { a_ [ 1 ].x - a_ [ 0 ].x, a_ [ 1 ].y - a_ [ 0 ].y }, { b_.x - a_ [ 0 ].x, b_.y - a_ [ 0 ].y } );

    return std::abs ( r ) < 4.0f * FLT_EPSILON;
}

bool isPointRightOfLine ( const LineSegment & a_, const Point & b_ ) noexcept {

    // Move the image, so that a_.first is on (0|0)...

    return perpDotProduct<float> ( { a_ [ 1 ].x - a_ [ 0 ].x, a_ [ 1 ].y - a_ [ 0 ].y }, { b_.x - a_ [ 0 ].x, b_.y - a_ [ 0 ].y } ) < 0.0f;
}

bool lineSegmentTouchesOrCrossesLine ( const LineSegment & a_, const LineSegment & b_ ) noexcept {

    return isPointOnLine ( a_, b_ [ 0 ] ) or isPointOnLine ( a_, b_ [ 1 ] ) or ( isPointRightOfLine ( a_, b_ [ 0 ] ) xor isPointRightOfLine ( a_, b_ [ 1 ] ) );
}

bool doLinesIntersect ( const LineSegment & a_, const LineSegment & b_ ) noexcept {

    return detail::doBoundingBoxesIntersect ( getBoundingBox ( a_ ), getBoundingBox ( b_ ) ) and lineSegmentTouchesOrCrossesLine ( a_, b_ ) and lineSegmentTouchesOrCrossesLine ( b_, a_ );
}
#endif
