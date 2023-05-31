#include "WordsTrainer.hpp"

#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <map>

namespace WordsTrainer
{
    bool WordsTrainerGame::isGameRunning = false;
    bool WordsTrainerGame::isTranslationChanged = false;
    bool WordsTrainerGame::isStudyMode = false;

    size_t WordsTrainerGame::rightAnswers = 0;
    size_t WordsTrainerGame::wrongAnswers = 0;
    size_t WordsTrainerGame::countOfFinishedCycles = 0;

    WordsTrainerGame::WordsTrainerGame(const std::string_view& wordsFileName)
    {
        m_WordsManager = std::make_unique<WordsManager>(wordsFileName);
    }
    void WordsTrainerGame::Run()
    {
        // Create a random number generator engine
        std::random_device rd;
        std::mt19937 gen(rd());

        bool canChangeSettings = false;

        auto wordsToStudy = std::make_unique<WordsManager>(*m_WordsManager);

        const auto KeyBoardManagement = [&]()
        {
            constexpr const int qKey = 0x51;
            constexpr const int wKey = 0x57;
            constexpr const int eKey = 0x45;
            while (isGameRunning)
            {
                if (ConsoleHandler::OnKeyComb(qKey))
                {
                    isGameRunning = !isGameRunning;
                }
                if(canChangeSettings)
                {
                    if (ConsoleHandler::OnKeyComb(wKey))
                    {
                        isTranslationChanged = !isTranslationChanged;
                        std::cout << std::endl << "ChangedTranslation = " << (isTranslationChanged ? "true" : "false") << std::endl;
                    }
                    else if (ConsoleHandler::OnKeyComb(eKey))
                    {
                        isStudyMode = !isStudyMode;
                        std::cout << std::endl << "StudyMode = " << (isStudyMode ? "true" : "false") << std::endl;
                    }
                }
            }
        };
        const auto GameMain = [&]()
        {
            std::cout << "Wellcome to WordsTrainer Game!" << std::endl;

            if (rightAnswers > 0 || wrongAnswers > 0)
            {
                std::cout << "Right answers " << rightAnswers << ", wrong answers " << wrongAnswers << std::endl;
            }
            if (countOfFinishedCycles > 0)
                std::cout << "Count of fully finished cycles is " << countOfFinishedCycles << std::endl << std::endl;

            std::cout << "Controlls:" << std::endl;
            std::cout << "Left Ctrl + Q - exit the game" << std::endl;
            std::cout << "Left Ctrl + W - change translation" << std::endl;
            std::cout << "Left Ctrl + E - change study mode" << std::endl;
            std::cout << std::endl;
            std::cout << "Mods settings:" << std::endl;
            std::cout << "ChangedTranslation = " << (isTranslationChanged ? "true" : "false") << std::endl;
            std::cout << "StudyMode = " << (isStudyMode ? "true" : "false") << std::endl;
            std::cout << std::endl;
            std::cout << "Enter the translation of the word:" << std::endl;
            std::cout << std::endl;

            if (wordsToStudy->GetVocabulary().size() == 0)
            {
                wordsToStudy = std::make_unique<WordsManager>(*m_WordsManager);
                countOfFinishedCycles++;
            }

            const int max = wordsToStudy->GetVocabulary().size();
            std::uniform_int_distribution<size_t> dist(0, max - 1);

            const size_t randIndex = dist(gen);

            const SynonymsWord randWord = wordsToStudy->GetVocabulary()[randIndex];
            const auto word = randWord.GetWord();
            const auto translation = randWord.GetTranslation();

            wordsToStudy->EraseWord(randIndex);

            if (word.size() == 0 || translation.size() == 0)
            {
                std::cerr << "word.size() == 0 || translation.size() == 0";
                    throw std::runtime_error("word.size() == 0 || translation.size() == 0");
            }

            const auto wToPrint = !isTranslationChanged ? word : translation;
            const auto right = isTranslationChanged ? word : translation;

            if (right.size() == 0 || wToPrint.size() == 0)
                throw std::runtime_error("right.size() == 0 || wToPrint.size() == 0");

            Word_ entered;

            std::uniform_int_distribution<size_t> wDist(0, wToPrint.size() - 1);//word dist
            std::cout << wToPrint[wDist(gen)] << " - ";
            //std::cin >> entered;
            std::getline(std::cin, entered);

            std::cout << "Now you can change some settings or exit the game using the key combination above" << std::endl;

            //std::thread keyboardInput(KeyBoardManagement);//why does it invokes before the 88s line?
            canChangeSettings = true;

            const auto PrintWordsAndTranslation = [](const Synonym& word, const Synonym& translation)
            {
                for (size_t i = 0; i < word.size(); i++)
                {
                    std::cout << word[i];
                    if (i < word.size() - 1)
                        std::cout << ", ";
                }
                std::cout << " - ";
                for (size_t i = 0; i < translation.size(); i++)
                {
                    std::cout << translation[i];
                    if (i < translation.size() - 1)
                        std::cout << ", ";
                }
            };

            const bool isCorrect = std::find(right.begin(), right.end(), entered) != right.end();

            if (isCorrect)
            {
                std::cout << "Right!" << std::endl;
                rightAnswers++;
            }
            else
            {
                std::cout << "Wrong!" << std::endl;
                wrongAnswers++;
            }
            if (!isCorrect || ((right.size() > 1 || wToPrint.size() > 1) && isCorrect))
            {
                PrintWordsAndTranslation(word, translation);
                std::cout << std::endl;
            }
            if (isStudyMode)
            {
                std::cout << "Enter the word one more time: ";
                std::cin.get();
            }

            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');//<-- this ignore all chars except of '\n'
            //std::cin.get();

            canChangeSettings = false;
            //keyboardInput.detach();

            system("cls");
        };

        isGameRunning = true;

        std::thread keyboardInput(KeyBoardManagement);

        while (isGameRunning)
            GameMain();

        keyboardInput.detach();

        system("cls");
        std::cout << "You have just exited WordsTrainer Game!" << std::endl;
        std::cout << "Statistics:" << std::endl;
        std::cout << "Total right answers are " << rightAnswers << ", wrong are " << wrongAnswers << std::endl;
    }
    bool ConsoleHandler::OnKeyPressed(const key& keyCode)
    {
        return (GetKeyState(keyCode) & 0x8000);
    }
    bool ConsoleHandler::OnKeyDown(const key& keyCode)
    {
        static std::map<int, bool> isKeyPressed;

        if (GetKeyState(keyCode) & 0x8000 && !isKeyPressed[keyCode])
        {
            isKeyPressed[keyCode] = true;
            return true;
        }
        else if (!(GetKeyState(keyCode) & 0x8000))
        {
            isKeyPressed[keyCode] = false;
        }

        return false;
    }
    bool ConsoleHandler::OnKeyComb(const key& keyToDown, const key& keyToPress)
    {
        if (OnKeyPressed(keyToPress))
        {
            if (OnKeyDown(keyToDown))
                return true;
            else
                return false;
        }
        return false;
    }
}