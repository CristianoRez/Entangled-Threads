/*
* Implementação da função hash com size_t para não ter problema de tamanho da chave
* e sondagem quadrática para evitar colisões.
*/ 

#ifndef HASH_HPP
#define HASH_HPP


#include "array_list.hpp" 
#include "utils.hpp" 


#include <string>   
#include <iostream>




bool isPrime(size_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (size_t i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}




size_t findNextPrime(size_t n) {
    if (n <= 1) return 2;
    size_t prime = n;
    bool found = false;
    while (!found) {
        prime++;
        if (isPrime(prime))
            found = true;
    }
    return prime;
}




enum class SlotState { EMPTY, OCCUPIED, TOMBSTONE };

template <typename T>
struct EqualTo {
    bool operator()(const T& a, const T& b) const { return a == b; }
};

// Obrigando a especialização so template para poder lidar com int e string como chave
template <typename T> 
struct Hasher;

// Especialização de int para lidar com id do pacote:
template <> 
struct Hasher<int> { 
    size_t operator()(int k) const { 
        return k; 
    } 
};

// Especialização de string para lidar com nome do cliente:
template <> 
struct Hasher<std::string> {
    size_t operator()(const std::string& key) const {
        // Algoritmo DJB2 para transformar strings em chaves
        size_t hash = 5381;
        for (char c : key) { 
            hash = ((hash << 5) + hash) + c; 
        }
        return hash;
    }
};

// Especialização para size_t para evitar números negativos e limitação de tamanho:
template <> 
struct Hasher<size_t> { 
    size_t operator()(size_t k) const { 
        return k; 
    } 
};



/* Tabela hash com endereçamento aberto:
*  KeyType - tipo da chave
*  ValueType - valor associado a chave
*  HasherType - Como gera a chave
*  KeyEqualType - Como compara duas chaves
*/
template <
    typename KeyType,
    typename ValueType,
    typename HasherType = Hasher<KeyType>,
    typename KeyEqualType = EqualTo<KeyType>
>
class Hash 
{
    private:
        // Cada slot armazena as três informações abaixo
        struct HashSlot {
            KeyType key;
            ValueType value;
            SlotState state = SlotState::EMPTY;
        };

        ArrayList<HashSlot> _vector;
        size_t _size = 0;
        // Sempre que chega a lotação abaixo é dobrado o tamanho da tabela
        float _maxCapacity = 0.7f;
        HasherType _hasher;
        KeyEqualType _keyEqual;


        size_t findPos(const KeyType& key) const;
        void rehash();

    public:
        explicit Hash(size_t initialSize = 3);
        bool insert(const KeyType& key, const ValueType& value);
        bool erase(const KeyType& key);
        bool contains(const KeyType& key) const;
        void clear();
        size_t size() const;
        bool empty() const;
        ValueType& operator[](const KeyType& key);
};




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
Hash<KeyType, ValueType, HasherType, KeyEqualType>::Hash(size_t initialSize) 
    : _vector(findNextPrime(initialSize > 0 ? initialSize : 3)) {}



// Algoritmo de sondagem quadrática para evitar colisões
template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
size_t Hash<KeyType, ValueType, HasherType, KeyEqualType>::findPos(const KeyType& key) const {
    size_t tableCapacity = _vector.getCapacity();
    if (tableCapacity == 0) return 0;

    size_t originalIndex = _hasher(key) % tableCapacity;
    size_t tombstonePos = -1;

    for (size_t i = 0; i < tableCapacity; i++) 
    {
        size_t index = (originalIndex + i * i) % tableCapacity;

        if (_vector[index].state == SlotState::EMPTY)
            return (tombstonePos != (size_t)-1) ? tombstonePos : index;

        if (_vector[index].state == SlotState::TOMBSTONE) 
        {
            if (tombstonePos == (size_t)-1) tombstonePos = index;
        } else if (_keyEqual(_vector[index].key, key)) {
            return index;
        }
    }
    return tombstonePos != (size_t)-1 ? tombstonePos : originalIndex;
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
void Hash<KeyType, ValueType, HasherType, KeyEqualType>::rehash() {
    ArrayList<HashSlot> oldTable = my_move(_vector);
    size_t oldCapacity = oldTable.getCapacity();
    size_t newCapacity = findNextPrime(oldCapacity * 2);
    _vector = ArrayList<HashSlot>(newCapacity);
    _size = 0;

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == SlotState::OCCUPIED) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
bool Hash<KeyType, ValueType, HasherType, KeyEqualType>::insert(const KeyType& key, const ValueType& value) {
    if (_vector.getCapacity() == 0 || _size >= _vector.getCapacity() * _maxCapacity) rehash();

    size_t pos = findPos(key);

    if (_vector[pos].state == SlotState::OCCUPIED) return false;

    _vector[pos].key = key;
    _vector[pos].value = value;
    _vector[pos].state = SlotState::OCCUPIED;
    _size++;
    return true;
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
bool Hash<KeyType, ValueType, HasherType, KeyEqualType>::erase(const KeyType& key) {
    if (empty()) return false;

    size_t pos = findPos(key);

    if (_vector[pos].state != SlotState::OCCUPIED) return false;

    _vector[pos].state = SlotState::TOMBSTONE;
    _size--;
    return true;
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
bool Hash<KeyType, ValueType, HasherType, KeyEqualType>::contains(const KeyType& key) const {
    if (empty()) return false;

    size_t pos = findPos(key);

    return _vector[pos].state == SlotState::OCCUPIED;
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
void Hash<KeyType, ValueType, HasherType, KeyEqualType>::clear() {
    _vector.clear();
    _vector.resize(16);
    _size = 0;
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
size_t Hash<KeyType, ValueType, HasherType, KeyEqualType>::size() const { 
    return _size; 
}




template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
bool Hash<KeyType, ValueType, HasherType, KeyEqualType>::empty() const { 
    return _size == 0; 
}



/* Acessa o valor associado a chave:
*  Se estiver ocupado, retorna o que está na posição.
*  Se não, preenche com o novo valor
*/
template <typename KeyType, typename ValueType, typename HasherType, typename KeyEqualType>
ValueType& Hash<KeyType, ValueType, HasherType, KeyEqualType>::operator[](const KeyType& key) {
    if (_vector.getCapacity() == 0 || _size >= _vector.getCapacity() * _maxCapacity) rehash();

    size_t pos = findPos(key);

    if (_vector[pos].state != SlotState::OCCUPIED) {
        _vector[pos].key = key;
        _vector[pos].state = SlotState::OCCUPIED;
        _vector[pos].value = ValueType{};
        _size++;
    }

    return _vector[pos].value;
}




#endif