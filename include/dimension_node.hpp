/**********************************************************************************
 *
 * FILE:            dimension_node.hpp
 * 
 * ********************************************************************************
 *
 * PROJECT:         Project Name (Entangled Threads Structure Algorithm)
 * 
 * ********************************************************************************
 *
 * DESCRIPTION:
 * For the Entangle Threads Structure to function, it is essential that the nodes 
 * possess a mechanism through which, from that object, one can access the various 
 * dimensions in which it exists and move within them.
 * 
 * To this end, a hash structure of the form hash<string, pointers> was developed, 
 * enabling constant-time access to the location of an object within a given 
 * dimension based on the dimension's name.
 * 
 * ********************************************************************************
 * 
 * COPYRIGHT NOTICE:
 * Copyright (c) 2025 Cristiano Rezende Sá Miranda Gonçalves
 * 
 * This software is released under the MIT License.
 * You should have received a copy of the MIT License along with this program. 
 * If not, see <https://opensource.org/license/mit>.
 * 
 * SPDX-License-Identifier: MIT
 **********************************************************************************/

#ifndef DIMENSION_NODE_HPP
#define DIMENSION_NODE_HPP

#include "hash.hpp"

#include <iostream>




template <typename T>
class DimensionNode;



template <typename T>
struct DimensionPointers {
    DimensionNode<T>* next;
    DimensionNode<T>* prev;

    DimensionPointers() : next(nullptr), prev(nullptr) {};

    ~DimensionPointers() = default;
};




template <typename T>
class DimensionNode {
    public:
        T item;
        using Pointers = DimensionPointers<T>;
        Hash<std::string, Pointers> dimension;

        DimensionNode() : item(-1), dimension(4) {};

        ~DimensionNode() = default;
};




#endif