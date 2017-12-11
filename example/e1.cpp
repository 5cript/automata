#include "automata.hpp"

#include <iostream>

int main()
{
    using namespace MiniAutomata;

    auto automat = makeAutomaton()
        << "Idle"_as(0)
        << "Ready"_as(1)
        << "InitWait"_as(2)
        << "T1 Wait"_as(3)
        << "T2 Wait"_as(4)
        << "Success"_as(5)
        << "Failure"_as(6)
    ;

    automat > "Idle" > [](){return true;}
            > "Ready"
            > "InitWait"
            > 3
            > 4 > [](){return true;}
            > (
                        "Success"_as < [](){return true;}
                    ||  "Failure"_as < [](){return true;}
            )
    ;

    automat > "Success" > "Idle";
    automat > "Failure" > "Idle";

    for (int i = 0; i != 15; ++i)
    {
        std::cout << automat.getCurrentStateName() << "\n";
        std::cin.get();
        system("cls");
        automat.advance();
    }
    std::cout << automat.getCurrentStateName() << "\n";


    return 0;
}
