////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
inline Vector4<T>::Vector4() :
v0(0),
v1(0),
v2(0),
v3(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>::Vector4(T V0, T V1, T V2, T V3) :
v0(V0),
v1(V1),
v2(V2),
v3(V3)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Vector4<T>::Vector4(const Vector4<U>& vector) :
v0(static_cast<T>(vector.v0)),
v1(static_cast<T>(vector.v1)),
v2(static_cast<T>(vector.v2)),
v3(static_cast<T>(vector.v3))
{
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator -(const Vector4<T>& left)
{
    return Vector4<T>(-left.v0, -left.v1, -left.v2, -left.v3);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator +=(Vector4<T>& left, const Vector4<T>& right)
{
	left.v0 += right.v0;
    left.v1 += right.v1;
    left.v2 += right.v2;
    left.v3 += right.v3;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator -=(Vector4<T>& left, const Vector4<T>& right)
{
	left.v0 -= right.v0;
    left.v1 -= right.v1;
    left.v2 -= right.v2;
    left.v3 -= right.v3;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator +(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.v0 + right.v0, left.v1 + right.v1, left.v2 + right.v2, left.v3 + right.v3);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator -(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.v0 - right.v0, left.v1 - right.v1, left.v2 - right.v2, left.v3 - right.v3);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator *(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.v0 * right, left.v1 * right, left.v2 * right, left.v3 * right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator *(T left, const Vector4<T>& right)
{
     return Vector4<T>(left * right.v0, left * right.v1, left * right.v2, left * right.v3);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator *=(Vector4<T>& left, T right)
{
	left.v0 *= right;
    left.v1 *= right;
    left.v2 *= right;
    left.v3 *= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator /(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.v0 / right, left.v1 / right, left.v2 / right, left.v3 / right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator /=(Vector4<T>& left, T right)
{
	left.v0 /= right;
    left.v1 /= right;
    left.v2 /= right;
    left.v3 /= right;
 
    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.v0 == right.v0) && (left.v1 == right.v1) && (left.v2 == right.v2) && (left.v3 == right.v3);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.v0 != right.v0) && (left.v1 != right.v1) && (left.v2 != right.v2) && (left.v3 != right.v3);
}
