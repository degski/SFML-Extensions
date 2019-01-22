
#include <ciso646>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream.hpp> // <iostream> + nl, sp etc. defined...
#include <iterator>
#include <list>
#include <map>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include <limits>
#include <iostream>
#include <typeinfo>

template <typename C>
inline C channel_invert1 ( C x ) {
    return std::numeric_limits<C>::max ( ) - x + std::numeric_limits<C>::min ( );
}

template <typename C>
inline C channel_invert2 ( C x ) {
    return ( x - std::numeric_limits<C>::max ( ) ) * ( -1 ) +
        std::numeric_limits<C>::min ( );
}

int main ( ) {

    int x = std::numeric_limits<int>::min ( );
    std::cout << x << std::endl;

    // plain expressions
    int x_invert1 = std::numeric_limits<int>::max ( ) - x +
        std::numeric_limits<int>::min ( );
    int x_invert2 = ( x - std::numeric_limits<int>::max ( ) ) * ( -1 ) +
        std::numeric_limits<int>::min ( );
    std::cout << x_invert1 << std::endl;
    std::cout << x_invert2 << std::endl;

    // the same expressions wrapped in function template
    std::cout << channel_invert1<int> ( x ) << std::endl;
    std::cout << channel_invert2<int> ( x ) << std::endl;
}
