#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP


#include "dimension_node.hpp"


#include <iostream>




template <typename T>
class LinkedList
{
    public:
        int size;
        DimensionNode<T>* head; 
        DimensionNode<T>* tail; 

        LinkedList() : size(0), head(nullptr), tail(nullptr) {};
        ~LinkedList() = default;
        
        int getSize() const { return size; };
};




#endif