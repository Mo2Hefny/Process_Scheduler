//	This is an updated version of code originally
//  created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** ADT stack: Array-based implementation.
@file ArrayStack.h */

#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"

//#define STACK_SIZE 50
//Unless spesificed by the stack user, the default size is 100
template<typename T>
class ArrayStack : public StackADT<T>
{
private:
	int STACK_SIZE;
	T* items;		// Array of stack items (you can also make it a pointer and allocate it dynamically)
	int      top;                   // Index to top of stack


public:

	ArrayStack(int size = 50)
	{
		STACK_SIZE = size;
		items = new T[STACK_SIZE];
		top = -1;
	}  // end default constructor

	~ArrayStack()
	{
		if (items)
			delete[] items;
	}

	bool isEmpty() const
	{
		return top == -1;
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if (top == STACK_SIZE - 1) return false;	//Stack is FULL

		top++;
		items[top] = newEntry;
		return true;
	}  // end push

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		top--;
		return true;
	}  // end pop

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}  // end peek

	int getSize() const
	{
		return top + 1;
	}

}; // end ArrayStack

#endif
