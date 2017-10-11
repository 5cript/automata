#pragma once

#include <functional>
#include <memory>

namespace MiniAutomata
{
    /**
     *  A trigger is the activation condition for a transition.
     *  Despite its name, a trigger does not automatically perform the transition.
     */
    class Trigger
    {
    public:
        Trigger(std::function <bool()> const& condition);
        Trigger();

        void setCondition(std::function <bool()> const& condition);

        bool test() const;

    private:
        std::function <bool()> condition_;
    };
}
