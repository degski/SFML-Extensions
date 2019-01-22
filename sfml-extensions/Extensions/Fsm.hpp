/*
* Copyright (c) 2015, 2016 Thomas Kemmer
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma

#include <type_traits>

#include "Extensions.hpp"


namespace sf {
namespace detail {
// from C++17
template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template<class T>
using result_of_t = typename std::result_of<T>::type;

// similar to std::integral_constant, but without const
template<class T>
struct typed_value {
    using type = T;
    static T value;
};

template<class T>
T typed_value<T>::value;

// similar to std::is_constructible
struct is_callable_test {
    struct no_type { int a; int b; };

    template<class T, class... Args, class = result_of_t<T ( Args... )>>
    static char test ( int );

    template<class, class...>
    static no_type test ( ... );
};

template<class T, class... Args>
using is_callable = typename std::conditional<
    std::is_function<T>::value,
    bool_constant<sizeof ( is_callable_test::test<T*, Args...> ( 0 ) ) == 1>,
    bool_constant<sizeof ( is_callable_test::test<T, Args...> ( 0 ) ) == 1>
>::type;

// basic mpl-like stuff
template<class...> struct list {
    using type = list;  // self-referential
};

template<class...> struct concat;

template<class T, class... Types>
struct concat<T, list<Types...>> {
    using type = list<T, Types...>;
};

template<>
struct concat<> {
    using type = list<>;
};

template<template<typename> class Predicate, class...> struct filter;

template<template<typename> class Predicate, class T, class... Types>
struct filter<Predicate, T, Types...> {
    using type = typename std::conditional<
        Predicate<T>::value,
        typename concat<T, typename filter<Predicate, Types...>::type>::type,
        typename filter<Predicate, Types...>::type
    >::type;
};

template<template<typename> class Predicate>
struct filter<Predicate> {
    using type = list<>;
};

// use any of fn(), fn(arg1), fn(arg2), fn(arg1, arg2)
template<
    class T, T* fn, class Arg1, class Arg2,
    bool f1 = is_callable<T>::value,
    bool f2 = is_callable<T, Arg1>::value,
    bool f3 = is_callable<T, Arg2>::value,
    bool f4 = is_callable<T, Arg1, Arg2>::value
> struct make_binary_function;

template<class T, T* fn, class Arg1, class Arg2>
struct make_binary_function<T, fn, Arg1, Arg2, true, false, false, false> {
    static struct type {
        result_of_t<T ( )> operator()( Arg1, Arg2 ) const {
            return ( *fn )( );
        }
    } value;
};

template<class T, T* fn, class Arg1, class Arg2>
typename make_binary_function<T, fn, Arg1, Arg2, true, false, false, false>::type
make_binary_function<T, fn, Arg1, Arg2, true, false, false, false>::value;

template<class T, T* fn, class Arg1, class Arg2>
struct make_binary_function<T, fn, Arg1, Arg2, false, true, false, false> {
    static struct type {
        result_of_t<T ( Arg1 )> operator()( Arg1 arg1, Arg2 ) const {
            return ( *fn )( arg1 );
        }
    } value;
};

template<class T, T* fn, class Arg1, class Arg2>
typename make_binary_function<T, fn, Arg1, Arg2, false, true, false, false>::type
make_binary_function<T, fn, Arg1, Arg2, false, true, false, false>::value;

template<class T, T* fn, class Arg1, class Arg2>
struct make_binary_function<T, fn, Arg1, Arg2, false, false, true, false> {
    static struct type {
        result_of_t<T ( Arg2 )> operator()( Arg1, Arg2 arg2 ) const {
            return ( *fn )( arg2 );
        }
    } value;
};

template<class T, T* fn, class Arg1, class Arg2>
typename make_binary_function<T, fn, Arg1, Arg2, false, false, true, false>::type
make_binary_function<T, fn, Arg1, Arg2, false, false, true, false>::value;

template<class T, T* fn, class Arg1, class Arg2>
struct make_binary_function<T, fn, Arg1, Arg2, false, false, false, true> {
    static struct type {
        result_of_t<T ( Arg1, Arg2 )> operator()( Arg1 arg1, Arg2 arg2 ) const {
            return ( *fn )( arg1, arg2 );
        }
    } value;
};

template<class T, T* fn, class Arg1, class Arg2>
typename make_binary_function<T, fn, Arg1, Arg2, false, false, false, true>::type
make_binary_function<T, fn, Arg1, Arg2, false, false, false, true>::value;
}

/**
* Finite state machine (FSM) base class template.
*
* @tparam Derived the derived state machine class
*
* @tparam State the FSM's state type, defaults to `int`
*/
template<class Derived, class State = Int32>
class Fsm {
    public:
    /**
    * The FSM's state type.
    */
    typedef State StateType;

