
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

#include <chrono>
#include <cmath>

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>

#include "./Extensions/Extensions.hpp"
#include "./Extensions/Box.hpp"

namespace sf {

float clampRadians ( float r_ ) noexcept {
    // Clamp radians between 0 and 2 pi.
    if ( r_ > two_pi ) {
        do {
            r_ -= two_pi;
        } while ( r_ > two_pi );
    }
    if ( r_ < 0.0f ) {
        do {
            r_ += two_pi;
        } while ( r_ < 0.0f );
    }
    return r_;
}


float makeOdd ( const float v_, const bool round_up_ ) noexcept {
    // The second parameter is a_ boolean value, true for round-up, false for round-down.
    if ( v_ > 0.0f ) {
        Uint64 v = ( Uint64 ) v_;
        if ( round_up_ ) {
            v += ( Uint64 ) not ( v & Uint64 { 1 } );
        }
        else {
            v -= ( Uint64 ) not ( v & Uint64 { 1 } );
        }
        return ( float ) v;
    }
    else {
        Uint64 v = ( Uint64 ) -v_;
        if ( round_up_ ) {
            v += ( Uint64 ) not ( v & Uint64 { 1 } );
        }
        else {
            v -= ( Uint64 ) not ( v & Uint64 { 1 } );
        }
        return - ( ( float ) v );
    }
}


void sleepForMilliseconds ( const Int32 milliseconds_ ) noexcept {
    std::this_thread::sleep_for ( std::chrono::milliseconds ( milliseconds_ ) );
}

void sleepForMicroseconds ( const Int32 microseconds_ ) noexcept {
    std::this_thread::sleep_for ( std::chrono::microseconds ( microseconds_ ) );
}


std::string systemTime ( ) noexcept {
    using smc = std::chrono::system_clock;
    const std::time_t now_c = smc::to_time_t ( smc::now ( ) );
    std::ostringstream oss;
    oss << std::put_time ( std::localtime ( &now_c ), "%T" );
    return oss.str ( );
}


void makeWindowSeeThrough ( RenderWindowRef window ) noexcept {
    // https://en.sfml-dev.org/forums/index.php?topic=21118.msg150860#msg150860
    HWND hwnd = window.getSystemHandle ( );
    MARGINS margins;
    margins.cxLeftWidth = -1;
    SetWindowLongPtr ( hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
    DwmExtendFrameIntoClientArea ( hwnd, &margins );
}


void makeWindowTransparent ( RenderWindowRef window ) noexcept {
    HWND hwnd = window.getSystemHandle ( );
    SetWindowLongPtr ( hwnd, GWL_EXSTYLE, GetWindowLongPtr ( hwnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
}


void makeWindowOpaque ( RenderWindowRef window ) noexcept {
    HWND hwnd = window.getSystemHandle ( );
    SetWindowLongPtr ( hwnd, GWL_EXSTYLE, GetWindowLongPtr ( hwnd, GWL_EXSTYLE ) & ~WS_EX_LAYERED );
    RedrawWindow ( hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN );
}


void setWindowAlpha ( RenderWindowRef window, Uint8 alpha ) noexcept {
    SetLayeredWindowAttributes ( window.getSystemHandle ( ), 0, alpha, LWA_ALPHA );
}


static Int32 get_titlebar_height_impl ( RenderWindowRef window_ ) noexcept {
    // https://stackoverflow.com/questions/21566307/how-to-get-the-title-bar-height-and-width-for-aero-and-basic-design
    const HWND hwnd = window_.getSystemHandle ( );
    POINT point { 0, 0 };
    RECT rect;
    return ClientToScreen ( hwnd, &point ) and GetWindowRect ( hwnd, &rect ) ? ( Int32 ) ( point.y - rect.top ) : Int32 { -1 };
}

Int32 getTitlebarHeight ( RenderWindowRef window_ ) noexcept {
    static const Int32 th ( get_titlebar_height_impl ( window_ ) );
    return th;
}


static Int32 get_window_top_impl ( RenderWindowRef window_ ) noexcept {
    // https://stackoverflow.com/questions/21566307/how-to-get-the-title-bar-height-and-width-for-aero-and-basic-design
    const HWND hwnd = window_.getSystemHandle ( );
    POINT point { 0, 0 };
    return ClientToScreen ( hwnd, &point ) ? ( Int32 ) point.y : Int32 { -1 };
}


Int32 getWindowTop ( RenderWindowRef window_ ) noexcept {
    static const Int32 wt ( get_window_top_impl ( window_ ) );
    return wt;
}


static Int32 get_screen_refresh_rate_impl ( ) noexcept {
    // https://msdn.microsoft.com/en-us/library/dd162611(v=VS.85).aspx
    DEVMODE devmode;
    devmode.dmSize = sizeof ( DEVMODE );
    devmode.dmDriverExtra = 0;
    return EnumDisplaySettings ( NULL, ENUM_CURRENT_SETTINGS, &devmode ) ? ( Int32 ) devmode.dmDisplayFrequency : Int32 { -1 };
}

Int32 getScreenRefreshRate ( ) noexcept {
    static const Int32 srr ( get_screen_refresh_rate_impl ( ) );
    return srr;
}


static ScreenSizeType screen_size_type_impl ( ) noexcept {
    const Uint32 height = sf::VideoMode::getDesktopMode ( ).height;
    return height > 768u ? ScreenSizeType::Large : ( height > 720u ? ScreenSizeType::Medium : ScreenSizeType::Small );
}

ScreenSizeType getScreenSizeType ( ) noexcept {
    // Small  : 1280 x 720                  -> height 576.
    // Medium : 1024 x 768 (or 1366  768 )  -> height 614.
    // Large  : 1920 x 1080                 -> height 864.
    static const ScreenSizeType type ( screen_size_type_impl ( ) );

    return type;
}


Vector2f centreOrigin ( RectangleShape & shape_ ) noexcept {
    shape_.setOrigin ( shape_.getSize ( ).x * 0.5f, shape_.getSize ( ).y * 0.5f );
    return shape_.getOrigin ( );
}


Vector2f centreOrigin ( Sprite & sprite_ ) noexcept {
    sprite_.setOrigin ( sprite_.getLocalBounds ( ).width * 0.5f, sprite_.getLocalBounds ( ).height * 0.5f );
    return sprite_.getOrigin ( );
}


Vector2f centreOrigin ( Text & text_ ) noexcept {
    text_.setOrigin ( text_.getLocalBounds ( ).width * 0.5f, text_.getLocalBounds ( ).height * 0.5f );
    return text_.getOrigin ( );
}


Vector2f centreRightOrigin ( Text & text_ ) noexcept {
    text_.setOrigin ( text_.getLocalBounds ( ).width, text_.getLocalBounds ( ).height * 0.5f );
    return text_.getOrigin ( );
}


Vector2f centreLeftOrigin ( Text & text_ ) noexcept {
    text_.setOrigin ( 0.0f, text_.getLocalBounds ( ).height * 0.5f );
    return text_.getOrigin ( );
}


namespace detail {


bool equal ( const float a_, const float b_ ) noexcept {
    return std::abs ( b_ - a_ ) < 4.0f * FLT_EPSILON;
}

bool equal ( const Point & a_, const Point & b_ ) noexcept {
    return equal ( a_.x, b_.x ) and equal ( a_.y, b_.y );
}

bool not_equal ( const float a_, const float b_ ) noexcept {
    return std::abs ( b_ - a_ ) >= 4.0f * FLT_EPSILON;
}

bool not_equal ( const Point & a_, const Point & b_ ) noexcept {
    return not_equal ( a_.x, b_.x ) or not_equal ( a_.y, b_.y );
}


FloatBox boundingBox ( const Point & a_, const Point & b_ ) noexcept {
    if ( a_.x < b_.x ) {
        return a_.y < b_.y ? FloatBox { a_.x, a_.y, b_.x, b_.y } : FloatBox { a_.x, b_.y, b_.x, a_.y };
    }
    else {
        return a_.y < b_.y ? FloatBox { b_.x, a_.y, a_.x, b_.y } : FloatBox { b_.x, b_.y, a_.x, a_.y };
    }
}

bool doBoundingBoxesIntersect ( const FloatBox & a_, const FloatBox & b_ ) noexcept {
    return a_.left <= b_.right and a_.right >= b_.left and a_.top <= b_.bottom and a_.bottom >= b_.top;
}

bool doLineSegmentsIntersect ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept {
    return doBoundingBoxesIntersect ( boundingBox ( p0_, p1_ ), boundingBox ( p2_, p3_ ) );
}


template<bool Strict>
std::optional<Point> lineSegmentIntersection ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept {
    // https://stackoverflow.com/questions/563198/whats-the-most-efficent-way-to-calculate-where-two-line-segments-intersect#565282
    const float s1_x = p1_.x - p0_.x, s1_y = p1_.y - p0_.y, s2_x = p3_.x - p2_.x, s2_y = p3_.y - p2_.y;
    const float perp_dot_prod = -s2_x * s1_y + s1_x * s2_y;
    if ( equal ( perp_dot_prod, 0.0f ) ) {
        // Co-linear.
        if constexpr ( Strict ) {
            if ( doLineSegmentsIntersect ( p0_, p1_, p2_, p3_ ) ) {
                if ( equal ( p0_, p2_ ) or equal ( p0_, p3_ ) ) {
                    return std::optional<Point> ( p0_ );
                }
                if ( equal ( p1_, p2_ ) or equal ( p1_, p3_ ) ) {
                    return std::optional<Point> ( p1_ );
                }
                return { Point { std::numeric_limits<float>::infinity ( ), std::numeric_limits<float>::infinity ( ) } };
            }
        }
        else {
            if ( equal ( p0_, p2_ ) or equal ( p0_, p3_ ) ) {
                return std::optional<Point> ( p0_ );
            }
            if ( equal ( p1_, p2_ ) or equal ( p1_, p3_ ) ) {
                return std::optional<Point> ( p1_ );
            }
        }
        return std::optional<Point> ( );
    }
    const float dif_x = p0_.x - p2_.x, dif_y = p0_.y - p2_.y;
    const float s = ( -s1_y * dif_x + s1_x * dif_y ) / perp_dot_prod;
    const float t = ( +s2_x * dif_y - s2_y * dif_x ) / perp_dot_prod;
    if ( s >= 0.0f and s <= 1.0f and t >= 0.0f and t <= 1.0f ) {
        // Collision detected.
        return Point { p0_.x + ( t * s1_x ), p0_.y + ( t * s1_y ) };
    }
    return std::optional<Point> ( );
}
}


bool segmentIntersectsRectangle ( const Point & p1_, const Point & p2_, const RectangleShape & rectangle_ ) noexcept {
    // https://stackoverflow.com/questions/5514366/how-to-know-if-a_-line-intersects-a_-rectangle#5514619
    const FloatBox rectangle_bounds ( rectangle_.getGlobalBounds ( ) );
    // Find min and max X for the segment.
    float min_x = p1_.x, max_x = p2_.x;
    if ( p1_.x > p2_.x ) {
        min_x = p2_.x;
        max_x = p1_.x;
    }
    // Find the intersection of the segment's and rectangle's x-projections.
    if ( max_x > rectangle_bounds.right ) {
        max_x = rectangle_bounds.right;
    }
    if ( min_x < rectangle_bounds.left ) {
        min_x = rectangle_bounds.left;
    }
    // If their projections do not intersect return false.
    if ( min_x > max_x ) {
        return false;
    }
    // Find corresponding min and max Y for min and max X we found before.
    float min_y = p1_.y, max_y = p2_.y;
    const float dx = p2_.x - p1_.x;
    if ( detail::not_equal ( dx, 0.0f ) ) {
        const float a_ = ( p2_.y - p1_.y ) / dx, b = p1_.y - a_ * p1_.x;
        min_y = a_ * min_x + b;
        max_y = a_ * max_x + b;
    }
    if ( min_y > max_y ) {
        std::swap ( max_y, min_y );
    }
    // Find the intersection of the segment's and rectangle's y-projections.
    if ( max_y > rectangle_bounds.bottom ) {
        max_y = rectangle_bounds.bottom;
    }
    if ( min_y < rectangle_bounds.top ) {
        min_y = rectangle_bounds.top;
    }
    // If Y-projections do not intersect return false.
    if ( min_y > max_y ) {
        return false;
    }
    return true;
}


std::optional<Point> lineSegmentIntersection ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept {
    return detail::lineSegmentIntersection<false> ( p0_, p1_, p2_, p3_ );
}


std::optional<Point> lineSegmentIntersectionStrict ( const Point & p0_, const Point & p1_, const Point & p2_, const Point & p3_ ) noexcept {
    return detail::lineSegmentIntersection<true> ( p0_, p1_, p2_, p3_ );
}


std::string loadFromResource ( const Int32 name_ ) {
    // Loads text from a_ resource (.rc) file and return it as a_ string.
    HRSRC rsrc_data = FindResource ( NULL, MAKEINTRESOURCE ( name_ ), L"FILEDATA" );
    if ( not ( rsrc_data ) ) {
        throw std::runtime_error ( "Can not find resource." );
    }
    DWORD rsrc_data_size = SizeofResource ( NULL, rsrc_data );
    if ( rsrc_data_size <= 0 ) {
        throw std::runtime_error ( "Data size of resource is 0." );
    }
    HGLOBAL grsrc_data = LoadResource ( NULL, rsrc_data );
    if ( not ( grsrc_data ) ) {
        throw std::runtime_error ( "Failed to load resource." );
    }
    LPVOID first_byte = LockResource ( grsrc_data );
    if ( not ( first_byte ) ) {
        throw std::runtime_error ( "Failed to lock resource." );
    }
    return std::string ( ( char* ) first_byte, rsrc_data_size );
}


void loadFromResource ( sf::Music & destination_, const Int32 name_ ) {
    HRSRC rsrc_data = FindResource ( NULL, MAKEINTRESOURCE ( name_ ), L"FILEDATA" );
    if ( not rsrc_data ) {
        throw std::runtime_error ( "Failed to find resource." );
    }
    DWORD rsrc_data_size = SizeofResource ( NULL, rsrc_data );
    if ( rsrc_data_size <= 0 ) {
        throw std::runtime_error ( "Size of resource is 0." );
    }
    HGLOBAL grsrc_data = LoadResource ( NULL, rsrc_data );
    if ( not grsrc_data ) {
        throw std::runtime_error ( "Failed to load resource." );
    }
    LPVOID first_byte = LockResource ( grsrc_data );
    if ( not ( first_byte ) ) {
        throw std::runtime_error ( "Failed to lock resource." );
    }
    if ( not ( destination_.openFromMemory ( first_byte, rsrc_data_size ) ) ) {
        throw std::runtime_error ( "Failed to load resource from memory." );
    }
}


void loadFromResource ( Shader & shader_, const Shader::Type type_, const Int32 name_ ) {
    if ( not ( shader_.loadFromMemory ( loadFromResource ( name_ ), type_ ) ) ) {
        throw std::runtime_error ( "Failed to load shader from memory." );
    }
}


void loadFromResource ( Shader & shader_, const Int32 vertex_name_, const Int32 fragment_name_ ) {
    if ( not ( shader_.loadFromMemory ( loadFromResource ( vertex_name_ ), loadFromResource ( fragment_name_ ) ) ) ) {
        throw std::runtime_error ( "Failed to load shader from memory." );
    }
}



void Pacer::reset ( const Int64 frames_per_second_ ) noexcept {
    const Int64 cd = ( Int64 ) std::ceil ( 1'000'000'000.0 / ( double ) frames_per_second_ );
    m_duration = std::chrono::nanoseconds ( cd );
    const double d = 1'000'000'000LL / frames_per_second_;
    c_start = 2;
    for ( ; detail::equal ( c_start * cd - 1LL, c_start * d ); ++c_start );
    // std::cout << ( c_start * cd - 1LL ) << " " << ( c_start * d ) << '\n';
    m_start = m_time = HrClock::now ( );
}


HrTimePoint Pacer::pace ( ) noexcept {
    static int c = c_start;
    // At 60hz, the duration of 1 frame is 16'666'667 nanoseconds, i.e. per 3 nanoseconds 1
    // nanosecond too much (should be: 3 * 16'666'666 + 2/3 = 50'000'000 =/= 50'000'001),
    // the below corrects for that so we avoid drift.
    if ( c-- ) {
        m_time += m_duration;
    }
    else {
        m_time += m_duration - IntDuration { 1LL };
        c = c_start;
    }
    std::this_thread::sleep_until ( m_time );// -duration { 666'666LL } );
    // std::cout << "co " << qpcounter ( ) << '\n';
    return HrClock::now ( );
}


bool Pacer::haveElapsed ( std::chrono::milliseconds ms_ ) const noexcept {
    return ( ( bool ) ( ( m_time - m_start ) > std::chrono::duration_cast<std::chrono::nanoseconds> ( ms_ ) ) );
}


bool Pacer::haveNotElapsed ( std::chrono::milliseconds ms_ ) const noexcept {
    return ( ( bool ) ( ( m_time - m_start ) < std::chrono::duration_cast<std::chrono::nanoseconds> ( ms_ ) ) );
}


IntDuration Pacer::now ( ) noexcept {
    // static const std::chrono::nanoseconds start { __rdtscp ( &m_ui ) };
    return std::chrono::nanoseconds { __rdtscp ( &m_ui ) };
}
}
