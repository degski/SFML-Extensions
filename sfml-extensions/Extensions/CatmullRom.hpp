
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
#include <vector>

#include <boost/container/deque.hpp>

#include <SFML/Graphics.hpp>
#include "Extensions.hpp"


namespace sf::CatmullRom {

    using Points = boost::container::deque<Point>;
    using CoordinatesVector = std::vector<Point>;

    template<typename Container>
    float length ( const Container &v_ ) noexcept {

        Vector2f d;
        float return_value = 0.0f;

        for ( std::size_t i = 1, l = v_.size ( ); i < l; ++i ) {

            d = v_ [ i ] - v_ [ i - 1 ];

            return_value += std::sqrtf ( d.x * d.x + d.y * d.y );
        }

        return return_value;
    }

    // CoordinatesVector catmullRom0 ( const Points &points_, const int number_of_points_per_interval_ );

    // Calculate Catmull Rom for a chain of points and return the combined curve. The
    // chains' first and last point are extrapolated from the second and 1-before-
    // last point, respectively...
    CoordinatesVector catmullRom ( const Points & points_, const Int32 number_of_points_per_interval_ ) noexcept;

    // Calculate Catmull Rom for a chain of points and return the combined curve. The
    // chains' first and last point are extrapolated from the second and 1-before-
    // last point, respectively. The returned points are at a distance of distance_
    // (at least) away from each other... 	Requires 3 points or more...
    CoordinatesVector catmullRom ( const Points & points_, float distance_ ) noexcept;
}  // namespace sf::CatmullRom
