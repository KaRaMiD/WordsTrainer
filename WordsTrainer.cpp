//#include "WordsTrainer.hpp"
//
import <windows.h>;
import <iostream>;
import <thread>;
import <random>;
import <algorithm>;
import <cstdlib>;
import <map>;
module WordsTrainer;

//import <iostream>;
//import <thread>;
//import <random>;
//import <algorithm>;
//import <cstdlib>;
//import <map>;

namespace WordsTrainer
{
    bool WordsTrainerGame::isGameRunning = false;
    bool WordsTrainerGame::isTranslationChanged = false;
    bool WordsTrainerGame::isStudyMode = false;

    size_t WordsTrainerGame::rightAnswers = 0;
    size_t WordsTrainerGame::wrongAnswers = 0;
    size_t WordsTrainerGame::countOfFinishedCycles = 0;

    HANDLE ConsoleHandler::console = GetStdHandle(STD_OUTPUT_HANDLE);

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
        size_t totalAnsweredWordsCount{};

        const auto KeyBoardManagement = [&]()
        {
            constexpr const int qKey = 0x51;
            constexpr const int wKey = 0x57;
            constexpr const int eKey = 0x45;
            while (isGameRunning)
            {
                if(GetForegroundWindow() == GetConsoleWindow())
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
                            std::cout << '\n' << "ChangedTranslation = " << (isTranslationChanged ? "true" : "false") << '\n';
                        }
                        else if (ConsoleHandler::OnKeyComb(eKey))
                        {
                            isStudyMode = !isStudyMode;
                            std::cout << '\n' << "StudyMode = " << (isStudyMode ? "true" : "false") << '\n';
                        }
                    }
                }
            }
        };
        const auto GameMain = [&]()
        {
            std::cout << "Wellcome to WordsTrainer Game!" << '\n' << '\n';
            std::cout << "Stats:" << '\n';
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightMagenta);
            std::cout << "Total number of words - " << m_WordsManager->GetVocabulary().size() << '\n';
            std::cout << "You have already answered on " << totalAnsweredWordsCount <<" words" << '\n';

            if (rightAnswers > 0 || wrongAnswers > 0)
            {
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightGreen);
                std::cout << "Right answers " << rightAnswers;
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
                std::cout << ", ";
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightRed);
                std::cout << "wrong answers " << wrongAnswers << '\n';
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightMagenta);
            }
            if (countOfFinishedCycles > 0)
                std::cout << "Count of fully finished cycles is " << countOfFinishedCycles << '\n';
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
            std::cout << '\n';

            std::cout << "Controlls:" << '\n';
            std::cout << "Left Ctrl + Q - exit the game" << '\n';
            std::cout << "Left Ctrl + W - change translation" << '\n';
            std::cout << "Left Ctrl + E - change study mode" << '\n';
            std::cout << '\n';
            std::cout << "Mods settings:" << '\n';
            std::cout << "ChangedTranslation = " << (isTranslationChanged ? "true" : "false") << '\n';
            std::cout << "StudyMode = " << (isStudyMode ? "true" : "false") << '\n';
            std::cout << '\n';
            std::cout << "Enter the translation of the word:" << '\n';
            std::cout << '\n';

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
                throw std::runtime_error("word.size() == 0 || translation.size() == 0");
            }

            const auto wToPrint = !isTranslationChanged ? word : translation;
            const auto right = isTranslationChanged ? word : translation;

            if (right.size() == 0 || wToPrint.size() == 0)
                throw std::runtime_error("right.size() == 0 || wToPrint.size() == 0");

            Word_ entered;

            std::uniform_int_distribution<size_t> wDist(0, wToPrint.size() - 1);//word dist
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightCyan);
            std::cout << wToPrint[wDist(gen)] << " - ";
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
            //std::cin >> entered;
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightYellow);
            std::getline(std::cin, entered);
            m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);

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
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightGreen);
                std::cout << "Right!" << '\n';
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
                rightAnswers++;
            }
            else
            {//TODO: ADD READING "^E", etc
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightRed);
                std::cout << "Wrong!" << '\n';
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
                wrongAnswers++;
            }
            if (!isCorrect || ((right.size() > 1 || wToPrint.size() > 1) && isCorrect))
            {
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::Cyan);
                PrintWordsAndTranslation(word, translation);
                m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
                std::cout << '\n';
            }
            std::cout << "Now you can change some settings or exit the game using the key combination above" << '\n';
            if (isStudyMode)
            {
                std::cout << "Enter the word one more time: ";
                std::cin.get();
            }

            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');//<-- this ignore all chars except of '\n'
            //std::cin.get();

            canChangeSettings = false;
            //keyboardInput.detach();

            totalAnsweredWordsCount++;

            system("cls");
        };

        isGameRunning = true;

        std::thread keyboardInput(KeyBoardManagement);

        while (isGameRunning)
            GameMain();

        keyboardInput.detach();

        system("cls");
        std::cout << "You have just exited WordsTrainer Game!" << '\n';
        std::cout << "Statistics:" << '\n';
        m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightGreen);
        std::cout << "Total right answers are " << rightAnswers;
        m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
        std::cout << ", ";
        m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::BrightRed);
        std::cout << "wrong are " << wrongAnswers << '\n';
        m_ConsoleHandler.SetConsoleColorAttributes(ConsoleAttributes::ConsoleTextColor::White);
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