
#include <autotimer.hpp>


#include <cassert>
#include <ciso646>
#include <cmath>

#include <array>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream.hpp> // <iostream> + nl, sp etc. defined...
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Extensions.hpp>

// Pong sounds: http://cs.au.dk/~dsound/DigitalAudio.dir/Greenfoot/Pong.dir/Pong.html

#include <autotimer.hpp>

#include "../../xoroshiro128plus/xoroshiro128plus/xoroshiro.hpp"
#include <integer_utils.hpp>

#include "wapi.h"

int main436547687 ( ) {

    sf::Pacer pac;

    pac.pace ( );
    pac.pace ( );
    pac.pace ( );

    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency ( &Frequency );
    QueryPerformanceCounter ( &StartingTime );

    // Activity to be timed

    {
        at::AutoTimer t;
        pac.pace ( );
    }

    QueryPerformanceCounter ( &EndingTime );
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

    //
    // We now have the elapsed number of ticks, along with the
    // number of ticks-per-second. We use these values
    // to convert to the number of elapsed microseconds.
    // To guard against loss-of-precision, we convert
    // to microseconds *before* dividing by ticks-per-second.
    //

    ElapsedMicroseconds.QuadPart *= 1'000'000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    std::cout << ElapsedMicroseconds.QuadPart << nl;

    return 0;
}


int main234568 ( ) {

    sf::Clock clo;
    sf::Pacer pac;

    sf::HrTimePoint time;

    std::cout << pac.frequency ( ) << nl;

    // exit ( 0 );

    for (;;) {

        const sf::HrTimePoint t = pac.pace ( );

        std::cout << ( t - time ).count ( ) << nl;

        time = t;

        // std::cout << pac.now ( ).count ( ) << nl;
    }

    return 0;
}

class Logger {

    Logger ( ) = default;

    public:

    static Logger & instance;

    static void log ( const std::string & s ) noexcept {

        std::cout << s << std::endl;
    }
};

Logger & Logger::instance = * ( new Logger );


template <typename T>
class opt_ref {
    T* _ptr = nullptr;

    public:
    opt_ref ( ) noexcept = default;
    opt_ref ( T& reference ) noexcept
        : _ptr ( std::addressof ( reference ) ) { }
    opt_ref ( std::nullopt_t ) noexcept
        : _ptr ( nullptr ) { }

    explicit operator bool ( ) const noexcept { return _ptr != nullptr; }

    T& operator*( ) noexcept {
        assert ( _ptr != nullptr && "Dereferencing null opt_ref" );
        return *_ptr;
    }

    T* operator->( ) noexcept {
        assert ( _ptr != nullptr && "Dereferencing null opt_ref" );
        return _ptr;
    }
};



int main23456 ( ) {

    sf::Path p1 = sf::setAppDataPath ( "test" );

    std::cout << p1.string ( ) << nl;

    sf::Path p2 = sf::getAppDataPath ( );

    std::cout << p2.string ( ) << nl;

    return 0;
}


