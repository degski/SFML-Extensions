
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

#include "./Extensions/CatmullRom.hpp"


// http://stackoverflow.com/questions/9489736/catmull-rom-curve-with-no-cusps-and-no-self-intersections/23980479#23980479


namespace sf::CatmullRom {

struct CubicPoly {

    float c0, c1, c2, c3;

    float evaluate ( const float t_ ) const noexcept {

        const float t2 = t_ * t_;

        return c0 + c1 * t_ + c2 * t2 + c3 * t2 * t_;
    }
};

struct CubicPolyXY {

    CubicPoly x, y;

    Vector2f evaluate ( const float t_ ) const noexcept {

        const float t2 = t_ * t_, t3 = t2 * t_;

        return Vector2f { x.c0 + x.c1 * t_ + x.c2 * t2 + x.c3 * t3, y.c0 + y.c1 * t_ + y.c2 * t2 + y.c3 * t3 };
    }
};
}

namespace sf::CatmullRom::detail {

// Compute coefficients for a cubic polynomial...

//  p(s) = c0 + c1*s + c2*s^2 + c3*s^3
// such that
//   p(0) = v0, p(1) = v1
//  and
//   p'(0) = t0, p'(1) = t1.

void cubicPoly ( const Vector4f & v_, const float t0_, const float t1_, CubicPoly & p_ ) noexcept {

    p_.c0 = v_.v1;
    p_.c1 = t0_;
    p_.c2 = -3.0f * v_.v1 + 3.0f * v_.v2 - 2.0f * t0_ - t1_;
    p_.c3 = 2.0f * v_.v1 - 2.0f * v_.v2 + t0_ + t1_;
}

// Standard Catmull-Rom spline: interpolate between v1 and v2 with previous/following points v1/x4
// (we don't need this here, but it's for illustration)...

void standardCatmullRom ( const Vector4f & v_, CubicPoly & p_ ) noexcept {

    // Catmull-Rom with tension 0.5...

    cubicPoly ( v_, 0.5f * ( v_.v2 - v_.v0 ), 0.5f * ( v_.v3 - v_.v1 ), p_ );
}

// Compute coefficients for a nonuniform Catmull-Rom spline...

void nonuniformCatmullRom ( const Vector4f & v_, const Vector3f & dt_, CubicPoly & p_ ) noexcept {

    // Compute tangents when parameterised in [t1, t2]...

    float t1 = ( v_.v1 - v_.v0 ) / dt_.x - ( v_.v2 - v_.v0 ) / ( dt_.x + dt_.y ) + ( v_.v2 - v_.v1 ) / dt_.y;
    float t2 = ( v_.v2 - v_.v1 ) / dt_.y - ( v_.v3 - v_.v1 ) / ( dt_.y + dt_.z ) + ( v_.v3 - v_.v2 ) / dt_.z;

    // Rescale tangents for parametrisation in [0, 1]...

    t1 *= dt_.x;
    t2 *= dt_.x;

    cubicPoly ( v_, t1, t2, p_ );
}


void centripetalCatmullRom ( const Vector2f & p0_, const Vector2f & p1_, const Vector2f & p2_, const Vector2f & p3_, CubicPolyXY & cp_ ) noexcept {

    const Vector3f dt { std::powf ( squaredLength ( p1_ - p0_ ), 0.25f ), std::powf ( squaredLength ( p2_ - p1_ ), 0.25f ), std::powf ( squaredLength ( p3_ - p2_ ), 0.25f ) };

    // Safety check for repeated points...

    // if ( dt.y < 1e-4f ) dt.y = 1.0f;
    // if ( dt.x < 1e-4f ) dt.x = dt.y;
    // if ( dt.z < 1e-4f ) dt.z = dt.y;

    nonuniformCatmullRom ( Vector4f { p0_.x, p1_.x, p2_.x, p3_.x }, dt, cp_.x );
    nonuniformCatmullRom ( Vector4f { p0_.y, p1_.y, p2_.y, p3_.y }, dt, cp_.y );
}


#define F 4.0f

inline void pointsOnSegment ( CoordinatesVector & return_value_, const CubicPolyXY & poly_, const Vector2f last_point_, const float sqrd_distance_ ) noexcept {

    float base = -1.0f / F;

    sf::Vector2f point;

    do {

        do {

            base += 1.0f / F;

            if ( base > 1.0f ) { // Overshoot, so skip the search with a negative step and restart with a smaller positive step...

                base -= 1.0f / F;

                goto next1;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ > squaredLength ( point - return_value_.back ( ) ) );

        do {

            base -= 1.0f / F / F;

            if ( base < 0.0f ) { // Overshoot, so skip the search with a positive step and restart with a smaller negative step...

                base += 1.0f / F / F;

                goto next2;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ < squaredLength ( point - return_value_.back ( ) ) );

    next1:

        do {

            base += 1.0f / F / F / F;

            if ( base > 1.0f ) {

                base -= 1.0f / F / F / F;

                goto next3;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ > squaredLength ( point - return_value_.back ( ) ) );

    next2:

        do {

            base -= 1.0f / F / F / F / F;

            if ( base < 0.0f ) {

                base += 1.0f / F / F / F / F;

                goto next4;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ < squaredLength ( point - return_value_.back ( ) ) );

    next3:

        do {

            base += 1.0f / F / F / F / F / F;

            if ( base > 1.0f ) {

                base -= 1.0f / F / F / F / F / F;

                goto next5;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ > squaredLength ( point - return_value_.back ( ) ) );

    next4:

        do {

            base -= 1.0f / F / F / F / F / F / F;

            if ( base < 0.0f ) {

                base += 1.0f / F / F / F / F / F / F;

                goto next6;
            }

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ < squaredLength ( point - return_value_.back ( ) ) );

    next5:

        do {

            base += 1.0f / F / F / F / F / F / F / F;

            if ( base > 1.0f ) break;

            point = poly_.evaluate ( base );

        } while ( sqrd_distance_ > squaredLength ( point - return_value_.back ( ) ) );

    next6:

        return_value_.emplace_back ( point );

    } while ( sqrd_distance_ < squaredLength ( last_point_ - point ) );
}

#undef F

}

namespace sf::CatmullRom {

CoordinatesVector catmullRom ( const Points & points_, const Int32 number_of_points_per_interval_ ) noexcept {

    // Calculate Catmull Rom for a chain of points and return the combined curve. The
    // chains' first and last point are extrapolated from the second and 1-before-
    // last point, respectively...

    const Int32 number_of_intervals = ( Int32 ) points_.size ( ) - 1L; // -1 = -3 + 2 extrap.
    const float rec = 1.0f / number_of_points_per_interval_;

    CubicPolyXY poly;

    CoordinatesVector r;

    r.reserve ( number_of_intervals * number_of_points_per_interval_ );

    if ( 1 == number_of_intervals ) { // 2 points...

        // Extrapolate both ends...

        detail::centripetalCatmullRom ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], 2.0f * points_ [ 1 ] - points_ [ 0 ], poly );

        for ( Int32 i = 0; i < number_of_points_per_interval_; ++i ) {

            r.emplace_back ( poly.evaluate ( rec * i ) );
        }
    }

    else { // 3 points or more...

        // Extrapolate...

        detail::centripetalCatmullRom ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], points_ [ 2 ], poly );

        for ( Int32 i = 0; i < number_of_points_per_interval_; ++i ) {

            r.emplace_back ( poly.evaluate ( rec * i ) );
        }

        // Calculate (only runs when 4 points or more)...

        for ( Int32 j = 0, l = ( Int32 ) points_.size ( ) - 3; j < l; ++j ) {

            detail::centripetalCatmullRom ( points_ [ j ], points_ [ j + 1 ], points_ [ j + 2 ], points_ [ j + 3 ], poly );

            for ( Int32 i = 0; i < number_of_points_per_interval_; ++i ) {

                r.emplace_back ( poly.evaluate ( rec * i ) );
            }
        }

        // Extrapolate...

        detail::centripetalCatmullRom ( points_ [ number_of_intervals - 2 ], points_ [ number_of_intervals - 1 ], points_ [ number_of_intervals ], 2.0f * points_ [ number_of_intervals ] - points_ [ number_of_intervals - 1 ], poly );

        for ( Int32 i = 0; i < number_of_points_per_interval_; ++i ) {

            r.emplace_back ( poly.evaluate ( rec * i ) );
        }
    }

    return r;
}


CoordinatesVector catmullRom ( const Points & points_, float distance_ ) noexcept {

    // Calculate Catmull Rom for a chain of points and return the combined curve. The
    // chains' first and last point are extrapolated from the second and 1-before-
    // last point, respectively. The returned points are at a distance of distance_
    // (at least) away from each other... 	Requires 3 points or more...

    // Square the distance, to avoid calculating a square root for length...

    assert ( 2ULL < points_.size ( ) );

    distance_ *= distance_;

    CubicPolyXY poly;

    static std::vector<Vector2f> return_value;

    // Prepare return_value...

    return_value.clear ( );
    return_value.emplace_back ( points_ [ 0 ] );

    if ( 2ULL == points_.size ( ) ) {

        // Extrapolate 2 points...

        detail::centripetalCatmullRom ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], 2.0f * points_ [ 1 ] - points_ [ 0 ], poly );
        detail::pointsOnSegment ( return_value, poly, points_ [ 1 ], distance_ );

        return return_value;
    }

