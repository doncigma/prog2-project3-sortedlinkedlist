/// @file linkedlistbase.hpp
/// @brief A common base class for our linked list implementation
/// @details This file contains the implementation of a simple linked list data structure.
/// Each element of the list is represented by a node that contains a value and a pointer to the next node.
/// The list is implemented as a template class, so it can hold elements of any type.
/// The list is implemented as a singly linked list, so it can only be traversed in one direction.
/// No one is expected to use this class but instead should derive from it.
#pragma once

#include <stdexcept>

/// @brief Exception class for linked list errors.  Allows us to catch known errors for our implementation.
class LinkedListException : public std::exception
{
private:
    const char *message;

public:
    LinkedListException(const char *msg) : message(msg) {}
    const char *what() const noexcept override
    {
        return message;
    }
};

/// @brief A basic linked list implementation
template <typename T>
class LinkedListBase
{
public:
    /// @brief Destructor - cleans up all memory allocated by this class
    ~LinkedListBase()
    {
        Clear();
    }

    /// @brief Function to remove an element at a specific position
    /// @param position The position of the element to remove
    /// @throws LinkedListException if the position is invalid
    void RemoveAt(int position)
    {
        if (position < 0 || position >= Size())
        {
            throw LinkedListException("Invalid position");
        }

        if (position == 0)
        {
            Node *ptr = _head;
            _head = _head->next;
            delete ptr;
            _size--;
            if (_size == 0)
            {
                _tail = NULL;
            }
        }
        else
        {
            Node *prev = GetNodeAt(position - 1);
            Node *temp = prev->next;
            prev->next = temp->next;
            delete temp;
            _size--;

            if (position == Size())
            {
                _tail = prev;
            }
        }
    }

    /// @brief Function to get the size of the linked list
    /// @return The size of the linked list
    int Size() const
    {
        return _size;
    }

    /// @brief Function to check if the linked list is empty
    /// @return True if the linked list is empty, false otherwise
    bool Empty() const
    {
        return _size == 0;
    }

    /// @brief Function to clear the linked list
    void Clear()
    {
        Node *ptr = _head;
        _head = NULL;
        _tail = NULL;
        _size = 0;

        while (ptr != NULL)
        {
            Node *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }

    /// @brief Function to get the element at a specific position
    /// @param position The position of the element to get
    /// @return The element at the specified position
    /// @throws LinkedListException if the position is invalid
    T Get(int position) const
    {
        if (position < 0 || position >= Size())
        {
            throw LinkedListException("Invalid position, Get()");
        }

        return GetNodeAt(position)->data;
    }

    /// @brief Function to get the element at a specific position
    /// @param position The position of the element to get
    /// @return The element at the specified position
    /// @throws LinkedListException if the position is invalid
    T operator[](int position) const
    {
        return Get(position);
    }

    /// @brief Function to find an element that satisfies a predicate
    /// @tparam Predicate The predicate function should take a const reference to the data type stored in the list and return a bool
    /// @param pred The predicate to apply to each element in the list.  Hint: pred(value) will apply the predicate to the value and return a bool.
    /// @return The first element that satisfies the predicate
    /// @throws LinkedListException if no element satisfies the predicate
    template <typename Predicate>
    T Find(Predicate pred) const
    {
        Node *ptr = _head;
        while (ptr)
        {
            if (pred(ptr->data))
            {
                return ptr->data;
            }
            ptr = ptr->next;
        }

        throw LinkedListException("Not found");
    }

    /// @brief Finds the index of the first element in the list that satisfies the given predicate.
    /// @tparam Predicate The predicate function should take a const reference to the data type stored in the list and return a bool
    /// @param pred The predicate to apply to each element in the list.  Hint: pred(value) will apply the predicate to the value and return a bool.
    /// @return The index of the first element in the list that satisfies the predicate.
    /// @throws LinkedListException if no element in the list satisfies the predicate.
    template <typename Predicate>
    int FindIndex(Predicate pred) const
    {
        Node *ptr = _head;
        int index = 0;
        while (ptr)
        {
            if (pred(ptr->data))
            {
                return index;
            }
            ptr = ptr->next;
            index++;
        }

        throw LinkedListException("Not found");
    }

    /// @brief Applies a function to each element of the linked list.
    /// @tparam Function The function should take a const reference to the data type stored in the list and return void.
    /// @param func The function to apply.  Hint func(value) will apply the function to the value.
    template <typename Function>
    void ForEach(Function func) const
    {
        Node *ptr = _head;
        while (ptr)
        {
            func(ptr->data);

            ptr = ptr->next;
        }
    }

private:
    /// @brief Node class
    class Node
    {
    public:
        T data;     ///< The data stored in the node
        Node *next; ///< Pointer to the next node

        /// @brief Constructor that copies the value into the node.  Template type must support copy constructor.
        /// @param value The value to be copied into the node
        Node(const T &value) : data(value), next(NULL) {}
    };

    Node *_head; ///< Pointer to the first node
    Node *_tail; ///< Pointer to the last node
    int _size;   ///< The number of elements in the list

protected:
    /// @brief Constructor - sets the initial state to be empty and self-consistent.
    LinkedListBase()
    {
        _head = NULL;
        _tail = NULL;
        _size = 0;
    }
    
    /// @brief Function to get a pointer to the node at a specific position.
    /// @param position The position of the node to get
    /// @return A pointer to the node at the specified position or NULL if the position is invalid (or empty list) or head if negative.
    Node *GetNodeAt(int position) const
    {
        int i = 0;
        Node *ptr = _head;

        while (ptr != NULL && i < position)
        {
            ptr = ptr->next;
            i++;
        }

        return ptr;
    }

    /// @brief Function to insert a new element at a specific position - 0 based
    /// @param value The value to be inserted
    /// @param position The position to insert the value at. 0 prepends and Size() appends.
    /// @throws LinkedListException if the position is invalid
    void InsertAt(const T &value, int position)
    {
        if (position < 0 || position > Size())
        {
            throw LinkedListException("Invalid position, InsertAt()");
        }

        if (position == 0)
        {
            Prepend(value);
        }
        else if (position == Size())
        {
            Append(value);
        }
        else
        {
            Node *newNode = new Node(value);
            // Get the node before the position we want to add at.
            Node *ptr = GetNodeAt(position - 1);
            newNode->next = ptr->next;
            ptr->next = newNode;
            _size++;
        }
    }

    /// @brief Function to add a new element to the end of the list
    /// @param value The value to be added
    void Append(const T &value)
    {
        Node *newNode = new Node(value);

        if (Empty())
        {
            // This copies the value into the node.  Template type must support copy constructor.
            _head = newNode;
            _tail = newNode;
            _size++;
        }
        else
        {
            _tail->next = newNode;
            _tail = newNode;
            _size++;
        }
    }

    /// @brief Function to add a new element to the beginning of the list
    /// @param value The value to be added
    void Prepend(const T &value)
    {
        Node *newNode = new Node(value);

        if (Empty())
        {
            // This copies the value into the node.  Template type must support copy constructor.
            _head = newNode;
            _tail = newNode;
            _size++;
        }
        else
        {
            newNode->next = _head;
            _head = newNode;
            _size++;
        }
    }
};
