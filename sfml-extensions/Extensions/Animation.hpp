
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

#include <cmath>

#include <chrono>
#include <functional>
#include <type_traits>

#include <plf/plf_colony.h>

#include "Extensions.hpp"

// #define DELEGATE_IMP

#ifdef DELEGATE_IMP
#include "Delegate.hpp"
#endif

namespace sf {

struct easing {

    // Values change with constant speed...

    static constexpr struct linearEasing {

        template < typename T >
        static typename std::enable_if<std::is_integral< T >::value, T>::type run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( std::roundf ( ( end - start ) * position + start ) );
        }

        template < typename T >
        static typename std::enable_if<!std::is_integral< T >::value, T>::type run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( end - start ) * position + start );
        }

    } linear = linearEasing { };

    // Accelerate initial values with a quadratic equation...

    static constexpr struct quadraticInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= position;

            return static_cast < T > ( ( end - start ) * position + start );
        }

    } quadraticIn = quadraticInEasing { };

    // Deccelerate ending values with a quadratic equation...

    static constexpr struct quadraticOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( -( end - start ) ) * position * ( position - 2.0f ) + start );
        }

    } quadraticOut = quadraticOutEasing { };

    // Accelerate initial and deccelerate ending values with a quadratic equation...

    static constexpr struct quadraticInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                position *= position;

                return static_cast < T > ( ( ( end - start ) / 2.0f ) * position + start );
            }

            --position;

            return static_cast < T > ( ( -( end - start ) / 2.0f ) * ( position * ( position - 2.0f ) - 1.0f ) + start );
        }

    } quadraticInOut = quadraticInOutEasing { };

    // Accelerate initial values with a cubic equation...

    static constexpr struct cubicInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            const float pos = position;

            position *= position;

            return static_cast < T > ( ( end - start ) * position * pos + start );
        }

    } cubicIn = cubicInEasing { };

    // Deccelerate ending values with a cubic equation...

    static constexpr struct cubicOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            --position;

            const float pos = position;

            position *= position;

            return static_cast < T > ( ( end - start ) * ( position * pos + 1 ) + start );
        }

    } cubicOut = cubicOutEasing { };

    // Accelerate initial and deccelerate ending values with a cubic equation...

    static constexpr struct cubicInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                const float pos = position;

                position *= position;

                return static_cast < T > ( ( ( end - start ) / 2.0f ) * position * pos + start );
            }

            position -= 2.0f;

            const float pos = position;

            position *= position;

            return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( position * pos + 2.0f ) + start );
        }

    } cubicInOut = cubicInOutEasing { };


    // Accelerate initial values with a quartic equation...

    static constexpr struct quarticInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= position;
            position *= position;

            return static_cast < T > ( ( end - start ) * position + start );
        }

    } quarticIn = quarticInEasing { };

    // Deccelerate ending values with a quartic equation...

    static constexpr struct quarticOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            --position;

            position *= position;
            position *= position;

            return static_cast < T > ( -( end - start ) * ( position - 1.0f ) + start );
        }

    } quarticOut = quarticOutEasing { };

    // Accelerate initial and deccelerate ending values with a quartic equation...

    static constexpr struct quarticInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                position *= position;
                position *= position;

                return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( position ) +start );
            }

            position -= 2.0f;
            position *= position;
            position *= position;

            return static_cast < T > ( ( -( end - start ) / 2.0f ) * ( position - 2.0f ) + start );
        }

    } quarticInOut = quarticInOutEasing { };

    // Accelerate initial values with a quintic equation...

    static constexpr struct quinticInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            const float pos = position;

            position *= position;
            position *= position;

            return static_cast < T > ( ( end - start ) * position * pos + start );
        }

    } quinticIn = quinticInEasing { };

    // Deccelerate ending values with a quintic equation...

    static constexpr struct quinticOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position--;

            const float pos = position;

            position *= position;
            position *= position;

            return static_cast < T > ( ( end - start ) * ( position * pos + 1.0f ) + start );
        }

    } quinticOut = quinticOutEasing { };

    // Accelerate initial and deccelerate ending values with a quintic equation...

    static constexpr struct quinticInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                const float pos = position;

                position *= position;
                position *= position;

                return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( position * pos ) + start );
            }

            position -= 2.0f;

            const float pos = position;

            position *= position;
            position *= position;

            return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( position * pos + 2.0f ) + start );
        }

    } quinticInOut = quinticInOutEasing { };

    // Accelerate initial values with a sinusoidal equation...

    static constexpr struct sinusoidalInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( -( end - start ) * std::cosf ( position * half_pi ) + ( end - start ) + start );
        }

    } sinusoidalIn = sinusoidalInEasing { };

    // Deccelerate ending values with a sinusoidal equation...

    static constexpr struct sinusoidalOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( end - start ) * std::sinf ( position * half_pi ) + start );
        }

    } sinusoidalOut = sinusoidalOutEasing { };

    // Accelerate initial and deccelerate ending values with a sinusoidal equation...

    static constexpr struct sinusoidalInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( -( end - start ) / 2.0f ) * ( std::cosf ( position * static_cast < float > ( pi ) ) - 1.0f ) + start );
        }

    } sinusoidalInOut = sinusoidalInOutEasing { };

    // Accelerate initial values with an exponential equation...

    static constexpr struct exponentialInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( end - start ) * std::powf ( 2.0f, 10.0f * ( position - 1.0f ) ) + start );
        }

    } exponentialIn = exponentialInEasing { };

    // Deccelerate ending values with an exponential equation...

    static constexpr struct exponentialOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return static_cast < T > ( ( end - start ) * ( -std::powf ( 2.0f, -10.0f * position ) + 1.0f ) + start );
        }

    } exponentialOut = exponentialOutEasing { };

    // Accelerate initial and deccelerate ending values with an exponential equation...

    static constexpr struct exponentialInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                return static_cast < T > ( ( ( end - start ) / 2.0f ) * powf ( 2.0f, 10.0f * ( position - 1.0f ) ) + start );
            }

            --position;

            return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( -powf ( 2.0f, -10.0f * position ) + 2.0f ) + start );
        }

    } exponentialInOut = exponentialInOutEasing { };

    // Accelerate initial values with a circular equation...

    static constexpr struct circularInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= position;

            return static_cast < T > ( -( end - start ) * ( std::sqrtf ( 1.0f - position ) - 1.0f ) + start );
        }

    } circularIn = circularInEasing { };

    // Deccelerate ending values with a circular equation...

    static constexpr struct circularOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            --position;

            position *= position;

            return static_cast < T > ( ( end - start ) * ( std::sqrtf ( 1.0f - position ) ) + start );
        }

    } circularOut = circularOutEasing { };

    // Accelerate initial and deccelerate ending values with a circular equation...

    static constexpr struct circularInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            position *= 2.0f;

            if ( position < 1.0f ) {

                position *= position;

                return static_cast < T > ( ( -( end - start ) / 2.0f ) * ( std::sqrtf ( 1.0f - position ) - 1.0f ) + start );
            }

            position -= 2.0f;
            position *= position;

            return static_cast < T > ( ( ( end - start ) / 2.0f ) * ( std::sqrtf ( 1.0f - position ) + 1.0f ) + start );
        }

    } circularInOut = circularInOutEasing { };

    // Accelerate initial values with a "bounce" equation...

    static constexpr struct bounceInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            return ( end - start ) - bounceOut.run ( ( 1.0f - position ), T ( ), end ) + start;
        }

    } bounceIn = bounceInEasing { };

    // Deccelerate ending values with a "bounce" equation...

    static constexpr struct bounceOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            const T c = end - start;

            if ( position < ( 1.0f / 2.75f ) ) {

                position *= position;

                return static_cast < T > ( c * ( 7.5625f * position ) + start );
            }

            else if ( position < ( 2.0f / 2.75f ) ) {

                const float postFix = position -= ( 1.5f / 2.75f );

                return static_cast < T > ( c * ( 7.5625f * ( postFix ) * position + 0.75f ) + start );
            }

            else if ( position < ( 2.5f / 2.75f ) ) {

                const float postFix = position -= ( 2.25f / 2.75f );

                return static_cast < T > ( c * ( 7.5625f * ( postFix ) * position + 0.9375f ) + start );
            }

            else {

                const float postFix = position -= ( 2.625f / 2.75f );

                return static_cast < T > ( c * ( 7.5625f * ( postFix ) * position + 0.984375f ) + start );
            }
        }

    } bounceOut = bounceOutEasing { };

    // Accelerate initial and deccelerate ending values with a "bounce" equation...

    static constexpr struct bounceInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            if ( position < 0.5f ) {

                return static_cast < T > ( bounceIn.run ( position * 2.0f, T ( ), end ) * 0.5f + start );
            }

            else {

                return static_cast < T > ( bounceOut.run ( ( position * 2.0f - 1.0f ), T ( ), end ) * 0.5f + ( end - start ) * 0.5f + start );
            }
        }

    } bounceInOut = bounceInOutEasing { };

    // Accelerate initial values with an "elastic" equation...

    static constexpr struct elasticInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            if ( position <= 0.00001f ) return start;
            if ( position >= 0.999f ) return end;

            const float p = 0.3f;
            const float a = end - start;
            const float s = p / 4.0f;
            const float postFix = a * std::powf ( 2.0f, 10.0f * ( position -= 1.0f ) ); // this is a fix, again, with post-increment operators

            return static_cast < T > ( -( postFix * std::sinf ( ( position - s ) * ( 2.0f * static_cast < float > ( pi ) ) / p ) ) + start );
        }

    } elasticIn = elasticInEasing { };

    // Deccelerate ending values with an "elastic" equation...

    static constexpr struct elasticOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            if ( position <= 0.00001f ) return start;
            if ( position >= 0.999f ) return end;

            const float p = 0.3f;
            const float a = end - start;
            const float s = p / 4.0f;

            return static_cast < T > ( a * std::powf ( 2.0f, -10.0f * position ) * std::sinf ( ( position - s ) * ( 2.0f * static_cast < float > ( pi ) ) / p ) + end );
        }

    } elasticOut = elasticOutEasing { };

    // Accelerate initial and deccelerate ending values with an "elastic" equation...

    static constexpr struct elasticInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            if ( position <= 0.00001f ) return start;
            if ( position >= 0.999f ) return end;

            position *= 2.0f;

            const float p = ( 0.3f * 1.5f );
            const float a = end - start;
            const float s = p / 4.0f;

            if ( position < 1.0f ) {

                const float postFix = a * std::powf ( 2.0f, 10.0f * ( position -= 1.0f ) ); // postIncrement is evil

                return static_cast < T > ( -0.5f * ( postFix * std::sinf ( ( position - s ) * ( 2.0f * static_cast < float > ( pi ) ) / p ) ) + start );
            }

            const float postFix = a * std::powf ( 2.0f, -10.0f * ( position -= 1.0f ) ); // postIncrement is evil

            return static_cast < T > ( postFix * std::sinf ( ( position - s ) * ( 2.0f * static_cast < float > ( pi ) ) / p ) * 0.5f + end );
        }

    } elasticInOut = elasticInOutEasing { };

    // Accelerate initial values with a "back" equation...

    static constexpr struct backInEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            const float s = 1.70158f;
            const float postFix = position;

            return static_cast < T > ( ( end - start ) * ( postFix ) * position * ( ( s + 1.0f ) * position - s ) + start );
        }

    } backIn = backInEasing { };

    // Deccelerate ending values with a "back" equation...

    static constexpr struct backOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            const float s = 1.70158f;
            position -= 1.0f;

            return static_cast < T > ( ( end - start ) * ( ( position ) * position * ( ( s + 1.0f ) * position + s ) + 1.0f ) + start );
        }

    } backOut = backOutEasing { };

    // Accelerate initial and deccelerate ending values with a "back" equation...

    static constexpr struct backInOutEasing {

        template < typename T >
        static T run ( float position, const T start, const T end ) noexcept {

            float s = 1.70158f;
            float t = position;
            const T b = start;
            const T c = end - start;
            const float d = 1.0f;

            s *= 1.525f;

            if ( ( t /= d / 2.0f ) < 1.0f ) {

                return static_cast < T > ( c / 2.0f * ( t * t * ( ( ( s ) +1.0f ) * t - s ) ) + b );
            }

            const float postFix = t -= 2.0f;

            return static_cast < T > ( c / 2.0f * ( ( postFix ) * t * ( ( ( s ) +1.0f ) * t + s ) + 2.0f ) + b );
        }

    } backInOut = backInOutEasing { };
};


