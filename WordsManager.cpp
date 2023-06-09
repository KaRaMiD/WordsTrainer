//#include "WordsManager.hpp"
//
//#include "WordsHandler.hpp"
import <iostream>;
module WordsTrainer.WordsManager;

import WordsTrainer.WordsHandler;

//import <iostream>;

namespace WordsTrainer
{
    WordsManager::WordsManager(const std::string_view& wordsFileName)
    {
        const auto rwords = WordsHandler::GetRawWordsFromTheFile(wordsFileName);

        const auto count = std::count(rwords.begin(), rwords.end(), newLineSymbol);

        m_Vocabulary.reserve(count);

        for (size_t i = 0; i < count; i++)
        {
            m_Vocabulary.push_back(WordsHandler::GetSynonimsWordFromTheLine(WordsHandler::GetCleanLine(rwords, i)));
        }
    }
}