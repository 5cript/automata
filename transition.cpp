#include "transition.hpp"
#include "automata.hpp"

namespace MiniAutomata
{
//#####################################################################################################################
    Transition::Transition(Automaton* parent, std::size_t to, boost::optional <Trigger> const& trig)
        : parent_{parent}
        , to_{to}
        , trigger_{trig ? trig.get() : Trigger{}}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Transition::test() const
    {
        return trigger_.test();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Transition::perform() const
    {
        parent_->setState(to_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Transition::getTargetName() const
    {
        return parent_->states_[to_].getName();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t Transition::getTarget() const
    {
        return to_;
    }
//#####################################################################################################################
    TransitionSet::TransitionSet()
        : transitions_{}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    void TransitionSet::insert(Transition* trans)
    {
        transitions_.insert(trans);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::iterator TransitionSet::begin()
    {
        return std::begin(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::iterator TransitionSet::end()
    {
        return std::end(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::const_iterator TransitionSet::begin() const
    {
        return std::cbegin(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::const_iterator TransitionSet::end() const
    {
        return std::cend(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::const_iterator TransitionSet::cbegin() const
    {
        return std::cbegin(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unordered_set <Transition*>::const_iterator TransitionSet::cend() const
    {
        return std::cend(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    Transition* TransitionSet::first()
    {
        return *std::begin(transitions_);
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TransitionSet::empty() const
    {
        return transitions_.empty();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t TransitionSet::size() const
    {
        return transitions_.size();
    }
//#####################################################################################################################
}
