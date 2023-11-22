/// @file unsortedlinkedlist.hpp
/// @brief An unsorted linked list implementation

#pragma once

#include "linkedlistbase.hpp"

/// @brief A basic unsorted linked list implementation
template <typename T>
class UnsortedLinkedList
    : public LinkedListBase<T>
{
public:
    /// @brief Function to add a new element to the end of the list
    /// @param value The value to be added
    void Append(const T &value)
    {
        LinkedListBase<T>::Append(value);
    }

    /// @brief Function to add a new element to the beginning of the list
    /// @param value The value to be added
    void Prepend(const T &value)
    {
        LinkedListBase<T>::Prepend(value);
    }

    /// @brief Function to insert a new element at a specific position - 0 based
    /// @param value The value to be inserted
    /// @param position The position to insert the value at. 0 prepends and Size() appends.
    /// @throws LinkedListException if the position is invalid
    void InsertAt(const T &value, int position)
    {
        LinkedListBase<T>::InsertAt(value, position);
    }
};
