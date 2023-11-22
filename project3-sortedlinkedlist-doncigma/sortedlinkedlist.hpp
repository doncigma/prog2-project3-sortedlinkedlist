/// @file sortedlinkedlist.hpp
/// @brief Two sorted linked list implementations - one with comparison function as a property and another by derived class specifying it in a virtual function

#pragma once

#include "sortedlinkedlistbase.hpp"
#include <vector>

/// @brief A sorted linked list implementation taking the comparison function as a property
template <typename T>
class SortedLinkedListProperty
    : public SortedLinkedListBase<T>
{
public:
    /// @brief Constructor - sets the initial state to be empty and self-consistent.
    /// @param compare The comparison function to use - must return -1 if left < right, 0 if left == right, 1 if left > right
    SortedLinkedListProperty(int (*compare)(const T &left, const T &right))
        : SortedLinkedListBase<T>()
    {
        _compareFunc = compare;
        _ascending = true;
    }

    /// @brief Set the comparison function to use and resorts the existing elements.
    /// @param compare The new comparison function.
    /// Hint: You may need to expose another method on the base class to handle this or expose more of the properties if you do not want to copy the objects.
    /// You can do it either way but if you can avoid copying, even more points.
    void SetCompareFunction(int (*compare)(const T &left, const T &right))
    {
        _compareFunc = compare;
        Resort();
    }

    /// @brief Set the order of the list - ascending or descending
    /// @param ascending Set to true to be in ascending order, false for descending order
    void SetOrder(bool ascending)
    {
        _ascending = ascending;
        Resort();
    }

    void Resort() {
        std::vector<T> newList;
        
        for (int i = 0; i < LinkedListBase<T>::Size(); i++) { 
            newList.push_back(LinkedListBase<T>::Get(i));
        }
        LinkedListBase<T>::Clear();

        for (size_t i = 0; i < newList.size(); i++) { 
            SortedLinkedListBase<T>::Insert(newList[i]);
        }
    }

protected:
    /// @brief If _compareFunc is null, then call base class method.  Otherwise, use _compareFunc and SetOrder (if you do the extra credit) to sort.
    /// @param left left element
    /// @param right right element
    /// @return true if left < right, false otherwise
    virtual bool IsLessThan(const T &left, const T &right) const
    {
        bool isLessThan;
        if (_compareFunc == NULL) {
            isLessThan = SortedLinkedListBase<T>::IsLessThan(left, right);
        }
        else {
            isLessThan = _compareFunc(left, right) == -1;
        }
        
        if (!_ascending) {
            isLessThan = !isLessThan;
        }
        return isLessThan;
    }

private:
    int (*_compareFunc)(const T &left, const T &right);
    bool _ascending;
};


/// @brief A sorted linked list implementation taking the comparison function by overloading a virtual function
template <typename T>
class SortedLinkedList
    : public SortedLinkedListBase<T>
{
public:
};
