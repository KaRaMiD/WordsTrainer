#pragma once

#include <string>

#include "Word.hpp"
#include "WordsContainer.hpp"

namespace WordsTrainer
{
    class WordsHandler
    {
    public:
        WordsHandler() = delete;
        ~WordsHandler() = delete;

        //unacceptable symbols
        static inline const std::string uasymbols = "\n ";
        static inline bool IsSymbol(const char& c) {//TODO THIS
            // Check if the character is in the uasymbols set
            return ((uasymbols.find_first_of(c) == std::string::npos));
        }
        /*
        * @note cutts only prefixes and suffixes
        */
        static std::string CutExtraSymbols(std::string str);
        /*
        * @param rstr - raw string
        * @param lindex - tmpline index(give it with 0)
        */
        static inline std::string_view GetLine(const std::string_view& rstr, const size_t& lindex);
        static std::string GetCleanLine(const std::string_view& rstr, const size_t& lindex);
        static size_t FindFirstCharPos(const std::string_view& str);

        /*
        * @note raw means that words is cutted with all whitespaces and etc
        * @param rline - raw tmpline
        */
        static std::string_view GetRawWordsFromLine(const std::string_view& rline);
        /*
        * @note raw means that words is cutted with all whitespaces and etc
        * @param rline - raw tmpline
        */
        static std::string_view GetRawTranslationFromLine(const std::string_view& rline);
        static Synonym SplitWordsToSynonyms(const std::string_view& str);

        static SynonymsWord GetSynonimsWordFromTheLine(const std::string_view& rline);

        static std::string GetRawWordsFromTheFile(const std::string_view& fileName = "words.txt");
    };
}