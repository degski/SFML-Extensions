
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

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_BOX_HPP
#define SFML_BOX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating 2D axis aligned rectangles
///
////////////////////////////////////////////////////////////
template <typename T>
class Box
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty rectangle (it is equivalent to calling
    /// Box(0, 0, 0, 0)).
    ///
    ////////////////////////////////////////////////////////////
    Box();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from its coordinates
    ///
    /// Be careful, the last two parameters are the width
    /// and height, not the right and bottom coordinates!
    ///
    /// \param boxLeft   Left coordinate of the rectangle
    /// \param boxTop    Top coordinate of the rectangle
    /// \param boxRight  Right coordinate of the rectangle
    /// \param boxBottom Bottom coordinate of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Box(T boxLeft, T boxTop, T boxRight, T boxBottom);

    ////////////////////////////////////////////////////////////

    Box(const Rect<T>& rect);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from position and size
    ///
    /// Be careful, the last parameter is the size,
    /// not the bottom-right corner!
    ///
    /// \param position Position of the top-left corner of the rectangle
    /// \param size     Size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Box(const Vector2<T>& position, const Vector2<T>& size);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from another type of rectangle
    ///
    /// This constructor doesn't replace the copy constructor,
    /// it's called only when U != T.
    /// A call to this constructor will fail to compile if U
    /// is not convertible to T.
    ///
    /// \param rectangle Rectangle to convert
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    explicit Box(const Box<U>& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check is non-inclusive. If the point lies on the
    /// edge of the rectangle, this function will return false.
    ///
    /// \param x X coordinate of the point to test
    /// \param y Y coordinate of the point to test
    ///
    /// \return True if the point is inside, false otherwise
    ///
    /// \see intersects
    ///
    ////////////////////////////////////////////////////////////
    bool contains(T x, T y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check is non-inclusive. If the point lies on the
    /// edge of the rectangle, this function will return false.
    ///
    /// \param point Point to test
    ///
    /// \return True if the point is inside, false otherwise
    ///
    /// \see intersects
    ///
    ////////////////////////////////////////////////////////////
    bool contains(const Vector2<T>& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check the intersection between two rectangles
    ///
    /// \param rectangle Rectangle to test
    ///
    /// \return True if rectangles overlap, false otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    bool intersects(const Box<T>& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check the intersection between two rectangles
    ///
    /// This overload returns the overlapped rectangle in the
    /// \a intersection parameter.
    ///
    /// \param rectangle    Rectangle to test
    /// \param intersection Rectangle to be filled with the intersection
    ///
    /// \return True if rectangles overlap, false otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    bool intersects(const Box<T>& rectangle, Box<T>& intersection) const;

    ////////////////////////////////////////////////////////////

    Rect<T> getRect ( ) const noexcept;

    ////////////////////////////////////////////////////////////

    sf::Vector2<T> getSize ( ) const noexcept;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T left;   ///< Left coordinate of the rectangle
    T top;    ///< Top coordinate of the rectangle
    T right;  ///< Right coordinate of the rectangle
    T bottom; ///< Bottom coordinate of the rectangle
};

////////////////////////////////////////////////////////////
/// \relates Box
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator ==(const Box<T>& left, const Box<T>& right);

////////////////////////////////////////////////////////////
/// \relates Box
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator !=(const Box<T>& left, const Box<T>& right);

#include "Box.inl"

// Create typedefs for the most common types
typedef Box<int>   IntBox;
typedef Box<float> FloatBox;

template <typename T>
Rect<T> boxToRect ( const Box<T> box ) noexcept
{
    return Rect<T> ( box.left, box.top, box.right-box.left, box.bottom-box.top );
}

template <typename T>
Box<T> rectToBox ( const Rect<T> rect ) noexcept
{
    return Rect<T> ( rect.left, rect.top, rect.left+rect.width, rect.top+rect.height );
}


} // namespace sf


#endif // SFML_BOX_HPP
