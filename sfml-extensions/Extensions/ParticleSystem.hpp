
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

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>


namespace sf {

using IntInterval = Vector2i;

struct Particle {

    Vector2f velocity;
    Time lifetime;

    Particle ( ) noexcept;
    void randomize ( ) noexcept;
};


class ParticleSystem : public Drawable, public Transformable, public Vertex, Color {

    public:

    ParticleSystem ( const Uint32 count_, const IntInterval speed_ = IntInterval { 50, 100 }, const IntInterval lifetime_ = IntInterval { 1'000, 3'000 } );

    void update ( const Time elapsed, const Color Color/* = sf::Color::White*/ );

    private:

    virtual void draw ( RenderTarget & target_, RenderStates states_ ) const {

        // Apply the transform...

        states_.transform *= getTransform ( );

        // Our particles don't use a texture...

        states_.texture = NULL;

        // Draw the vertex array...

        target_.draw ( m_vertices, states_ );
    }

    private:

    void resetParticle ( const std::size_t index );

    public:

    IntInterval speed, lifetime;
    Vector2f emitter;

    private:

    std::vector<Particle> m_particles;
    VertexArray m_vertices;
    Time m_lifetimes;
};
}
