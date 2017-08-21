#pragma once

#include <functional>
#include <memory>

namespace MiniAutomata
{
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
