
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

#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/random/detail/config.hpp>
#include <boost/random/detail/seed.hpp>
#include <boost/random/detail/seed_impl.hpp>


namespace sf {

namespace detail {

// const std::uint64_t v = 0x1AEC805299990163, y = 0xCDFB859A3DD0884B;
// 0x0CF3FD1B9997F637 0xAFC1530680179F87 - 0.0033298184
inline std::uint64_t xoroshiro_integer_hash ( std::uint64_t x ) {

    x = ( ( x >> 32 ) ^ x ) * std::uint64_t { 0x1AEC805299990163 };
    x = ( ( x >> 32 ) ^ x );
    return x;
}
}

/**
* This is a fixed-increment version of Java 8's SplittableRandom generator
* See http://dx.doi.org/10.1145/2714064.2660195 and
* http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html
*
* It is a very fast generator passing BigCrush, and it can be useful if
* for some reason one absolutely want 64 bits of state;
*
* c-code by Sebastiano Vigna: http://xoroshiro.di.unimi.it/SplitMix64.c
*/
class SplitMix64 {

    public:

    using result_type = std::uint64_t;

    // Required for old Boost.Random concept.
    static const bool has_fixed_range = true;
    static const std::uint64_t default_seed = std::uint64_t { 0x9E3779B97F4A7C15 };

    /**
    * Constructs a @c SplitMix64, using the default seed.
    */
    SplitMix64 ( ) {
        seed ( );
    }

    /**
    * Constructs a @c SplitMix64, seeding it with @c value.
    */
    BOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR ( SplitMix64,
        std::uint64_t, value ) {
        seed ( value );
    }

    /**
    * Constructs a @c SplitMix64, seeding it with values
    * produced by a call to @c seq.generate().
    */
    BOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR ( SplitMix64,
        SeedSeq, seq ) {
        seed ( seq );
    }

    /**
    * Constructs a @c SplitMix64 and seeds it with values taken
    * from the iterator range [first, last) and adjusts first to
    * point to the element after the last one used. If there are
    * not enough elements, throws @c std::invalid_argument.
    *
    * first and last must be input iterators.
    */
    template<class It>
    SplitMix64 ( It& first, It last ) {
        seed ( first, last );
    }

    // compiler-generated copy constructor and assignment operator are fine.

    /**
    * Calls seed(default_seed).
    */
    void seed ( ) {
        seed ( default_seed );
    }

    /**
    * Seeds a @c SplitMix64 using the supplied value. 'Hashes' @c value
    * using the bijection described by:
    *
    * std::uint64_t integer_hash(std::uint64_t x) {
    *
    *     x = ((x >> 32) ^ x) * 0xDE17C195AA959A81;
    *	   x = ((x >> 32) ^ x) * 0xDE17C195AA959A81;
    *     x = ((x >> 32) ^ x);
    *
    *	   return x;
    * }
    */
    BOOST_RANDOM_DETAIL_ARITHMETIC_SEED ( SplitMix64, std::uint64_t, value ) {
        _s [ 0 ] = detail::xoroshiro_integer_hash ( value );
    }

    /**
    * Seeds a @c SplitMix64 using values from a SeedSeq.
    */
    BOOST_RANDOM_DETAIL_SEED_SEQ_SEED ( SplitMix64, SeedSeq, seq ) {
        boost::random::detail::seed_array_int<64, 1, SeedSeq, std::uint64_t> ( seq, _s );
    }

    /**
    * seeds a @c SplitMix64 with values taken from the iterator
    * range [first, last) and adjusts @c first to point to the
    * element after the last one used.  If there are not enough
    * elements, throws @c std::invalid_argument.
    *
    * @c first and @c last must be input iterators.
    */
    template<class It>
    void seed ( It& first, It last ) {
        boost::random::detail::fill_array_int<64, 1, It, std::uint64_t> ( first, last, _s );
    }

    /**
    * Returns the smallest value that the @c SplitMix64
    * can produce.
    */
    static result_type min BOOST_PREVENT_MACRO_SUBSTITUTION ( ) {
        return 0;
    }

    /**
    * Returns the largest value that the @c SplitMix64
    * can produce.
    */
    static result_type max BOOST_PREVENT_MACRO_SUBSTITUTION ( ) {
        return UINT64_MAX;
    }

    /** Returns the next value of the @c SplitMix64. */
    std::uint64_t operator()( ) {
        return hash ( next ( ) );
    }

    /** Fills a range with random values. */
    template<class Iter>
    void generate ( Iter first, Iter last ) {
        boost::random::detail::generate_from_int ( *this, first, last );
    }

    /** Advances the state of the generator by @c z. */
    void discard ( boost::uintmax_t z ) {
        // This seems to be the fastest way (release),
        // as opposed to anything more fancy.
        while ( z-- ) {
            next ( );
        }
    }

    friend bool operator==( const SplitMix64& x,
        const SplitMix64& y ) {
        return x._s [ 0 ] == y._s [ 0 ];
    }

    friend bool operator!=( const SplitMix64& x,
        const SplitMix64& y ) {
        return !( x == y );
    }

    /** Writes a @c SplitMix64 to a @c std::ostream. */
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
        operator<<( std::basic_ostream<CharT, Traits>& os,
            const SplitMix64& sm64 ) {
        os << sm64._s [ 0 ];
        return os;
    }

    /** Reads a @c SplitMix64 from a @c std::istream. */
    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>&
        operator >> ( std::basic_istream<CharT, Traits>& is,
            SplitMix64& sm64 ) {
        is >> sm64._s [ 0 ];
        return is;
    }

    private:

    /// \cond show_private

    std::uint64_t next ( ) {
        return ( _s [ 0 ] += std::uint64_t { 0x9E3779B97F4A7C15 } );
    }

    static std::uint64_t hash ( std::uint64_t z ) {
        z = ( z ^ ( z >> 30 ) ) * std::uint64_t { 0xBF58476D1CE4E5B9 };
        z = ( z ^ ( z >> 27 ) ) * std::uint64_t { 0x94D049BB133111EB };
        return z ^ ( z >> 31 );
    }

    /// \endcond

    std::uint64_t _s [ 1 ];
};
}
