[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/16s-BjRY)
# Project 3 - Linked List
You will create both a sorted and unsorted linked list data structure .  You will use a templated linked list so you can store any value type.  You will be given the class LinkedListBase in linkedlistbase.hpp to use as the base class.  You can only use its protected or public methods to expose the expected public APIs in the list below.

## UnsortedLinkedList
1. unsortedlinkedlist.hpp is the header which defines the unsortedlinkedlist itself and contains the implementation.
2. You will need the following methods: Append, Prepend, InsertAt, RemoveAt, Clear, Size, Empty, Get, [], Find, FindIndex, ForEach.
3. Some of the methods are already public on the base class and others are not.  Please fill in the methods that are left to implement: Append, Prepend, InsertAt.
## SortedLinkedListBase
1. sortedlinkedlistbase.hpp is the header which defines the common sortedlinkedlistbase and contains the common implementation.
2. You will need the following methods: Insert, RemoveAt, Clear, Size, Empty, Get, [], Find, FindIndex, ForEach.
3. Some of the methods are already public on the base class and others are not.  Please fill in the methods that are left to implement: Insert.
4. Insert will use the protected method IsLessThan to sort the list.  You can use this from within your Insert method.
## SortedLinkedListProperty & SortedLinkedListVirtual & SortedLinkedList
1. SortedLinkedListProperty takes a parameter in the constructor which we save as a property and use for determining sort order.
2. SortedLinkedListVirtual has a public method that can be overrided to provide the sorting function.
3. SortedLinkedList implements the public method to use a standard <, >, = functions of the templated type T.
4. Each of these will override some virtual methods and you must supply those implementations.
## Testing
1. Run "make test" to test the linkedlist class to see if it works.
2. Run "make testdebug" to get debug messages while testing the class.  This will now include line by line messages as it processes each command as well as the file name it is processing.
3. lltest.txt contains the list of tests for UnsortedLinkedList, plltest.txt for SortedLinkedListProperty, and slltest.txt for SortedLinkedList.  You can add new tests yourself or it will indicate where in the file the tests failed and what was expected.
4. You are responsible for ensuring the class always works as the test cases are not exhaustive.
5. You DO NOT need to handle out of memory errors.
## Extra Credit
1. If you want to add the ability to "resort" your list by adding a SetCompareFunction method to SortedLinkedListProperty, then I will give you additional credit.  This would change the function and then resort the current elements to be in the new sort order.
2. If you want to get even more credit, add a SetSortOrder method allowing it to be in ascending or descending order when changed.
3. You will need to write your own tests for these or ask me to write them for you this week.  I did not add them to the project.