    public:
    /**
    * Create a state machine with default initial state.
    */
    Fsm ( ) : m_state ( ) { }

    /**
    * Create a state machine with a custom initial state.
    *
    * @param init_state the FSM's initial state
    */
    Fsm ( StateType init_state ) : m_state ( init_state ) { }

    /**
    * Process an event.
    *
    * @tparam Event the event tyoe
    *
    * @param event the event instance
    */
    template<class Event>
    void processEvent ( Event const& event ) {
        static_assert( std::is_base_of<Fsm, Derived>::value, "must derive from Fsm" );
        Derived* self = static_cast<Derived*>( this );
        m_state = Derived::TransitionTable::execute ( *self, event, m_state );
    }

    /**
    * Return the state machine's current state.
    */
    StateType currentState ( ) const { return m_state; }

    protected:
    /**
    * Called when no transition can be found for the given event
    * in the current state.  Derived state machines may override
    * this to throw an exception, or change to some other (error)
    * state.  The default is to return the current state, so no
    * state change occurs.
    *
    * @tparam Event the event type
    *
    * @param event the event instance
    *
    * @return the FSM's new state
    */
    template<class Event>
    StateType no_transition ( Event const& event ) {
        return m_state;
    }

    private:
    template<class Event>
    struct no_action_type {
        void operator()( Derived&, Event const& ) const { }
    };

    template<class Event>
    struct no_guard_type {
        bool operator()( Derived const&, Event const& ) const {
            return true;
        }
    };

    template<class Event, void ( Derived::*action )( Event const& )>
    struct mem_fn_action_type {
        void operator()( Derived& self, Event const& event ) const {
            ( self.*action )( event );
        }
    };

    template<class Event, bool ( Derived::*guard )( Event const& ) const>
    struct mem_fn_guard_type {
        bool operator()( Derived const& self, Event const& event ) const {
            return ( self.*guard )( event );
        }
    };

    template<class Event>
    struct no_action : detail::typed_value<no_action_type<Event>> { };

    template<class Event>
    struct no_guard : detail::typed_value<no_guard_type<Event>> { };

    template<class Event, void ( Derived::*action )( Event const& )>
    struct mem_fn_action : detail::typed_value<mem_fn_action_type<Event, action>> { };

    template<class Event, bool ( Derived::*guard )( Event const& ) const>
    struct mem_fn_guard : detail::typed_value<mem_fn_guard_type<Event, guard>> { };

    template<class Event, class T, T* fn>
    using make_action = typename std::conditional<
        !std::is_void<T>::value,
        detail::make_binary_function<T, fn, Derived&, Event const&>,
        no_action<Event>
    >::type;

    template<class Event, class T, T* fn>
    using make_guard = typename std::conditional<
        !std::is_void<T>::value,
        detail::make_binary_function<T, fn, Derived const&, Event const&>,
        no_guard<Event>
    >::type;

    template<class Event, void ( Derived::*action )( Event const& )>
    using make_mem_fn_action = typename std::conditional<
        action != nullptr,
        mem_fn_action<Event, action>,
        no_action<Event>
    >::type;

    /* FIXME g++-4.8.4: ‘(guard != nullptr)’ is not a constant expression
    template<class Event, bool (Derived::*guard)(Event const&) const>
    using make_mem_fn_guard = typename std::conditional<
    guard != nullptr,
    mem_fn_guard<Event, guard>,
    no_guard<Event>
    >::type;
    */