    else {

        // Extrapolate 3 points...

        detail::centripetalCatmullRom ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], points_ [ 2 ], poly );
        detail::pointsOnSegment ( return_value, poly, points_ [ 1 ], distance_ );

        // Calculate (only runs when 4 points or more)...

        for ( std::size_t j = 0, l = points_.size ( ) - 3ULL; j < l; ++j ) {

            detail::centripetalCatmullRom ( points_ [ j ], points_ [ j + 1 ], points_ [ j + 2 ], points_ [ j + 3 ], poly );
            detail::pointsOnSegment ( return_value, poly, points_ [ j + 2 ], distance_ );
        }

        // Extrapolate back...

        detail::centripetalCatmullRom ( points_ [ points_.size ( ) - ( std::size_t ) 3 ], points_ [ points_.size ( ) - ( std::size_t ) 2 ], points_ [ points_.size ( ) - ( std::size_t ) 1 ], 2.0f * points_ [ points_.size ( ) - ( std::size_t ) 1 ] - points_ [ points_.size ( ) - ( std::size_t ) 2 ], poly );
        detail::pointsOnSegment ( return_value, poly, points_ [ points_.size ( ) - ( std::size_t ) 1 ], distance_ );

        return return_value;
    }
}
}

#if 0


sf::Vector2f PointOnCurve ( const float t, const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3 ) {

	const float t2 = t * t, t3 = t2 * t;

	return sf::Vector2f { 0.5f * ( ( 2.0f * p1.x ) + ( -p0.x + p2.x ) * t + ( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 + ( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 ), 0.5f * ( ( 2.0f * p1.y ) + ( -p0.y + p2.y ) * t + ( 2.0f * p0.y - 5.0f * p1.y + 4 * p2.y - p3.y ) * t2 + ( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) * t3 ) };
}

/*

out.x = 0.5f * ( ( 2.0f * p1.x ) +
	( -p0.x + p2.x ) * t1 +
	( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 +
	( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 );
out.y = 0.5f * ( ( 2.0f * p1.y ) +
	( -p0.y + p2.y ) * t1 +
	( 2.0f * p0.y - 5.0f * p1.y + 4 * p2.y - p3.y ) * t2 +
	( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) * t3 );

	*/


struct PoC {

	float cx0, cx1, cx2, cx3, cy0, cy1, cy2, cy3;

	PoC ( ) { }
	PoC ( const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3 ) :

		cx0 ( 2.0f * p1.x ),
		cx1 ( -p0.x + p2.x ),
		cx2 ( 2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x ),
		cx3 ( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ),
		cy0 ( 2.0f * p1.y ),
		cy1 ( -p0.y + p2.y ),
		cy2 ( 2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y ),
		cy3 ( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) {
	}

	sf::Vector2f evaluate ( const float t1 ) const {

		return sf::Vector2f { 0.5f * ( cx0 + ( cx1 + ( cx2 + cx3 * t1 ) * t1 ) * t1 ), 0.5f * ( cy0 + ( cy1 + ( cy2 + cy3 * t1 ) * t1 ) * t1 ) };
	}
};



CoordinatesVector catmullRom0 ( const Points &points_, const int number_of_points_per_interval_ ) {

	// Calculate Catmull Rom for a chain of points and return the combined curve. The
	// chains' first and last point are extrapolated from the second and 1-before-
	// last point, respectively...

	const int number_of_intervals = static_cast<int> ( points_.size ( ) ) - 1L; // -1 = -3 + 2 extrap.
	const float rec = 1.0f / number_of_points_per_interval_;

	PoC poly;

	CoordinatesVector r;

	r.reserve ( number_of_intervals * number_of_points_per_interval_ );

	if ( number_of_intervals == 1 ) { // 2 points...

		// Extrapolate both ends...

		poly = PoC ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], 2.0f * points_ [ 1 ] - points_ [ 0 ] );

		for ( int i = 0; i < number_of_points_per_interval_; i++ ) {

			r.emplace_back ( poly.evaluate ( rec * i ) );
		}
	}

	else { // 3 points or more...

		   // Extrapolate...

		poly = PoC ( 2.0f * points_ [ 0 ] - points_ [ 1 ], points_ [ 0 ], points_ [ 1 ], points_ [ 2 ] );

		for ( int i = 0; i < number_of_points_per_interval_; i++ ) {

			r.emplace_back ( poly.evaluate ( rec * i ) );
		}

		// Calculate (only runs when 4 points or more)...

		for ( int j = 0; j < points_.size ( ) - 3; j++ ) {

			poly = PoC ( points_ [ j ], points_ [ j + 1 ], points_ [ j + 2 ], points_ [ j + 3 ] );

			for ( int i = 0; i < number_of_points_per_interval_; i++ ) {

				r.emplace_back ( poly.evaluate ( rec * i ) );
			}
		}

		// Extrapolate...

		poly = PoC ( points_ [ number_of_intervals - 2 ], points_ [ number_of_intervals - 1 ], points_ [ number_of_intervals ], 2.0f * points_ [ number_of_intervals ] - points_ [ number_of_intervals - 1 ] );

		for ( int i = 0; i < number_of_points_per_interval_; i++ ) {

			r.emplace_back ( poly.evaluate ( rec * i ) );
		}
	}

	return r;
}

#endif
