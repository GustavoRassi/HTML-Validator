/******************************************
* DynamicSet<Type>.h
*
* Dynamic Set class.
*
* Authors: Juan O. Lopez & Gustavo A. Rassi
******************************************/
#include <iostream>
#include "StaticSet.h"

#ifndef DYNAMICSET_H
#define DYNAMICSET_H

template <class Type>
class DynamicSet
{
	template <class T>
	friend std::ostream& operator<<(std::ostream&, const DynamicSet<T>&);

	public:
		DynamicSet(int = 10); // constructor with default parameter
		DynamicSet(const DynamicSet<Type>&); // copy constructor
		const DynamicSet<Type>& operator=(const DynamicSet<Type>&); // Overload =
		//~DynamicSet();  Destructor of Static Set will be automatically invoked

		void add(const Type &);
		bool remove(const Type &); // remove a single copy
		int removeAll(const Type &); // remove ALL copies
		void clear();
		bool isElement(const Type &) const;
		int size() const; // amount of elements
		bool isEmpty() const;
		DynamicSet<Type> setunion(const DynamicSet<Type> &) const;
		DynamicSet<Type> intersection(const DynamicSet<Type> &) const;
		DynamicSet<Type> difference(const DynamicSet<Type> &) const;
		bool isSubset(const DynamicSet<Type> &) const;
		Type* asArray() const;
	private:
		int capacity;
		/* Instead of directly manipulating an elements array,
		 * we use a DynamicSet object to handle everything for us
		 * (except the add method in case the set is full). */
		//Type *elements;
		StaticSet<Type> theSet; // object composition
};

/* Implementation included in the same file due to the use of templates. */

/* Constructor */
template <class Type>
DynamicSet<Type>::DynamicSet(int initialCapacity)
{
	if (initialCapacity < 1) // Make sure we get a valid number
		initialCapacity = 10; // Same as default parameter
	capacity = initialCapacity;
	theSet = StaticSet<Type>(capacity); // Uses operator=
}

/* Copy constructor */
template <class Type>
DynamicSet<Type>::DynamicSet(const DynamicSet<Type>& otherSet)
{
	capacity = otherSet.capacity;
	theSet = otherSet.theSet; // Use Static Set's overloaded =
}

/* Overloading assignment operator (=) */
template <class Type>
const DynamicSet<Type>& DynamicSet<Type>::operator=(const DynamicSet<Type>& otherSet)
{
	if (this != &otherSet) // Avoid self-assignment
	{
		capacity = otherSet.capacity;
		theSet = otherSet.theSet; // Use Static Set's overloaded =
	}

	return *this;
}

/*
 * add
 *
 * Add an element to the set if there's room left.
 * 
 * Parameters: e - Element to be added to the set
 */
template <class Type>
void DynamicSet<Type>::add(const Type& e)
{
	/* First, check if there's room */
	if (theSet.size() == capacity)
	{
		/* Set is full, need to "grow"
		 * New set should hold twice as many elements */
		Type *setAsArray = theSet.asArray();
		theSet = StaticSet<Type>(2*capacity); // uses DynamicSet's overloaded =
		for (int i = 0; i < capacity; i++)
			theSet.add(setAsArray[i]);
		capacity *= 2;
	}
	theSet.add(e);
}

/*
 * remove
 *
 * Remove from the set one copy of an element if it's there.
 *
 * Parameters: e - Element to be removed
 * Returns: true if element was removed, false otherwise
 */
template <class Type>
bool DynamicSet<Type>::remove(const Type& e)
{
	return theSet.remove(e);
}

/*
 * removeAll
 *
 * Remove from the set all copies of an element.
 *
 * Parameters: e - Element to be removed
 * Returns: Amount of copies removed (could be 0)
 */
template <class Type>
int DynamicSet<Type>::removeAll(const Type& e)
{
	return theSet.removeAll(e);
}

/*
 * clear
 *
 * Remove all elements from the set
 */
template <class Type>
void DynamicSet<Type>::clear()
{
	theSet.clear();
}

/*
 * isElement
 *
 * Determines if an element is present in the set.
 *
 * Parameters: e - Element to look for
 * Returns: True if the element is in the set, false otherwise
 */
template <class Type>
bool DynamicSet<Type>::isElement(const Type& e) const
{
	return theSet.isElement(e);
}

/*
 * size
 *
 * Determines the amount of elements in the set.
 *
 * Returns: Amount of elements in the set
 */
template <class Type>
int DynamicSet<Type>::size() const
{
	return theSet.size();
}

/*
 * isEmpty
 *
 * Determines whether the set is empty.
 *
 * Returns: True if the set is empty, false otherwise
 */
template <class Type>
bool DynamicSet<Type>::isEmpty() const
{
	return theSet.isEmpty();
}

/*
 * asArray
 *
 * Returns: Contents of the set as an array.
 * (Useful due to the lack of an iterator).
 */
template <class Type>
Type* DynamicSet<Type>::asArray() const
{
	return theSet.asArray();
}


/*
 * operator<<
 *
 * Overload the << operator to output the set using an output stream.
 *
 * Parameters: os  - Output stream to use
 *             set - Set to output
 * Returns: Output stream that was used
 */
template <class Type>
std::ostream& operator<<(std::ostream& os, const DynamicSet<Type>& set)
{
	return (os << set.theSet); // this invokes operator<< for Static Set
}

/*
 * setunion
 *
 * Perform the union operation with the specified set.
 *
 * Parameters: otherSet - Set to perform union with
 * Returns: New set resulting from the union
 */
template <class Type>
DynamicSet<Type> DynamicSet<Type>::setunion(const DynamicSet<Type>& otherSet) const
{
	DynamicSet<Type> result(size() + otherSet.size()); // In case all elements are different
	result.theSet = theSet.setunion(otherSet.theSet);
	return result;
}

/*
 * intersection
 *
 * Perform the intersection operation with the specified set.
 *
 * Parameters: otherSet - Set to perform intersection with
 * Returns: New set resulting from the intersection
 */
template <class Type>
DynamicSet<Type> DynamicSet<Type>::intersection(const DynamicSet<Type>& otherSet) const
{
	DynamicSet<Type> result(theSet.size());
	result.theSet = theSet.intersection(otherSet.theSet);
	return result;
}

/*
 * difference
 *
 * Perform the difference operation with the specified set.
 *
 * Parameters: otherSet - Set to perform difference with
 * Returns: New set resulting from the difference
 */
template <class Type>
DynamicSet<Type> DynamicSet<Type>::difference(const DynamicSet<Type>& otherSet) const
{
	DynamicSet<Type> result(theSet.size());
	result.theSet = theSet.difference(otherSet.theSet);
	return result;
}

/*
 * isSubset
 *
 * Determine if set is a subset of another set.
 *
 * Parameters: otherSet - The set which might contain this set
 * Returns: True if this set is a subset of otherSet, and false otherwise
 */
template <class Type>
bool DynamicSet<Type>::isSubset(const DynamicSet<Type>& otherSet) const
{
	return theSet.isSubset(otherSet.theSet);
}

#endif
