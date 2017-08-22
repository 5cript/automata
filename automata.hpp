#pragma once

#include "automata_fwd.hpp"
#include "state.hpp"
#include "transition.hpp"

#include <utility>
#include <vector>
#include <unordered_map>
#include <random>

namespace MiniAutomata
{
    /**
     *  A finite state (non)deterministic automaton.
     */
    class Automaton
    {
    public:
        friend Transition;

        struct TransitionBegin
        {
            Automaton* stem;
            std::size_t from;
            boost::optional <Trigger> trig;

            TransitionBegin(Automaton* stem, std::size_t from, boost::optional <Trigger> trig)
                : stem{stem}
                , from{from}
                , trig{trig}
            {}

            TransitionBegin(Automaton* stem, std::size_t from)
                : stem{stem}
                , from{from}
                , trig{boost::none}
            {}
        };

    public:
        Automaton();

        /**
         *  Seeds the random engine.
         */
        template <typename T>
        void seed(T const& seed)
        {
            randGenerator_.seed(seed);
        }

        /**
         *  Seeds the random engine with time.
         */
        void seed();

        /**
         *  operator<< to insert states into the automata
         */
        template <typename T>
        typename std::enable_if <std::is_same <typename std::decay <T>::type, State>::value, Automaton&>::type
        operator<<(T&& state)
        {
            states_.push_back(std::forward <T&&>(state));
            insertMappings();
            return *this;
        }

        /**
         *  operator<< to insert id-less states into the automata
         */
        template <typename T>
        typename std::enable_if <std::is_same <typename std::decay <T>::type, ProtoState>::value, Automaton&>::type
        operator<<(T&& state)
        {
            return operator<<(std::move(std::forward <T&&> (state)()));
        }

        /**
         *  operator<< to insert states into the automata
         */
        Automaton& operator<<(const char* cstr)
        {
            return operator<<(ProtoState{cstr}());
        }

        /**
         *  operator[] to retrieve state elements by name.
         */
        State& operator[](std::string const& name);

        /**
         *  operator[] to retrieve state elements by id.
         */
        State& operator[](int id);

        /**
         *  Test for any transition conditions becoming true.
         */
        TransitionSet getActiveTransitions();

        /**
         *  Inserts Transitions.
         */
        friend TransitionBegin operator>(Automaton& automat, std::string const& name);

        /**
         *  Inserts transitions.
         */
        friend TransitionBegin operator>(Automaton& automat, int id);

        /**
         *  Inserts transitions.
         */
        friend TransitionBegin operator>(TransitionBegin const& prior, std::string const& name);

        /**
         *  Inserts transitions.
         */
        friend TransitionBegin operator>(TransitionBegin const& prior, int id);

        /**
         *  Inserts transitions.
         */
        friend void operator>(TransitionBegin const& prior, ProtoState::StateBinding&& binding);

        /**
         *  Sets trigger for transition.
         */
        friend TransitionBegin operator>(TransitionBegin&& prior, std::function <bool()> const& func);

        /**
         *  Returns the name of the current state
         */
        std::string getCurrentStateName() const;

        /**
         *  Returns the name of the current state
         */
        boost::optional <int> getCurrentStateId() const;

        /**
         *  Transition to the next state, if possible. Selects a random transition, if multiple are active.
         *
         *  @return Returns true, if a transition has been made.
         */
        bool advance();

        /**
         *  Transition to the next state, if possible. Calls the selector function, if multiple are active.
         *
         *  @return Returns true, if a transition has been made.
         */
        bool advance(std::function <TransitionSet::const_iterator(TransitionSet const& set)> selector);

    private:
        void tryEmplace(std::size_t from, std::size_t to, boost::optional <Trigger> const& trig);
        void insertMappings();
        void setState(std::size_t num);
        std::size_t getMapped(std::string const& name);
        std::size_t getMapped(int id);

    private:
        // Mappings
        std::unordered_map <std::string, std::size_t> nameMappings_;
        std::unordered_map <int, std::size_t> idMappings_;

        // Can only grow, cannot shrink
        std::vector <State> states_;

        std::size_t currentState_;

        // Edges / Transitions
        std::unordered_multimap <std::size_t, Transition> transitions_;

        std::mt19937 randGenerator_;
    };

    Automaton makeAutomaton();
}
