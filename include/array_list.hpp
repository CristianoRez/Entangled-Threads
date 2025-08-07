#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP


#include "utils.hpp"


#include <iostream>  
#include <stdexcept>




template <typename T>
class ArrayList 
{
    private:
        int _size;   
        int _maxSize;
        T* _items;    
        
        void swap(ArrayList<T>& other);

    public:
        ArrayList();
        explicit ArrayList(int maxSize);
        ArrayList(const ArrayList<T>& other);
        ArrayList(ArrayList<T>&& other) noexcept;
        ~ArrayList();

        ArrayList<T>& operator=(const ArrayList<T>& other);
        ArrayList<T>& operator=(ArrayList<T>&& other) noexcept;

        // Acesso e estado do vetor
        T* data();
        const T* data() const;
        int getSize() const;
        int getCapacity() const;
        bool isEmpty() const;

        // Manipulação de _items
        T getItem(int pos) const;
        void setItem(const T& item, int pos);
        void setItem(T&& item, int pos);
        void insertAtEnd(const T& item);
        void insertAtPosition(const T& item, int pos);
        T removeFromPosition(int pos);

        // Operador de Acesso
        T& operator[](int index);
        const T& operator[](int index) const;

        // Utilitários
        void resize(int newSize);
        int search(const T& key) const;
        void clear();
};




// Construtores padrão
template <typename T>
ArrayList<T>::ArrayList() : _size(0), _maxSize(10), _items(new T[10]) {}



// Construtor com tamanho
template <typename T>
ArrayList<T>::ArrayList(int maxSize) {
    if (maxSize < 0) {
        throw std::invalid_argument("Max size cannot be negative.");
    }
    _maxSize = maxSize;
    _size = 0;
    _items = (maxSize > 0) ? new T[_maxSize] : nullptr;
}



// Construtor de cópia
template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) : _size(other._size), _maxSize(other._maxSize) {
    _items = (other._maxSize > 0) ? new T[other._maxSize] : nullptr;
    for (int i = 0; i < _size; i++) {
        _items[i] = other._items[i];
    }
}



// Construção de cópia por movimentação
template <typename T>
ArrayList<T>::ArrayList(ArrayList<T>&& other) noexcept
    : _size(other._size), _maxSize(other._maxSize), _items(other._items) {
    other._items = nullptr; 
    other._size = 0;     
    other._maxSize = 0;
}




template <typename T>
ArrayList<T>::~ArrayList() {
    delete[] _items;
}



// Implementação da função swap local
template<typename T>
void ArrayList<T>::swap(ArrayList<T>& other) {
    // Trocando size
    int temp_size = _size;
    _size = other._size;
    other._size = temp_size;

    // Trocando maxSize
    int temp_maxSize = _maxSize;
    _maxSize = other._maxSize;
    other._maxSize = temp_maxSize;

    // Trocando o ponteiro dos itens
    T* temp__items = _items;
    _items = other._items;
    other._items = temp__items;
}




template <typename T>
T* ArrayList<T>::data() { return _items; }




template <typename T>
const T* ArrayList<T>::data() const { return _items; }




template <typename T>
int ArrayList<T>::getSize() const { return _size; }




template <typename T>
int ArrayList<T>::getCapacity() const { return _maxSize; }




template <typename T>
bool ArrayList<T>::isEmpty() const { return _size == 0; }




template <typename T>
T ArrayList<T>::getItem(int pos) const {
    if (pos < 0 || pos >= _size)
    throw std::out_of_range("Invalid position: getItem");
    return _items[pos];
}




template <typename T>
void ArrayList<T>::setItem(const T& item, int pos) {
    if (pos < 0 || pos > _size || pos >= _maxSize)
    throw std::out_of_range("Invalid position: setItem (copy)");
    
    _items[pos] = item;
    if (pos == _size) {
        _size++;
    }
}




template <typename T>
void ArrayList<T>::setItem(T&& item, int pos) {
    if (pos < 0 || pos > _size || pos >= _maxSize)
        throw std::out_of_range("Invalid position: setItem (move)");
    
    _items[pos] = my_move(item); 
    if (pos == _size) {
        _size++;
    }
}




template <typename T>
void ArrayList<T>::insertAtEnd(const T& item) {
    if (_size == _maxSize) {
        int newCapacity = (_maxSize > 0) ? (_maxSize * 2) : 10; 
        this->resize(newCapacity);
    }
    _items[_size++] = item;
}




template <typename T>
void ArrayList<T>::insertAtPosition(const T& item, int pos) {
    if (_size == _maxSize)
        this->resize(this->getSize() * 2);
    if (pos < 0 || pos > _size)
        throw std::out_of_range("Invalid position insertAtPosition");

    for (int i = _size; i > pos; i--)
        _items[i] = _items[i - 1];

    _items[pos] = item;
    _size++;
}




template <typename T>
T ArrayList<T>::removeFromPosition(int pos) {
    if (pos < 0 || pos >= _size)
        throw std::out_of_range("Invalid position removeFromPosition");
    
    T temp = my_move(_items[pos]);
    for (int i = pos; i < _size - 1; i++)
    _items[i] = my_move(_items[i + 1]);
    _size--;
    return temp;
}




template <typename T>
T& ArrayList<T>::operator[](int index) {
    if (index < 0 || index >= _maxSize) {
        throw std::out_of_range("Invalid index: operator[] is out of capacity.");
    }
    return _items[index];
}




template <typename T>
const T& ArrayList<T>::operator[](int index) const {
    if (index < 0 || index >= _maxSize) {
        throw std::out_of_range("Invalid index: const operator[] is out of capacity.");
    }
    return _items[index];
}




template <typename T>
void ArrayList<T>::resize(int newSize) {
    if (newSize < 0) {
        throw std::invalid_argument("New size cannot be negative for resize.");
    }
    if (newSize == _maxSize) {
        return;
    }

    T* new_items = (newSize > 0) ? new T[newSize] : nullptr;
    int limit = (newSize < _size) ? newSize : _size;
    for (int i = 0; i < limit; i++) {
        new_items[i] = my_move(_items[i]);
    }
    delete[] _items;
    _items = new_items;
    _maxSize = newSize;
    if (_size > newSize)
        _size = newSize;
}




template <typename T>
int ArrayList<T>::search(const T& key) const {
    for (int i = 0; i < _size; i++) {
        if (_items[i] == key) 
            return i;
    }
    return -1;
}




template <typename T>
void ArrayList<T>::clear() {
    _size = 0;
}




template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    if (this != &other) {
        ArrayList<T> temp(other);
        this->swap(temp);
    }
    return *this;
}




template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& other) noexcept {
    if (this != &other) {
        delete[] _items; 
        _size = other._size;
        _maxSize = other._maxSize;
        _items = other._items;
        other._items = nullptr; 
        other._size = 0;       
        other._maxSize = 0;
    }
    return *this;
}




#endif