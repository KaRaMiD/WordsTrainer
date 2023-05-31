#include <iostream>
#include <windows.h>

#include "WordsTrainer.hpp"

//#define DEBUG

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        WordsTrainer::WordsTrainerGame w;
        w.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "error: " << e.what();
    }

#ifndef DEBUG
    system("pause");
#endif // !DEBUG

    return 0;
}