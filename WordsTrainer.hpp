#pragma once

#include "WordsManager.hpp"

#include <memory>
#include <windows.h>

//TODO: WordsTrainerGame, ConsoleHandler(colors)
namespace WordsTrainer
{
    namespace ConsoleAttributes
    {
        enum class ConsoleTextColor : WORD
        {
            Black = 0,
            Blue = FOREGROUND_BLUE,
            Green = FOREGROUND_GREEN,
            Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
            Red = FOREGROUND_RED,
            Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
            Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
            White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            Gray = FOREGROUND_INTENSITY,
            BrightBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BrightCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
            BrightMagenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightYellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BrightWhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };

        enum class ConsoleBackgroundColor : WORD
        {
            Black = 0,
            Blue = BACKGROUND_BLUE,
            Green = BACKGROUND_GREEN,
            Cyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
            Red = BACKGROUND_RED,
            Magenta = BACKGROUND_RED | BACKGROUND_BLUE,
            Yellow = BACKGROUND_RED | BACKGROUND_GREEN,
            White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
        };
    }
}

template<typename... Attributes>
concept ConsoleColorAttributes = (std::is_enum_v<Attributes> && ...)
&& ((std::is_same_v<Attributes, WordsTrainer::ConsoleAttributes::ConsoleTextColor> || ...)
    || (std::is_same_v<Attributes, WordsTrainer::ConsoleAttributes::ConsoleBackgroundColor> || ...));

namespace WordsTrainer
{
    class ConsoleHandler
    {
    public:
        using key = int;

        ConsoleHandler() = delete;
        ~ConsoleHandler() = delete;

        template<ConsoleColorAttributes... Attributes>
        constexpr inline static void SetConsoleColorAttributes(Attributes&&... attrs)
        {
            SetConsoleTextAttribute(console, static_cast<WORD>((static_cast<WORD>(attrs) | ...)));
        }
        static bool OnKeyPressed(const key& key);
        static bool OnKeyDown(const key& key);
        static bool OnKeyComb(const key& keyToDown, const key& keyToPress = VK_LCONTROL);
    private:
        static HANDLE console;
    };
    //game class
    class WordsTrainerGame
    {
    public:
        WordsTrainerGame(const std::string_view& wordsFileName = wordsFileName);
        ~WordsTrainerGame() = default;

        void Run();
    private:
        //const WordsManager m_WordsManager;
        std::unique_ptr<WordsManager> m_WordsManager;
        static ConsoleHandler m_ConsoleHandler;

        static bool isGameRunning;
        static bool isTranslationChanged;
        static bool isStudyMode;

        static size_t rightAnswers;
        static size_t wrongAnswers;
        static size_t countOfFinishedCycles;
    };
}