
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

// #include <algorithm> // std::clamp
#include <iostream>
#include <random>

#include "Extensions/Extensions.hpp"
#include "Extensions/ParticleSystem.hpp"

#include <sax/prng.hpp>
#include <sax/uniform_int_distribution.hpp>

namespace sf {

Particle::Particle ( ) noexcept {
    randomize ( );
}

void Particle::randomize ( ) noexcept {
    static sax::Rng rng;
    const float angle = std::uniform_real_distribution<float> ( 0.0f, two_pi ) ( rng );
    const float speed = ( float ) sax::uniform_int_distribution<Int32> ( 50, 100 ) ( rng );
    velocity.x = std::cos ( angle ) * speed;
    velocity.y = std::sin ( angle ) * speed;
    lifetime = sf::milliseconds ( sax::uniform_int_distribution<Int32> ( 1'000, 3'000 ) ( rng ) );
}

ParticleSystem::ParticleSystem ( const Uint32 count_, const IntInterval speed_, const IntInterval lifetime_ ) :
    speed ( speed_ ), lifetime ( lifetime_ ),
    emitter ( 0.0f, 0.0f ),
    m_particles ( count_ ),
    m_vertices ( sf::Points, count_ ),
    m_lifetimes ( sf::seconds ( 4.0f ) ) {
}

void ParticleSystem::update ( const Time elapsed_, Color color_/* = sf::Color::White*/ ) {
    for ( std::size_t i = std::size_t { 0 }, l = m_particles.size ( ); i < l; ++i ) {
        // Update the particle lifetime.
        Particle & p = m_particles [ i ];
        p.lifetime -= elapsed_;
        // If the particle is dead, respawn it.
        if ( p.lifetime <= sf::Time::Zero )
            resetParticle ( i );
        // Update the position of the corresponding vertex.
        m_vertices [ i ].position += p.velocity * elapsed_.asSeconds ( );
        // Update the alpha (transparency) of the particle according to its lifetime.
        // color_.a = ( Uint8 ) std::clamp ( p.lifetime.asSeconds ( ) / m_lifetimes.asSeconds ( ), 0.0f, 1.0f ) * 255.0f;
        m_vertices [ i ].color = color_;
    }
}

void ParticleSystem::resetParticle ( const std::size_t index_ ) {
    // Give a random velocity and lifetime to the particle.
    m_particles [ index_ ].randomize ( );
    // Reset the position of the corresponding vertex.
    m_vertices [ index_ ].position = emitter;
}

}  // namespace sf
