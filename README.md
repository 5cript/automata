# Automata
A library for bringing automatons with specific transition conditions directly into code.
Only one state can ever be active at the time.
Multiple transitions can be active at the same time, but only one can fire to change the state.

Choosing which transition to take is random if not handled by the library user.

---
## Creating an automaton
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    // An automaton without states and transitions
    auto automat = makeAutomaton();
}
```

## Creating states
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    // An automaton with states
    auto automat = makeAutomaton()
        << "State 0"
        << "State 1"
    ;

    // insert a state with a numeric id.
    automat << "Bla"_as(2);
}
```

## Transitions
(Now assuming following automaton)
```C++
auto automat = makeAutomaton()
    << "Idle"
    << "Active"
    << "Waiting"
;
```

Transition without condition.
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    // Insert a transition without condition from Idle to Active.
    automat > "Idle" > "Active";
}
```
Transition with simple condition.
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    // Can transition from idle to active every time.
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Active";
}
```

Transitions to different states
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    // Can transition from idle to active every time.
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Active";
        
    // Do note that both transitions are active and could be chosen.
    automat 
        > "Active" > [](){return true;} // <- transition condition
        > "Waiting";        
    automat 
        > "Active" > [](){return true;} // <- transition condition
        > "Idle";

    // Alternatively
    automat 
        > "Active" 
        // Transition to idle or waiting, depending on condition and selection
        // do note the _as literal, as its required for overload resolution to take proper operators.
        > (
                "Idle"_as < [](){return true;}
            ||  "Waiting"_as < [](){return true;}
        )
    ;
}
```

## Performing State Transitions.
Simple case.
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    automat.seed(0); // set a random seed. Take time or std::random_device if randomness is needed.

    // Can transition from idle to active every time.
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Active";

    // take all active transitions and select one active at random
    // do nothing if no transition is active.
    automat.advance();
}
```

Selecting a transition from a set of active transitions.
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    automat.seed(0); // set a random seed. Take time or std::random_device if randomness is needed.
        
    // Do note that both transitions are active and could be chosen.
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Waiting";        
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Active";

    // Would be random
    //automat.advance();

    automat.advance([&](auto const& transitionSet) {
        std::cout << "there are " << transitionSet.size() << " active transtions\n";

        // (*transitionSet.begin())->getTargetName();

        // always chose the first transition
        return transitionSet.cbegin();
    });
}
```

## Attaching state actions
```C++
#include <automata/automata.hpp>

using namespace MiniAutomata;

int main()
{
    /* ... */

    // Can transition from idle to active every time.
    automat 
        > "Idle" > [](){return true;} // <- transition condition
        > "Active";

    automat["Active"].bindAction([](){
        std::cout << "Automaton is now at 'Active'\n";
    });

    // take all active transitions and select one active at random.
    // Will call the action of active.
    automat.advance();
}
```
