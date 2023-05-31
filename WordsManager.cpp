#include "WordsManager.hpp"

#include "WordsHandler.hpp"

namespace WordsTrainer
{
    WordsManager::WordsManager(const std::string_view& wordsFileName)
    {
        const auto rwords = WordsHandler::GetRawWordsFromTheFile(wordsFileName);

        const auto count = std::count(rwords.begin(), rwords.end(), newLineSymbol);

        for (size_t i = 0; i < count; i++)
        {
            m_Vocabulary.push_back(WordsHandler::GetSynonimsWordFromTheLine(WordsHandler::GetCleanLine(rwords, i)));
        }
    }
}