    template<class Event, bool ( Derived::*guard )( Event const& ) const>
    struct make_mem_fn_guard_type {
        bool operator()( Derived const& self, Event const& event ) const {
            return guard ? ( self.*guard )( event ) : true;
        }
    };

    template<class Event, bool ( Derived::*guard )( Event const& ) const>
    struct make_mem_fn_guard : detail::typed_value<make_mem_fn_guard_type<Event, guard>> { };

    template<
        State start,
        class Event,
        State target,
        class Action,
        Action* action,
        class Guard,
        Guard* guard
    >
        struct basic_row {
        using StateType = State;
        using event_type = Event;

        static constexpr StateType start_value ( ) { return start; }
        static constexpr StateType target_value ( ) { return target; }

        static void processEvent ( Derived& self, Event const& event ) {
            ( *action )( self, event );
        }

        static bool check_guard ( Derived const& self, Event const& e ) {
            return ( *guard )( self, e );
        }
    };

    protected:
    /**
    * Transition table base class template.
    *
    * Each derived state machine class must define a nested
    * non-template class `TransitionTable` derived from `table`.
    */
    template<class... Rows>
    struct Table {
        template<class Event>
        static StateType execute ( Derived& self, Event const& event, State state ) {
            using rows = typename filter_by_event_type<Event, Rows...>::type;
            return invoke<Event, rows>::execute ( self, event, state );
        }
    };

    /**
    * Generic transition class template.
    *
    * @tparam start the start state of the transition
    *
    * @tparam Event the event type triggering the transition
    *
    * @tparam target the target state of the transition
    *
    * @tparam Action an action functor type, or `void`
    *
    * @tparam action a pointer to an `Action` instance
    *
    * @tparam Guard a guard functor type, or `void`
    *
    * @tparam guard a pointer to a `Guard` instance
    */
    template<
        State start,
        class Event,
        State target,
        class Action = void,
        Action* action = nullptr,
        class Guard = void,
        Guard* guard = nullptr
    >
        struct row : basic_row<
        start, Event, target,
        typename make_action<Event, Action, action>::type,
        &make_action<Event, Action, action>::value,
        typename make_guard<Event, Guard, guard>::type,
        &make_guard<Event, Guard, guard>::value
        > { };

    /**
    * Member function transition class template.
    *
    * @tparam start the start state of the transition
    *
    * @tparam Event the event type triggering the transition
    *
    * @tparam target the target state of the transition
    *
    * @tparam action an action member function
    *
    * @tparam guard a guard member function, or `nullptr`
    */
    template<
        State start,
        class Event,
        State target,
        void ( Derived::*action )( Event const& ),
        bool ( Derived::*guard )( Event const& ) const = nullptr
    >
        struct Transition : basic_row<
        start, Event, target,
        typename make_mem_fn_action<Event, action>::type,
        &make_mem_fn_action<Event, action>::value,
        typename make_mem_fn_guard<Event, guard>::type,
        &make_mem_fn_guard<Event, guard>::value
        > { };

    private:
    template<class T, class Event>
    struct has_event_type : std::is_same<typename T::event_type, Event> { };

    template<class Event, class... Types>
    struct filter_by_event_type {
        template <class T> using predicate = has_event_type<T, Event>;
        using type = typename detail::filter<predicate, Types...>::type;
    };

    template<class Event, class...> struct invoke;

    template<class Event, class T, class... Types>
    struct invoke<Event, detail::list<T, Types...>> {
        static State execute ( Derived& self, Event const& event, State state ) {
            return state == T::start_value ( ) && T::check_guard ( self, event ) ?
                T::processEvent ( self, event ), T::target_value ( ) :
                invoke<Event, detail::list<Types...>>::execute ( self, event, state );
        }
    };

    template<class Event>
    struct invoke<Event, detail::list<>> {
        static State execute ( Derived& self, Event const& event, State ) {
            return self.no_transition ( event );
        }
    };

    private:
    StateType m_state;
};
}



/*


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

int main ( ) {

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



*/