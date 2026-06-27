#ifndef HASH_CHAINING_HPP
#define HASH_CHAINING_HPP

#include <algorithm>
#include <iostream>
#include<vector>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <cstddef>

#include "Dictionary.hpp"


template<typename K, typename V >
struct ChainNode{
    K key;
    V value;
    ChainNode<K, V>* next = nullptr;

    ChainNode(K key, V value){
        this->key = key;
        this->value = value;
    }
};

template<typename K, typename V, typename Hash = std::hash<K>>
class HashChaining : public Dictionary<K, V>
{
private:
    int capacity;
    int count = 0;

    int keyComparisons = 0;

    Hash hasher;

    std::vector<ChainNode<K, V>*> content;

    bool isPrime(int n) const{
            if(n <= 1) return false;
            if(n == 2 || n == 3) return true;
            if((n % 2 == 0) || (n % 3 == 0)) return false;

            int root = std::sqrt(n);
            for(int i = 5; i <= root; i+=6){
                if((n % i == 0) || (n % (i+2) == 0) ) return false;
            }
            return true;
        }

        int searchPrime(int n) const{
            while (!isPrime(n))
            {
                n++;
            }
            return n;
        }

        size_t hash(const K& key) const{
            size_t hashCode = hasher(key);
            return hashCode % capacity;
        }

        float loadFactor(){
            return static_cast<float>(count) / capacity;
        }

        void reHashing(){
            int oldCapacity = this->capacity;
            std::vector<ChainNode<K,V>*> oldContent = this->content;

            this->capacity = searchPrime(capacity*2);

            this->content.resize(capacity);
            this->count = 0;

            for(auto &bucket : this->content) bucket = nullptr;

            for(int i = 0; i < oldCapacity;i++){
                if(oldContent[i] != nullptr){
                    ChainNode<K, V>* current = oldContent[i];

                    while (current != nullptr)
                    {
                        add(current->key, current->value);
                        ChainNode<K, V>* toDelete = current;
                        current = current->next;
                        delete toDelete;
                    }
                    
                }
            }

        }



public:
    HashChaining(const HashChaining&) = delete;
    HashChaining& operator=(const HashChaining&) = delete;

    HashChaining(int capacity){
        this->capacity = searchPrime(capacity);

        content.resize(this->capacity);

        for (auto &current : content){
            current = nullptr;
        }
        
    };

    bool add(K key){
        return add(key, 1);
    }

    bool add(const K& key, const V& value) override {
        if(loadFactor() > 1){
            reHashing();
        }

        size_t index = hash(key);

        ChainNode<K,V>* current = content[index];

        while (current != nullptr){
            this->keyComparisons += 1;
            if(current->key == key){
                current->value += value;
                return true;
            }

            current = current->next;
        }

        ChainNode<K,V>* newNode = new ChainNode<K,V>(key, value);
        newNode->next = content[index];
        content[index] = newNode;
        count++;
        return true;
    }

    bool modify(const K& key, const V& newValue) override {
        size_t index = hash(key);

        ChainNode<K, V>* current = content[index];

        while (current != nullptr)
        {
            this->keyComparisons += 1;
            if(current->key == key){
                current->value = newValue;
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool contains(const K& key) override {
        size_t index = hash(key);

        ChainNode<K, V>* current = content[index];

        while (current != nullptr)
        {
            this->keyComparisons += 1;
            if(current->key == key){
                return true;
            }
            current = current->next;
        }

        return false;
    }

    std::size_t size() const override {
        return this->count;
    }

    V get(const K& key) override {
        int index = hash(key);

        ChainNode<K, V>* current = content[index];

        while (current != nullptr)
        {
            this->keyComparisons += 1;
            if(current->key == key){
                return current->value;
            }
            current = current->next;
        }

        throw std::out_of_range("Chave nao encontrada");
    }

    bool remove(const K& key) override {
        int index = hash(key);

        ChainNode<K, V>* current = content[index];

        ChainNode<K, V>* previous = nullptr;

        while (current != nullptr)
        {
            this->keyComparisons += 1;
            if(current->key == key){
                current->value -= 1;
                if(current->value > 0){
                    return true;                 // só decrementa, mantem o no
                }

                // value chegou a 0 -> remove o no de fato
                if (previous == nullptr)
                {
                    content[index] = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
                count--;
                return true;
            }
            previous = current;
            current = current->next;
        }

        return false;


    }

    std::size_t getKeyComparisons() const override {
        return this->keyComparisons;
    }

    void resetMetrics() override {
        this->keyComparisons = 0;
    }

    void clear() override {
        for(int i = 0; i < capacity; i++){
            ChainNode<K, V>* current = content[i];
            while (current != nullptr)
            {
                ChainNode<K, V>* toDelete = current;
                current = current->next;
                delete toDelete;
            }
            content[i] = nullptr;
        }
        count = 0;
    }

    ~HashChaining() override {
        clear();
    }

    void print(){
      for(int i = 0; i < this->content.size(); i++){
        ChainNode<K, V>* e = this->content.at(i);
        while(e != nullptr){
          std::cout << e->key << ": " << e->value << std::endl;
          e = e->next;
        }
      }
    }
    
    std::vector<K> getKeys(){
      std::vector<K> v;
      for(int i = 0; i < this->content.size(); i++){
        ChainNode<K, V>* e = this->content.at(i);
        while(e != nullptr){
          v.push_back(e->key);
          e = e->next;
        }
      }
      std::sort(v.begin(), v.end());
      return v;
    }
};



#endif
