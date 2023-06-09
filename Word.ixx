//#pragma once
//
//#include <string>
//#include <vector>
//#include <type_traits>
export module WordsTrainer.Types;

import <string>;
import <vector>;
import <type_traits>;

export namespace WordsTrainer
{
    using Word_ = std::string;
    using Synonym = std::vector<Word_>;

    template<typename W>
    concept _Word = (std::is_same_v<W, Synonym> || std::is_same_v<W, std::string>);

    constexpr inline const char synonimSymbol = '|';
    constexpr inline const char translationSymbol = '=';
    constexpr inline const char newLineSymbol = ';';
    constexpr inline const char commentSymbol = '/';//it must be doubled
}