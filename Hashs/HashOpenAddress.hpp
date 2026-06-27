#ifndef HASH_OPEN_ADDRESS_HPP
#define HASH_OPEN_ADDRESS_HPP

#include<vector>
#include <cmath>
#include <functional>
#include <stdexcept>

#include "Dictionary.hpp"


enum Status {
    EMPTY,
    ACTIVE,
    DELETED
};


template<typename K, typename V>
struct Entry{
    K key;
    V value;
    Status stats;

    Entry(K key, V value){
        this->key = key;
        this->value = value;
        this->stats = Status::ACTIVE;
    }

    Entry() {
        value = V{};
        stats = Status::EMPTY;
    }
};



template<typename K, typename V, typename Hash = std::hash<K>>
class HashOpenAddress : public Dictionary<K, V>{

    private:
        int capacity;
        int count;
        std::vector<Entry<K, V>> content;

        int const_R;

        int keyComparisons = 0;

        Hash hasher;

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

        size_t doubleHash(K key) const{
            size_t hashCode = hasher(key);
            return const_R - (hashCode % const_R);
        }

        size_t coPrime(int newCapacity){
            if(newCapacity <= 2) return 1;
            for(int i = newCapacity - 1; i > 2; i--){
                if(isPrime(i)) return i;
            }
            return 1;
        }

        float loadFactor(){
            return static_cast<float>(count) / capacity;
        }

        void reHashing(){
            int oldCapacity = capacity;
            std::vector<Entry<K, V>> oldContent = std::move(content);
            capacity = searchPrime(oldCapacity*2);

            const_R = coPrime(capacity);



            content.resize(capacity);
            count = 0;

            for(int i = 0; i < oldCapacity; i++){
                if(oldContent[i].stats == Status::ACTIVE){
                    add(oldContent[i].key, oldContent[i].value);
                }
            }

        }


    public:
        
        HashOpenAddress(int m = 127){
            capacity = searchPrime(m);
            count = 0;
            content.resize(capacity);

            const_R = coPrime(capacity);

        }



    bool add(const K& key) {
        return add(key, 1);
    }

    bool add(const K& key, const V& value) override {
        if (loadFactor() >= 0.7) reHashing();

        size_t indexInicial = hash(key);
        size_t passo = doubleHash(key);

        int firstDeleted = -1;

        for (int trial = 0; trial < capacity; trial++) {
            size_t index = (indexInicial + trial * passo) % capacity;

            if (content[index].stats == Status::EMPTY) {
                // chave nao existe -> insere na lapide guardada, ou aqui mesmo
                size_t target = (firstDeleted != -1) ? (size_t)firstDeleted : index;
                content[target] = Entry<K, V>(key, value);
                this->count += 1;
                return true;
            }

            if (content[index].stats == Status::DELETED) {
                if (firstDeleted == -1) firstDeleted = index;  // lembra a 1a lapide, mas NAO insere ainda
                continue;
            }

            // ACTIVE
            this->keyComparisons+=1;
            if (content[index].key == key) {
                content[index].value += value;
                return true;
            }
        }

        // sondou tudo sem achar EMPTY: usa a lapide se houver
        if (firstDeleted != -1) {
            content[firstDeleted] = Entry<K, V>(key, value);
            this->count += 1;
            return true;
        }
        return false;
    }

    V get(const K& key) override {
        size_t indexInicial = hash(key);
        size_t passo = doubleHash(key);

        for(int trial = 0; trial < capacity; trial++){
            size_t index = (indexInicial + trial * passo) % capacity; 
            if(content[index].stats == Status::EMPTY){
                throw std::out_of_range("Erro: A chave procurada não existe na tabela.");
            }
            if(content[index].stats == Status::ACTIVE){
                this->keyComparisons+=1;
                if (content[index].key == key){
                    return content[index].value;
                }
                else continue;
            }
        }
        throw std::out_of_range("Erro: A chave procurada não existe na tabela.");
    }

    bool modify(const K &key, const V &newValue) override {
        size_t indexInicial = hash(key);
        size_t passo = doubleHash(key);

        for(int trial = 0; trial < capacity; trial++){
            size_t index = (indexInicial + trial * passo) % capacity; 
            if(content[index].stats == Status::EMPTY){
                return false;
            }
            if(content[index].stats == Status::ACTIVE){
                this->keyComparisons+=1;
                if (content[index].key == key){
                    content[index].value = newValue;
                    return true;
                }
                else continue;
            }
        }
        return false;
    }

    bool remove(const K& key) override {
        size_t indexInicial = hash(key);
        size_t passo = doubleHash(key);

        for(int trial = 0; trial < capacity; trial++){
            size_t index = (indexInicial + trial * passo) % capacity; 
            if(content[index].stats == Status::EMPTY){
                return false;
            }
            if(content[index].stats == Status::ACTIVE){
                this->keyComparisons+=1;
                if (content[index].key == key){
                    content[index].value -= 1;
                    if(content[index].value <= 0){
                        content[index].value = V{};
                        content[index].stats = Status::DELETED;
                        count-=1;
                    }
                    return true;
                }
                else continue;
            }
        }
        return false;
    }

    bool contains(const K& key) override {
        size_t indexInicial = hash(key);
        size_t passo = doubleHash(key);

        for(int trial = 0; trial < capacity; trial++){
            size_t index = (indexInicial + trial * passo) % capacity; 
            if(content[index].stats == Status::EMPTY){
                return false;
            }
            if(content[index].stats == Status::ACTIVE){
                this->keyComparisons+=1;
                if (content[index].key == key){
                    return true;
                }
                else continue;
            }
        }
        return false;
    }

    std::size_t size() const override {
        return this->count;
    }

    void clear() override {
        this->count = 0;

        for(int i = 0; i < capacity; i++){
            content[i].stats = Status::EMPTY;
        }
    }

    std::size_t getKeyComparisons() const override {
        return this->keyComparisons;
    }

    void resetMetrics() override {
        this->keyComparisons = 0;
    }

    
};

#endif // HASH_OPEN_ADDRESS_HPP