int main243654765876987 ( ) {

    std::shared_ptr<sf::RenderWindow> w ( new sf::RenderWindow );

    w->setVerticalSyncEnabled ( true );
    w->create ( sf::VideoMode ( 1'280, 720 ), "Window" );

    sf::ParticleSystem particle_0 ( 50'000 ), particle_1 ( 50'000 ), particle_2 ( 50'000 );

    sf::Clock clock;
    sf::Time elapsed;
    sf::SplitMix64 rng;

    auto dis1 = std::uniform_int_distribution<int>   ( 64   ,    192    );
    auto dis2 = std::uniform_real_distribution<float> ( 0.0f,    740.0f );
    auto dis3 = std::uniform_real_distribution<float> ( 0.0f,  1'350.0f );
    auto dis4 = std::uniform_real_distribution<float> ( 0.0f,  7'500.0f );
    auto dis5 = std::uniform_real_distribution<float> ( 0.0f, 13'540.0f );

    while ( w->isOpen ( ) ) {

        sf::Event e;

        while ( w->pollEvent ( e ) ) {

            if ( e.type == sf::Event::Closed ) {

                w->close ( );
            }
        }

        if ( 1 == sf::Keyboard::isKeyPressed ( sf::Keyboard::Escape ) ) {

            w->close ( );
        }

        w->clear ( sf::Color::Black );

        elapsed = clock.restart ( );

        particle_0.update ( elapsed, sf::Color ( dis1 ( rng ), dis1 ( rng ), dis1 ( rng ) ) );
        particle_0.emitter = sf::Vector2f ( dis3 ( rng ), dis2 ( rng ) );
        w->draw ( particle_0 );

        particle_1.update ( elapsed, sf::Color ( dis1 ( rng ), dis1 ( rng ), dis1 ( rng ) ) );
        particle_1.emitter = sf::Vector2f ( dis5 ( rng ), dis2 ( rng ) );
        w->draw ( particle_1 );

        particle_2.update ( elapsed, sf::Color ( dis1 ( rng ), dis1 ( rng ), dis1 ( rng ) ) );
        particle_2.emitter = sf::Vector2f ( dis4 ( rng ), dis2 ( rng ) );
        w->draw ( particle_2 );

        w->display ( );
    }

    return 0;
}


LARGE_INTEGER g_frequency;
const double kDelayTime = 1.0;

double GetTime ( ) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter ( &counter );
    return counter.QuadPart / double ( g_frequency.QuadPart );
}

int g_array [ 1024 ];
int offset;
int g_sum;

void SpinABit ( ) {
    for ( int i = 0; i < ARRAYSIZE ( g_array ); ++i ) {
        g_sum += g_array [ i + offset ];
    }
}

void Stall ( ) {
    double start = GetTime ( );
    int iterations = 0;
    for ( ;;) {
        ++iterations;
        SpinABit ( );
        double elapsed = GetTime ( ) - start;
        if ( elapsed >= kDelayTime ) {
            printf ( "%1.5e iterations/s\n", iterations / elapsed );
            return;
        }
    }
}

int main325465476576 ( int argc, char* argv [ ] ) {
    QueryPerformanceFrequency ( &g_frequency );
    for ( ;;)
        Stall ( );
    return 0;
}


class player : public sf::Fsm<player> {

    friend class Fsm;  // base class needs access to transition_table

    public:

    enum states { Stopped, Open, Empty, Playing, Paused };

    player ( StateType init_state = Empty ) : Fsm ( init_state ) { }

    struct play { };
    struct open_close { };
    struct cd_detected {
        cd_detected ( char const* s = "" ) : title ( s ) { }
        std::string title;
    };
    struct stop { };
    struct pause { };

    private:
    void start_playback ( play const& );
    void open_drawer ( open_close const& );
    void close_drawer ( open_close const& );
    void store_cd_info ( cd_detected const& cd );
    void stop_playback ( stop const& );
    void pause_playback ( pause const& );
    void stop_and_open ( open_close const& );
    void resume_playback ( play const& );

    private:
    using TransitionTable = Table<
        //              Start    Event        Target   Action
        //  -----------+--------+------------+--------+------------------------+-
        Transition< Stopped, play, Playing, &player::start_playback >,
        Transition< Stopped, open_close, Open, &player::open_drawer >,
        Transition< Open, open_close, Empty, &player::close_drawer >,
        Transition< Empty, open_close, Open, &player::open_drawer >,
        Transition< Empty, cd_detected, Stopped, &player::store_cd_info >,
        Transition< Playing, stop, Stopped, &player::stop_playback >,
        Transition< Playing, pause, Paused, &player::pause_playback >,
        Transition< Playing, open_close, Open, &player::stop_and_open >,
        Transition< Paused, play, Playing, &player::resume_playback >,
        Transition< Paused, stop, Stopped, &player::stop_playback >,
        Transition< Paused, open_close, Open, &player::stop_and_open >
        //  -----------+--------+------------+--------+------------------------+-
    >;
};

void player::start_playback ( play const& ) {
    std::cout << "Starting playback\n";
}

void player::open_drawer ( open_close const& ) {
    std::cout << "Opening drawer\n";
}

void player::close_drawer ( open_close const& ) {
    std::cout << "Closing drawer\n";
}

void player::store_cd_info ( cd_detected const& cd ) {
    std::cout << "Detected CD '" << cd.title << "'\n";
}

void player::stop_playback ( stop const& ) {
    std::cout << "Stopping playback\n";
}

void player::pause_playback ( pause const& ) {
    std::cout << "Pausing playback\n";
}

void player::stop_and_open ( open_close const& ) {
    std::cout << "Stopping and opening drawer\n";
}

void player::resume_playback ( play const& ) {
    std::cout << "Resuming playback\n";
}

int main234678 ( ) {
    player p;

    p.processEvent ( player::open_close ( ) );
    assert ( p.currentState ( ) == player::Open );
    p.processEvent ( player::open_close ( ) );
    assert ( p.currentState ( ) == player::Empty );
    p.processEvent ( player::cd_detected ( "Rubber Soul" ) );
    assert ( p.currentState ( ) == player::Stopped );
    p.processEvent ( player::play ( ) );
    assert ( p.currentState ( ) == player::Playing );
    p.processEvent ( player::pause ( ) );
    assert ( p.currentState ( ) == player::Paused );
    p.processEvent ( player::open_close ( ) );
    assert ( p.currentState ( ) == player::Open );
    p.processEvent ( player::open_close ( ) );
    assert ( p.currentState ( ) == player::Empty );

    return 0;
}



// basic hfsm sample

#include <iostream>
#include <hfsm/fsm.hpp>


// custom states (gerunds) and actions (infinitives)

enum {
    walking = 'WALK',
    defending = 'DEFN',
    sleeping = 'SLPN',

    tick = 'tick',
};

struct ant_t {
    fsm::stack fsm;
    int health, distance, flow, sleep;

    ant_t ( ) : health ( 0 ), distance ( 0 ), flow ( 1 ), sleep ( 0 ) {
        // define fsm transitions: on(state,trigger) -> do lambda
        fsm.on ( walking, 'init' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "initializing" << std::endl;
        };
        fsm.on ( walking, 'exit' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "exiting" << std::endl;
        };
        fsm.on ( walking, 'push' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "pushing current task." << std::endl;
        };
        fsm.on ( walking, 'back' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "back from another task. remaining distance: " << distance << std::endl;
        };
        fsm.on ( walking, tick ) = [ & ] ( const fsm::args &args ) {
            std::cout << "\r" << "\\|/-" [ distance % 4 ] << " walking " << ( flow > 0 ? "-->" : "<--" ) << " ";
            distance += flow;
            if ( 1000 == distance ) {
                std::cout << "at food!" << std::endl;
                flow = -flow;
            }
            if ( -1000 == distance ) {
                std::cout << "at home!" << std::endl;
                flow = -flow;
            }
        };
        fsm.on ( defending, 'init' ) = [ & ] ( const fsm::args &args ) {
            health = 1000;
            std::cout << "somebody is attacking me! he has " << health << " health points" << std::endl;
        };
        fsm.on ( defending, tick ) = [ & ] ( const fsm::args &args ) {
            std::cout << "\r" << "\\|/-$" [ health % 4 ] << " health: (" << health << ")   ";
            --health;
            if ( health < 0 ) {
                std::cout << std::endl;
                fsm.pop ( );
            }
        };
        fsm.on ( defending, 'exit' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "regrouping and carry on" << std::endl;
        };

        fsm.on ( sleeping, 'init' ) = [ & ] ( const fsm::args &args ) {
            sleep = 10;
            std::cout << "going to sleep for " << sleep << " ticks" << std::endl;
        };
        fsm.on ( sleeping, tick ) = [ & ] ( const fsm::args &args ) {
            std::cout << "z";
            --sleep;
            if ( sleep < 0 ) {
                std::cout << "..." << std::endl;
                fsm.pop ( );
            }
        };
        fsm.on ( sleeping, 'exit' ) = [ & ] ( const fsm::args &args ) {
            std::cout << "just woke up" << std::endl;
        };

        // set initial fsm state
        fsm.set ( walking );
    }
};

int main ( ) {

    sf::SplitMix64 rng;

    ant_t ant;

    for ( int i = 0; i < 120000; ++i ) {

        if ( std::bernoulli_distribution ( 1.0 / 1'000.0 ) ( rng ) ) {

            ant.fsm.push ( defending );
        }

        if ( std::bernoulli_distribution ( 1.0 / 1'000.0 ) ( rng ) ) {

            ant.fsm.push ( sleeping );
        }

        ant.fsm.command ( tick );
    }

    return 0;
}


enum {
    opening,
    closing,
    waiting,
    playing,

    open,
    close,
    play,
    stop,
    insert,
    eject,
};

struct cd_player {

    // implementation variables
    bool has_cd;

    // implementation conditions / guards
    bool good_disk_format ( ) { return true; }

    // implementation actions
    void open_tray ( ) { std::cout << "opening tray" << std::endl; }
    void close_tray ( ) { std::cout << "closing tray" << std::endl; }
    void get_cd_info ( ) { std::cout << "retrieving CD info" << std::endl; }
    void start_playback ( const int track ) { std::cout << "playing track #" << track << std::endl; }

    // the core
    fsm::stack fsm;

    cd_player ( ) : has_cd ( false ) {
        // define fsm transitions: on(state,trigger) -> do lambda
        fsm.on ( opening, close ) = [ & ] ( const fsm::args &args ) {
            close_tray ( );
            if ( !has_cd ) {
                fsm.set ( closing );
            }
            else {
                get_cd_info ( );
                fsm.set ( waiting );
            }
        };
        fsm.on ( opening, insert ) = [ & ] ( const fsm::args &args ) {
            has_cd = true;
            fsm.set ( opening );
        };
        fsm.on ( opening, eject ) = [ & ] ( const fsm::args &args ) {
            has_cd = false;
            fsm.set ( opening );
        };

        fsm.on ( closing, open ) = [ & ] ( const fsm::args &args ) {
            open_tray ( );
            fsm.set ( opening );
        };

        fsm.on ( waiting, play ) = [ & ] ( const fsm::args &args ) {
            if ( !good_disk_format ( ) ) {
                fsm.set ( waiting );
            }
            else {
                start_playback ( args [ 0 ] );
                fsm.set ( playing );
            }
        };
        fsm.on ( waiting, open ) = [ & ] ( const fsm::args &args ) {
            open_tray ( );
            fsm.set ( opening );
        };

        fsm.on ( playing, open ) = [ & ] ( const fsm::args &args ) {
            open_tray ( );
            fsm.set ( opening );
        };
        fsm.on ( playing, stop ) = [ & ] ( const fsm::args &args ) {
            fsm.set ( waiting );
        };

        // set initial fsm state
        fsm.set ( waiting );
    }
};

// usage

int main232456 ( ) {

    cd_player cd;

    for (;;) {

        std::cout << "[" << cd.fsm.get_state ( ) << "] ";
        std::cout << "(o)pen lid/(c)lose lid, (i)nsert cd/(e)ject cd, (p)lay/(s)top cd? ";

        char cmd;
        std::cin >> cmd;

        switch ( cmd ) {

            case 'p': cd.fsm.command ( play, 1 + rand ( ) % 10 ); break;
            case 'o': cd.fsm.command ( open ); break;
            case 'c': cd.fsm.command ( close ); break;
            case 's': cd.fsm.command ( stop ); break;
            case 'i': cd.fsm.command ( insert ); break;
            case 'e': cd.fsm.command ( eject ); break;
            default: std::cout << "what?" << std::endl;
        }
    }

    return 0;
}
