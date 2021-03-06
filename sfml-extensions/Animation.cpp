
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

#include "./Extensions/Animation.hpp"


/*

example use


struct Tester {

    void ani1 ( const float value_ ) noexcept {
        std::cout << "ani 1 " << value_ << nl;
    }

    void ani2 ( const float value_ ) noexcept {
        std::cout << "ani 2 " << value_ << nl;
    }
};



int main ( ) {

    Tester tester;

    sf::DelegateAnimator animator;

    animator.emplace ( INSTANCE_CALLBACK_EASING_START_END_DURATION_DELAY ( tester, ani1, sf::easing::quadraticInEasing, 0.0f, 90.0f, 5000, 1000 ) );

    while ( true ) {

        animator.run ( );
        sf::sleepForMilliseconds ( 16 );
    }


    return EXIT_SUCCESS;
}


*/


namespace sf {

CallbackTimer::CallbackTimer ( Callback && callback_, Easing easing_, const float start_, const float end_, const FloatDuration duration_, const std::chrono::milliseconds delay_ ) noexcept :
    m_callback ( callback_ ),
    m_easing ( easing_ ),
    m_duration ( duration_.count ( ) ), m_start ( start_ ), m_end ( end_ ),
    m_start_time ( s_clock.now ( ) + delay_ ) {

}

typename CallbackTimer::Status CallbackTimer::run ( ) noexcept {
    float progress = std::chrono::duration < float > ( s_clock.now ( ) - m_start_time ).count ( );
    if ( progress < 0.0f ) {
        return Status::waiting;
    }
    progress /= m_duration;
    if ( progress < 1.0f ) {
        m_callback ( m_easing ( progress, m_start, m_end ) );
        return Status::animating;
    }
    m_callback ( m_end );
    return Status::finished;
}

HrClock CallbackTimer::s_clock;

}
