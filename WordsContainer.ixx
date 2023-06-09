//#pragma once
//
//#include "Word.hpp"
//
//#include <string>
//#include <type_traits>
//#include <vector>
export module WordsTrainer.WordsContainer;

import WordsTrainer.Types;

import <string>;
import <type_traits>;
import <vector>;

export namespace WordsTrainer
{
    //W - word
    //TW - translated word
    template<_Word W, _Word TW>
    struct WordsContainer
    {
    public:
        WordsContainer() = default;
        WordsContainer(const W& word, const TW& translation) noexcept
            : m_Word(word), m_Translation(translation) {}
        WordsContainer(const WordsContainer& other) noexcept
        {
            SetWord(other.m_Word, other.m_Translation);
        }
        WordsContainer(WordsContainer&& other) noexcept
        {
            SetWord(std::move(other.m_Word), std::move(other.m_Translation));
        }
        ~WordsContainer() = default;

        void SetWord(const W& word, const TW& translation) noexcept
        {
            m_Word = word;
            m_Translation = translation;
        }
        void SetWord(W&& word, TW&& translation) noexcept
        {
            std::swap(m_Word, word);
            std::swap(m_Translation, translation);
        }

        const W& GetWord() const noexcept { return m_Word; }
        const TW& GetTranslation() const noexcept { return m_Translation; }

        WordsContainer& operator=(const WordsContainer& rhs) noexcept
        {
            if (this == &rhs)
                return *this;
            m_Word = rhs.m_Word;
            m_Translation = rhs.m_Translation;
            return *this;
        }
        friend bool operator==(const WordsContainer& lhs, const WordsContainer& rhs) noexcept
        {
            return (lhs.m_Word == rhs.m_Word && lhs.m_Translation == rhs.m_Translation);
        }
        friend bool operator!=(const WordsContainer& lhs, const WordsContainer& rhs) noexcept
        {
            return !(lhs == rhs);
        }
    private:
        W m_Word;
        TW m_Translation;
    };
    using Word = WordsContainer<Word_, Word_>;
    using SynonymsWord = WordsContainer<Synonym, Synonym>;
    using WSynonymsWords = WordsContainer<Synonym, Word_>;
    using TSynonymsWords = WordsContainer<Word_, Synonym>;

    template<typename W>
        requires (std::is_same_v<W, Word> || std::is_same_v<W, SynonymsWord>
    || std::is_same_v<W, WSynonymsWords> || std::is_same_v<W, TSynonymsWords>)
        using Vocabulary = std::vector<W>;
}