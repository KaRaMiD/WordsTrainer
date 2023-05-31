#include "WordsHandler.hpp"

#include <type_traits>
#include <string_view>
#include <string>
#include <cctype>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace WordsTrainer
{
    std::string WordsHandler::CutExtraSymbols(std::string str)
    {
        //begin
        for (; str.size();)
        {
            if (!IsSymbol(str[0]))
            {
                str.erase(str.begin());
                continue;
            }
            break;
        }
        //end
        for (; str.size();)
        {
            if (!IsSymbol(str[str.size() - 1]))
            {
                str.erase(str.end() - 1);
                continue;
            }
            break;
        }

        return str;
    }
    inline std::string_view WordsHandler::GetLine(const std::string_view& rstr, const size_t& lindex)
    {
        if (rstr.find(newLineSymbol) == std::string::npos)
            throw std::runtime_error("GetLine: there are not any newLineSymbols");
        size_t found{};
        std::string_view line;
        //TODO: make this work possible with a lindex = 0
        if (lindex > 0)//funcking crutch
        {
            for (size_t i = 0; i < rstr.size(); i++)
            {
                const size_t tmp = rstr.find_first_of(newLineSymbol, found + 1);

                if (tmp == std::string::npos)
                    break;
                else
                {
                    found = tmp;
                    if (i == lindex - 1)
                    {
                        line = rstr.substr(found + 1, rstr.find_first_of(newLineSymbol, found + 1) - found - 1);
                    }
                }
            }//TODO THIS FUNC
        }
        else
        {
            line = rstr.substr(0, rstr.find_first_of(newLineSymbol));
        }

        return line;//rstr.substr(0, rstr.find_first_of(newLineSymbol))
    }
    std::string WordsHandler::GetCleanLine(const std::string_view& rstr, const size_t& lindex)
    {
        return CutExtraSymbols((std::string)GetLine(rstr, lindex));
    }
    size_t WordsHandler::FindFirstCharPos(const std::string_view& str)
    {
        for (size_t i = 0; i < str.size(); i++)
        {
            if (!std::isspace(str.data()[i]))
                return i;
        }
        throw std::runtime_error("FindFirstCharPos: there are no any characters");
    }
    std::string_view WordsHandler::GetRawWordsFromLine(const std::string_view& rline)
    {
        if (rline.find(translationSymbol) == std::string::npos)
            throw std::runtime_error("GetWordsFromLine: there are no translationSymbols");

        return rline.substr(0, rline.find_first_of(translationSymbol));
    }
    std::string_view WordsHandler::GetRawTranslationFromLine(const std::string_view& rline)
    {
        if (rline.find(translationSymbol) == std::string::npos)
            throw std::runtime_error("GetRawTranslationFromLine: there are no translationSymbols");

        return rline.substr(rline.find_first_of(translationSymbol) + 1, rline.size() - 1);
    }
    Synonym WordsHandler::SplitWordsToSynonyms(const std::string_view& str)
    {
        //if (rline.find(synonimSymbol) == std::string::npos)
        //    throw std::runtime_error("SplitWordsToSynonyms: there are no any syninim symbols");

        Synonym synonym;

        if (str.find(synonimSymbol) != std::string::npos)
        {
            for (size_t i = 0; i < str.size();)
            {
                Word_ word;
                const size_t foundSynSymb = str.find_first_of(synonimSymbol, i);
                if (foundSynSymb == std::string::npos)
                {
                    word = str.substr(i, str.size());
                    synonym.push_back(CutExtraSymbols(word));
                    break;
                }
                else
                {
                    word = str.substr(i, str.find_first_of(synonimSymbol, i) - i);
                    if (!word.empty())
                        synonym.push_back(CutExtraSymbols(word));
                    i = foundSynSymb + 2;
                }
            }
        }
        else
        {
            synonym.push_back(CutExtraSymbols((std::string)str));
        }

        return synonym;
    }
    SynonymsWord WordsHandler::GetSynonimsWordFromTheLine(const std::string_view& rline)
    {
        if (rline.empty() || rline.find(translationSymbol) == std::string::npos)
            throw std::runtime_error("GetWordFromTheLine: the str is empty or str doesn't contain translation symbol");

        const SynonymsWord word(
            SplitWordsToSynonyms(GetRawWordsFromLine(rline)),
            SplitWordsToSynonyms(GetRawTranslationFromLine(rline))
        );

        return word;
    }
    std::string WordsHandler::GetRawWordsFromTheFile(const std::string_view& fileName)
    {
        std::ifstream file(fileName.data(), std::ios::in);

        if (file)
        {
            std::string tmpline;
            std::stringstream ist;
            while (std::getline(file, tmpline))
            {
                if (tmpline[0] == commentSymbol && tmpline[1] == commentSymbol)
                    continue;
                ist << tmpline;

            }

            return ist.str();
        }
        else
            throw std::runtime_error("GetFileSource: error");
    }
}