#pragma once

#include "WordsContainer.hpp"

namespace WordsTrainer
{
    static constinit const char* const wordsFileName = "words.txt";

    class WordsManager
    {
    public:
        WordsManager(const std::string_view& wordsFileName = wordsFileName);
        
        ~WordsManager() = default;

        const Vocabulary<SynonymsWord>& GetVocabulary() { return m_Vocabulary; }
        void EraseWord(const size_t& index) { m_Vocabulary.erase(m_Vocabulary.begin() + index); }
    private:
        Vocabulary<SynonymsWord> m_Vocabulary;
    };
}