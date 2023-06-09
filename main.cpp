//#include <iostream>
//#include <windows.h>
//#include <memory>
//#include <fstream>

//#include "WordsTrainer.hpp"

//#define DEBUG

import <iostream>;
import <windows.h>;

import WordsTrainer;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        WordsTrainer::WordsTrainerGame w;
        w.Run();
        /*auto wordsManager = std::make_unique<WordsTrainer::WordsManager>(WordsTrainer::wordsFileName);
        std::ofstream ofile("words__.txt");
        if (ofile)
        {
            for (size_t i = 0; i < wordsManager->GetVocabulary().size(); i++)
            {
                const auto size = wordsManager->GetVocabulary()[i].GetWord().size();
                for (size_t j = 0; j < size; j++)
                {
                    ofile << wordsManager->GetVocabulary()[i].GetWord()[j];
                    ofile << ", ";
                }
            }
        }
        else
            throw std::runtime_error("failed to create new words file");
        ofile.close();*/
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