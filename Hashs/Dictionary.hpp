#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include<cstddef>

template<typename K, typename V>
class Dictionary{
    public:
        virtual ~Dictionary() = default;

        virtual bool add(const K& key, const V& value) = 0;

        virtual bool modify(const K& key, const V& newValue) = 0;

        virtual V get(const K& key) = 0;

        virtual bool remove(const K& key) = 0;

        virtual bool contains(const K& key) = 0;

        virtual std::size_t size() const = 0;

        virtual void clear() = 0;

        virtual std::size_t getKeyComparisons() const = 0;

        virtual void resetMetrics() = 0;
};

#endif // DICTIONARY_HPP