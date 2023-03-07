/***********************************************
 * StackADT.h
 *
 * Stack ADT defined as an abstract base class.
 *
 * Author: Juan O. Lopez
 **********************************************/

#ifndef STACKADT_H
#define STACKADT_H

template <class Type>
class StackADT
{
	public:
		virtual void push(const Type&) = 0; // add element to the top of the stack
		virtual Type pop() = 0; // remove and return element from top of the stack
		virtual Type top() const = 0; // retrieve element from the top of the stack

		/* Non-virtual functions */
		int size() const;
		bool isEmpty() const;
		void clear(); // empty the stack

	protected:
		int currentSize;
};

/*
 * size
 *
 * Provides the amount of elements in the list.
 *
 * Returns: Amount of elements in the list
 */
template <class Type>
int StackADT<Type>::size() const
{
	return currentSize;
}

/*
 * isEmpty
 *
 * Determines whether the list is empty.
 *
 * Returns: True if the list is empty, false otherwise
 */
template <class Type>
bool StackADT<Type>::isEmpty() const
{
	return size() == 0;
}

/*
 * clear
 *
 * Removes all elements from the stack
 */
template <class Type>
void StackADT<Type>::clear()
{
	while (!isEmpty())
		pop();
}

#endif