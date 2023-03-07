/*****************************************************
 * LinkedStack.h
 *
 * Stack ADT implementation using a linked structure
 *
 * Authors: Juan O. Lopez & Gustavo A. Rassi
 ****************************************************/
#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include "StackADT.h"

template <class Type>
struct nodeType
{
	Type data;
	nodeType *next;
};
	
template <class Type>
class LinkedStack : public StackADT<Type>
{
	template <class T>
	friend std::ostream& operator<<(std::ostream&, const LinkedStack<T>&);

	public:
		LinkedStack(); // constructor
		LinkedStack(const LinkedStack<Type>&); // copy constructor
		const LinkedStack<Type>& operator=(const LinkedStack<Type>&); // overload of = operator
		~LinkedStack(); // destructor

		void push(const Type&); // add to top of stack
		Type pop(); // remove and return top of stack
		Type top() const; // return top of stack
	private:
		void copyStack(const LinkedStack<Type>&); // Used by copy constructor and operator=

	 	nodeType<Type> *stackTop; // Could also be named "head" if you prefer
};

/* Constructor */
template <class Type>
LinkedStack<Type>::LinkedStack()
{
	this->currentSize = 0;
	stackTop = nullptr;
}

/* Copy constructor */
template <class Type>
LinkedStack<Type>::LinkedStack(const LinkedStack<Type>& otherStack)
{
	stackTop = nullptr;
	this->currentSize = 0;
	copyStack(otherStack);
}

/*
 * operator=
 *
 * Overload the assignment operator (=) to copy one stack into another.
 *
 * Parameters: otherStack - The other stack to be copied into this stack
 * Returns: A reference to this stack
 */
template <class Type>
const LinkedStack<Type>& LinkedStack<Type>::operator=(const LinkedStack<Type>& otherStack)
{
	if (this != &otherStack) // avoid self-assignment
	{
		/* The only difference between the copy constructor and operator=
		 * is that copy constructor creates a completely new stack, whereas
		 * operator= assigns to an existing stack that may have elements.
		 * Hence, we delete those elements first and then invoke the
		 * shared code. */
		this->clear(); // discard any existing data
		copyStack(otherStack);
	}

	return *this;
}

/*
 * copyStack
 *
 * Make this stack a copy of another stack, implementing a deep copy,
 * as opposed to the default of just copying pointers (shallow copy).
 * This code is shared by copy constructor and operator=.
 */
template <class Type>
void LinkedStack<Type>::copyStack(const LinkedStack<Type>& otherStack)
{
	/* Any existing data would have been removed in operator= */

	/* Copy otherStack if not empty */
	if (!otherStack.isEmpty())
	{
		nodeType<Type> *curOther, *curThis, *newNode;

		stackTop = new nodeType<Type>;
		stackTop->data = otherStack.stackTop->data;
		stackTop->next = nullptr;
		curOther = otherStack.stackTop->next;
		/* curThis is like a temporary tail node, used to next
		 * existing nodes with new node */
		curThis = stackTop;
		while (curOther != nullptr)
		{
			newNode = new nodeType<Type>;
			newNode->data = curOther->data;
			newNode->next = nullptr; // in case this is the last node
			curThis->next = newNode;
			curThis = newNode;
			curOther = curOther->next;
		}
	}
	this->currentSize = otherStack.currentSize;
}

/* Destructor */
template <class Type>
LinkedStack<Type>::~LinkedStack() {
	this->clear();
}

template <class Type>
void LinkedStack<Type>::push(const Type& obj)
{
	nodeType<Type> *newNode = new nodeType<Type>;
	newNode->data = obj;
	newNode->next = stackTop;
	stackTop = newNode;
	this->currentSize++;
}

template <class Type>
Type LinkedStack<Type>::pop()
{
	if (this->isEmpty())
		throw "EXCEPTION: Stack is empty!";
	Type etr = stackTop->data;
	nodeType<Type> *nodeToDelete = stackTop;
	stackTop = stackTop->next;
	delete nodeToDelete;
	this->currentSize--;

	return etr;
}

template <class Type>
Type LinkedStack<Type>::top() const
{
	if (this->isEmpty())
		throw "EXCEPTION: Stack is empty!";
	return stackTop->data;
}

/*
 * operator<<
 *
 * Overload the << operator to output the stack using an output stream.
 *
 * Parameters: os  - Output stream to use
 *             stack - stack to output
 * Returns: Output stream that was used
 */
template <class Type>
std::ostream& operator<<(std::ostream& os, const LinkedStack<Type>& stack)
{
	/* How can we print the top of the stack at the end, like in ArrayStack?
	 * Simple, use another stack!!! */
	LinkedStack<Type> reversedStack;
	nodeType<Type> *curNode; // used to traverse the nodes
	for (curNode = stack.stackTop; curNode != nullptr; curNode = curNode->next)
		reversedStack.push(curNode->data);
	/* The top of the original stack is at the bottom of reversedStack,
	 * so it will be the last element printed (like in ArrayStack) */
	while (!reversedStack.isEmpty())
		os << reversedStack.pop() << " ";
	os << "\n";
	return os;
}

#endif
