#include "state.hpp"

namespace MiniAutomata
{
//#####################################################################################################################
    ProtoState::ProtoState(const char* name)
        : id_{boost::none}
        , name_{name}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::ProtoState(unsigned long long int id)
        : id_{static_cast <int> (id)}
        , name_{std::to_string(id)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    State ProtoState::operator()(int id)
    {
        return {id, std::move(name_)};
    }
//---------------------------------------------------------------------------------------------------------------------
    State ProtoState::operator()() &&
    {
        return {std::move(name_)};
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState&& lhs, std::string&& name)
    {
        if (lhs.id_)
            return {{lhs.id_.get()}, {name}};
        else
            return {{lhs.name_}, {name}};
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState&& lhs, ProtoState&& rhs)
    {
        if (lhs.id_ && rhs.id_)
            return {{lhs.id_.get()}, {rhs.id_.get()}};
        else if (lhs.id_)
            return {{lhs.id_.get()}, {rhs.name_}};
        else
            return {{lhs.name_}, {rhs.name_}};
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState&& lhs, int id)
    {
        if (lhs.id_)
            return {{lhs.id_.get()}, {id}};
        else
            return {{lhs.name_}, {id}};
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState::StateBinding&& lhs, std::string&& name)
    {
        lhs.push_back({name});
        return lhs;
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState::StateBinding&& lhs, ProtoState&& rhs)
    {
        if (rhs.id_)
            lhs.emplace_back(rhs.id_.get());
        else
            lhs.emplace_back(rhs.name_);
        return lhs;
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState::StateBinding&& lhs, ProtoState::StateBinding&& rhs)
    {
        lhs.insert(std::end(lhs), std::begin(rhs), std::end(rhs));
        return lhs;
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator||(ProtoState::StateBinding&& lhs, int id)
    {
        lhs.push_back({id});
        return lhs;
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState::StateBinding operator<(ProtoState&& proto, std::function <bool()> trig)
    {
        if (proto.id_)
            return {{proto.id_.get(), trig}};
        else
            return {{proto.name_, trig}};
    }
//#####################################################################################################################
    State::State(std::string&& name)
        : id_{boost::none}
        , name_{std::move(name)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    State::State(int id, std::string&& name)
        : id_{id}
        , name_{std::move(name)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    std::string State::getName() const
    {
        return name_;
    }
//---------------------------------------------------------------------------------------------------------------------
    boost::optional <int> State::getId() const
    {
        return id_;
    }
//#####################################################################################################################
    ProtoState operator "" _as(const char* name, std::size_t)
    {
        return ProtoState{name};
    }
//---------------------------------------------------------------------------------------------------------------------
    ProtoState operator "" _as(unsigned long long int id)
    {
        return ProtoState{id};
    }
}
