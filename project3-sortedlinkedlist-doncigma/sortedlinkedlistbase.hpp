/// @file sortedlinkedlistbase.hpp
/// @brief A sorted linked list base implementation

#pragma once

#include <algorithm>
#include "linkedlistbase.hpp"

/// @brief A sorted linked list base implementation
template <typename T>
class SortedLinkedListBase
    : public LinkedListBase<T>
{
public:
    /// @brief Function to insert a new element into the list in sorted order
    /// @param value The value to be added
    void Insert(const T &valueToAdd)
    {
        int i = 0;
        while (i < LinkedListBase<T>::Size()) {
            if (this->IsLessThan(valueToAdd, LinkedListBase<T>::Get(i))) {
                break;
            }
            i++;
        }
        LinkedListBase<T>::InsertAt(valueToAdd, i);
    }

protected:
    /// @brief Base implementation of IsLessThan - uses < operator on T
    /// @param left left element
    /// @param right right element
    /// @return true if left < right, false otherwise
    virtual bool IsLessThan(const T &left, const T &right) const
    {
        return left < right;
    }
};
