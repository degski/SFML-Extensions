
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


////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box() :
left  (0),
top   (0),
right (0),
bottom(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box(T boxLeft, T boxTop, T boxRight, T boxBottom) :
left  (boxLeft),
top   (boxTop),
right (boxRight),
bottom(boxBottom)
{

}



////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box( const Rect<T>& rect ) :
left ( rect.left ),
top ( rect.top ),
right (rect.left+rect.width),
bottom (rect.top+rect.height)
{

}



////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box(const Vector2<T>& position, const Vector2<T>& size) :
left  (position.x),
top   (position.y),
right (position.x+size.x),
bottom(position.y+size.y)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Box<T>::Box(const Box<U>& rectangle) :
left  (static_cast<T>(rectangle.left)),
top   (static_cast<T>(rectangle.top)),
right (static_cast<T>(rectangle.right)),
bottom(static_cast<T>(rectangle.bottom))
{
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::contains(T x, T y) const
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    return x >= left && x <= right && y <= top && y >= bottom;

}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::contains(const Vector2<T>& point) const
{
    return contains(point.x, point.y);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::intersects(const Box<T>& rectangle) const
{
    Box<T> intersection;
    return intersects(rectangle, intersection);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::intersects(const Box<T>& rectangle, Box<T>& intersection) const
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    T r1MinX = std::min ( left, right );
    T r1MaxX = std::max ( left, right );
    T r1MinY = std::min ( top, bottom );
    T r1MaxY = std::max ( top, bottom );

    // Compute the min and max of the second rectangle on both axes
    T r2MinX = std::min ( rectangle.left, rectangle.right );
    T r2MaxX = std::max ( rectangle.left, rectangle.right );
    T r2MinY = std::min ( rectangle.top, rectangle.bottom );
    T r2MaxY = std::max ( rectangle.top, rectangle.bottom );

    // Compute the intersection boundaries
    T interLeft = std::max ( r1MinX, r2MinX );
    T interTop = std::max ( r1MinY, r2MinY );
    T interRight = std::min ( r1MaxX, r2MaxX );
    T interBottom = std::min ( r1MaxY, r2MaxY );

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ( ( interLeft < interRight ) && ( interTop < interBottom ) ) {
        intersection = Box<T> ( interLeft, interTop, interRight, interBottom );
        return true;
    }
    else {
        intersection = Box<T> ( 0, 0, 0, 0 );
        return false;
    }
}


////////////////////////////////////////////////////////////
template <typename T>
sf::Rect<T> Box<T>::getRect ( ) const noexcept
{
    return Rect<T> ( left, top, right - left, bottom - top );
}


////////////////////////////////////////////////////////////
template <typename T>
sf::Vector2<T> Box<T>::getSize ( ) const noexcept
{
    return Vector2<T> { right - left, bottom - top };
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Box<T>& left, const Box<T>& right)
{
    return (left.left == right.left) && (left.right == right.right) &&
           (left.top == right.top) && (left.bottom == right.bottom);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Box<T>& left, const Box<T>& right)
{
    return !(left == right);
}