struct CallbackTimer {

    enum class Status : Int32 { waiting, animating, finished };

    using Callback = std::function < void ( const float ) >;
    using Easing = float ( * ) ( float, const float, const float );

    Callback m_callback;
    Easing m_easing;
    float m_duration, m_start, m_end;
    HrTimePoint m_start_time;

    CallbackTimer ( Callback && callback_, Easing easing_, const float start_, const float end_, const FloatDuration duration_, const std::chrono::milliseconds delay_ = std::chrono::milliseconds { 0 } ) noexcept;

    Status run ( ) noexcept;

    private:

    static HrClock s_clock;
};
}

#define INSTANCE_CALLBACK_EASING_START_END_DURATION( I, F, AE, P1, P2, D ) std::bind ( &decltype(I)::F, &I, _1 ), &AE::run<float>, P1, P2, std::chrono::milliseconds { ( D ) }
#define INSTANCE_CALLBACK_EASING_START_END_DURATION_DELAY( I, F, AE, P1, P2, D, E ) std::bind ( &decltype(I)::F, &I, _1 ), &AE::run<float>, P1, P2, std::chrono::milliseconds { ( D ) }, std::chrono::milliseconds { ( E ) }


#ifdef DELEGATE_IMP

namespace sf {

struct DelegateTimer {

