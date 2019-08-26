#include "automata.hpp"

#include <stdexcept>
#include <chrono>

namespace MiniAutomata
{
	using namespace std::string_literals;
//#####################################################################################################################
    Automaton::Automaton()
        : nameMappings_{}
        , idMappings_{}
        , states_{}
        , currentState_{0}
        , transitions_{}
        , randGenerator_{static_cast <unsigned int> (std::chrono::system_clock::now().time_since_epoch().count())}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void Automaton::insertMappings()
    {
        auto& state = states_.back();
        nameMappings_.emplace(state.getName(), states_.size() - 1u);
        auto id = state.getId();
        if (id)
            idMappings_.emplace(id.get(), states_.size() - 1u);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t Automaton::getMapped(std::string const& name)
    {
        auto iter = nameMappings_.find(name);
        if (iter != std::end(nameMappings_))
            return iter->second;
        else
            throw std::invalid_argument(("no such state with name '"s + name + "' in automata").c_str());
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t Automaton::getMapped(int id)
    {
        auto iter = idMappings_.find(id);
        if (iter != std::end(idMappings_))
            return iter->second;
        else
            throw std::invalid_argument(("no such state with id '"s + std::to_string(id) + "' in automata").c_str());
    }
//---------------------------------------------------------------------------------------------------------------------
    State& Automaton::operator[](std::string const& name)
    {
        return states_[getMapped(name)];
    }
//---------------------------------------------------------------------------------------------------------------------
    State& Automaton::operator[](int id)
    {
        return states_[getMapped(id)];
    }
//---------------------------------------------------------------------------------------------------------------------
    void Automaton::setState(std::size_t num)
    {
        currentState_ = num;
        states_[currentState_]();
    }
//---------------------------------------------------------------------------------------------------------------------
    TransitionSet Automaton::getActiveTransitions()
    {
        TransitionSet result;
        auto range = transitions_.equal_range(currentState_);
        for (auto i = range.first; i != range.second; ++i)
        {
            if (i->second.test())
            {
                result.insert(&i->second);
            }
        }
        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Automaton::seed()
    {
        seed(std::chrono::system_clock::now().time_since_epoch().count());
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Automaton::advance()
    {
        // advance to a random active transition.
        return advance([this](TransitionSet const& active) -> TransitionSet::const_iterator {
            std::uniform_int_distribution<std::size_t> distribution{0, active.size() - 1};
            auto iter = std::begin(active);
            std::advance(iter, distribution(randGenerator_));
            return iter;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    boost::optional <std::string> Automaton::getCurrentStateName() const
    {
		if (states_.empty())
			return boost::none;
        return states_[currentState_].getName();
    }
//---------------------------------------------------------------------------------------------------------------------
    boost::optional <int> Automaton::getCurrentStateId() const
    {
		if (states_.empty())
			return boost::none;
        return states_[currentState_].getId();
    }
//---------------------------------------------------------------------------------------------------------------------
	std::size_t Automaton::stateCount() const
	{
		return states_.size();
	}
//---------------------------------------------------------------------------------------------------------------------
    Automaton::TransitionBegin operator>(Automaton& automat, std::string const& name)
    {
        auto num = automat.getMapped(name);
        return {&automat, num};
    }
//---------------------------------------------------------------------------------------------------------------------
    Automaton::TransitionBegin operator>(Automaton& automat, int id)
    {
        auto num = automat.getMapped(id);
        return {&automat, num};
    }
//---------------------------------------------------------------------------------------------------------------------
    void Automaton::tryEmplace(std::size_t from, std::size_t to, boost::optional <Trigger> const& trig)
    {
        // Do not add edges twice.
        auto range = transitions_.equal_range(from);
        for (auto i = range.first; i != range.second; ++i)
            if (i->second.getTarget() == to)
                return;

        // add edge.
        transitions_.emplace(from, Transition{this, to, trig});
    }
//---------------------------------------------------------------------------------------------------------------------
    Automaton::TransitionBegin operator>(Automaton::TransitionBegin const& prior, std::string const& name)
    {
        auto to = prior.stem->getMapped(name);
        prior.stem->tryEmplace(prior.from, to, prior.trig);
        return Automaton::TransitionBegin{prior.stem, to};
    }
//---------------------------------------------------------------------------------------------------------------------
    Automaton::TransitionBegin operator>(Automaton::TransitionBegin const& prior, int id)
    {
        auto to = prior.stem->getMapped(id);
        prior.stem->tryEmplace(prior.from, to, prior.trig);
        return Automaton::TransitionBegin{prior.stem, to};
    }
//---------------------------------------------------------------------------------------------------------------------
    void operator>(Automaton::TransitionBegin const& prior, ProtoState::StateBinding&& binding)
    {
        auto addTransition = [&](auto identification, Trigger const& trig)
        {
            auto to = prior.stem->getMapped(identification);
            if (prior.trig)
                prior.stem->tryEmplace(
                    prior.from,
                    to,
                    Trigger{[trig, ptrig=prior.trig.get()]() -> bool {return trig.test() && ptrig.test();}}
                );
            else
                prior.stem->tryEmplace(prior.from, to, trig);
        };

        for (auto const& i : binding)
        {
            if (i.id)
                addTransition(i.id.get(), i.trig);
            else
                addTransition(i.name.get(), i.trig);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Automaton::advance(std::function <TransitionSet::const_iterator(TransitionSet const& set)> selector)
    {
        auto active = getActiveTransitions();
        if (active.empty())
            return false;

        if (active.size() == 1)
        {
            active.first()->perform();
            return true;
        }

        auto iter = selector(active);

        (*iter)->perform();
        return true;
    }
//---------------------------------------------------------------------------------------------------------------------
    Automaton::TransitionBegin operator>(Automaton::TransitionBegin&& prior, std::function <bool()> const& func)
    {
        prior.trig = Trigger{func};
        return prior;
    }
//#####################################################################################################################
    Automaton makeAutomaton()
    {
        return {};
    }
//#####################################################################################################################
}
