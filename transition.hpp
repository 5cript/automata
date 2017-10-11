#pragma once

#include "automata_fwd.hpp"
#include "trigger.hpp"
#include "state.hpp"

#include <unordered_set>

namespace MiniAutomata
{
    /**
     *  A transition describes a one way connection between two states.
     */
    class Transition
    {
    public:
        Transition(Automaton* parent, std::size_t to, boost::optional <Trigger> const& trig);

        /**
         *  Test the trigger condition.
         */
        bool test() const;

        /**
         *  Does no testing, just performs the transition.
         */
        void perform() const;

        /**
         *  Returns the name of the state this transition points to.
         */
        std::string getTargetName() const;

        /**
         *  Returns the node position in the automaton.
         */
        std::size_t getTarget() const;

    private:
        Automaton* parent_;
        std::size_t to_;
        Trigger trigger_;
    };

    /**
     *  A transition set composes a set of transitions that are active and selectable.
     */
    class TransitionSet
    {
    public:
        using iterator = std::unordered_set <Transition*>::iterator;
        using const_iterator = std::unordered_set <Transition*>::const_iterator;

    public:
        TransitionSet();
        void insert(Transition* trans);
        std::unordered_set <Transition*>::iterator begin();
        std::unordered_set <Transition*>::iterator end();
        std::unordered_set <Transition*>::const_iterator begin() const;
        std::unordered_set <Transition*>::const_iterator end() const;
        std::unordered_set <Transition*>::const_iterator cbegin() const;
        std::unordered_set <Transition*>::const_iterator cend() const;
        Transition* first();
        bool empty() const;
        std::size_t size() const;

    private:
        std::unordered_set <Transition*> transitions_;
    };
}