    enum class Status : Int32 { waiting, animating, finished };

    using Delegate = fd::delegate < void ( float ) >;
    using Easing = float ( * ) ( float, const float, const float );

    Delegate m_delegate;
    Easing m_easing;
    float m_duration, m_start, m_end;
    HrTimePoint m_start_time;

    DelegateTimer ( Delegate && delegate_, Easing easing_, const float start_, const float end_, const FloatDuration duration_, const std::chrono::milliseconds delay_ = std::chrono::milliseconds { 0 } ) noexcept;

    Status run ( ) noexcept;

    private:

    static HrClock s_clock;
};
}

#define INSTANCE_DELEGATE_EASING_START_END_DURATION( I, F, AE, P1, P2, D ) sf::fd::delegate < void ( float )>::from < decltype(I), &decltype(I)::F > ( &I ), &AE::run<float>, P1, P2, std::chrono::milliseconds { ( D ) } }
#define INSTANCE_DELEGATE_EASING_START_END_DURATION_DELAY( I, F, AE, P1, P2, D, E ) sf::fd::delegate < void ( float )>::from < decltype(I), &decltype(I)::F > ( &I ), &AE::run<float>, P1, P2, std::chrono::milliseconds { ( D ) }, std::chrono::milliseconds { ( E ) }

#endif

namespace sf {

template<typename Timer>
class Animator {

public:

    using Timers = plf::colony<Timer>;

private:

    Timers m_timers;

public:

    inline void reserve ( const uint32_t r_ ) noexcept {
        m_timers.reserve ( r_ );
    }

    template < typename ... Args >
    inline void emplace ( Args ... args_ ) noexcept {
        m_timers.emplace ( std::forward < Args > ( args_ ) ... );
    }

    void run ( ) noexcept {
        typename Timers::iterator it = std::begin ( m_timers );
        while ( std::end ( m_timers ) != it ) {
            if ( Timer::Status::finished == it->run ( ) ) {
                it = m_timers.erase ( it );
            }
            else {
                ++it;
            }
        }
    }

    inline uint32_t size ( ) const noexcept {
        return ( uint32_t ) m_timers.size ( );
    }

    inline bool empty ( ) const noexcept {
        return m_timers.empty ( );
    }
};

using CallbackAnimator = Animator<CallbackTimer>;
#ifdef DELEGATE_IMP
using DelegateAnimator = Animator<DelegateTimer>;
#endif

}
