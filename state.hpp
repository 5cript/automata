#pragma once

#include "automata_fwd.hpp"
#include "trigger.hpp"

#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace MiniAutomata
{
//#####################################################################################################################
    // declaration
    class State;
//#####################################################################################################################
    class ProtoState
    {
    public:
        struct StateBindingElement
        {
            boost::optional <std::string> name;
            boost::optional <int> id;
            Trigger trig;

            StateBindingElement(std::string const& name)
                : name{name}
                , id{boost::none}
                , trig{}
            {}

            StateBindingElement(int id)
                : name{boost::none}
                , id{id}
                , trig{}
            {}

            StateBindingElement(std::string const& name, std::function <bool()> trig)
                : name{name}
                , id{boost::none}
                , trig{trig}
            {}

            StateBindingElement(int id, std::function <bool()> trig)
                : name{boost::none}
                , id{id}
                , trig{trig}
            {}
        };

        using StateBinding = std::vector <StateBindingElement>;

    public:
        friend Transition;

    public:
        ProtoState(const char* name /* null determinated */);
        ProtoState(unsigned long long int id);

        State operator()(int id);
        State operator()() &&;

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(ProtoState&& lhs, std::string&& name);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(ProtoState&& lhs, ProtoState&& rhs);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(ProtoState&& lhs, int id);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(StateBinding&& lhs, std::string&& name);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(StateBinding&& lhs, ProtoState&& rhs);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(StateBinding&& lhs, StateBinding&& rhs);

        /**
         *  Binds states together for transitions.
         */
        friend StateBinding operator||(StateBinding&& lhs, int id);

        /**
         *  Add triggers to protos
         */
        friend StateBinding operator<(ProtoState&& proto, std::function <bool()> trig);

    private:
        boost::optional <int> id_;
        std::string name_;
    };
//#####################################################################################################################
    class State
    {
    public:
        State(std::string&& name);
        State(int id, std::string&& name);

        std::string getName() const;
        boost::optional <int> getId() const;

    private:
        boost::optional <int> id_;
        std::string name_;
    };
//#####################################################################################################################
    ProtoState operator "" _as(const char* name, std::size_t);
    ProtoState operator "" _as(unsigned long long int id);
}
