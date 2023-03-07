/******************************************
* StaticSet.h
*
* Static Set class (using templates).
*
* Author: Juan O. Lopez
******************************************/

#ifndef STATICSET_H
#define STATICSET_H

#include <iostream>

template <class Type>
class StaticSet
{
	template <class T>
	friend std::ostream& operator<<(std::ostream&, const StaticSet<T>&);

	public:
		StaticSet(int = DEFAULTAMT); // constructor with default parameter
		StaticSet(const StaticSet<Type> &); // Copy constructor
		const StaticSet<Type>& operator=(const StaticSet<Type> &); // Overload =
		~StaticSet(); // destructor
		
		void add(const Type &);
		bool remove(const Type &); // remove a single copy
		int removeAll(const Type &); // remove ALL copies
		void clear();
		bool isElement(const Type &) const;
		int size() const; // amount of elements
		bool isEmpty() const;
		Type* asArray() const;
		StaticSet<Type> setunion(const StaticSet<Type> &) const;
		StaticSet<Type> intersection(const StaticSet<Type> &) const;
		StaticSet<Type> difference(const StaticSet<Type> &) const;
		bool isSubset(const StaticSet<Type> &) const;
	private:
		int currentSize, capacity;
		Type *elements;
		static const int DEFAULTAMT = 10;
};

/* Implementation included in the same file due to the use of templates. */

/* Constructor */
template <class Type>
StaticSet<Type>::StaticSet(int initialCapacity)
{
	if (initialCapacity < 1) // Make sure we get a valid number
		initialCapacity = DEFAULTAMT;
	capacity = initialCapacity;
	elements = new Type[capacity];
	currentSize = 0; // Set is initially empty
}

/* Copy constructor */
template <class Type>
StaticSet<Type>::StaticSet(const StaticSet<Type>& otherSet)
{
	/* No need to clear out elements, because when a copy
	 * constructor is invoked, the object is empty. */
	currentSize = otherSet.currentSize;
	capacity = otherSet.capacity;
	elements = new Type[otherSet.capacity];
	for (int i = 0; i < currentSize; i++)
		elements[i] = otherSet.elements[i];
}

/* Overloading assignment operator (=) */
template <class Type>
const StaticSet<Type>& StaticSet<Type>::operator=(const StaticSet<Type>& otherSet)
{
	if (this != &otherSet) // Avoid self-assignment (e.g. bag1 = bag1;)
	{
		/* Unlike the copy constructor, we are going to copy to an existing set which
		may have elements, so we first delete the existing elements. */
		delete [] elements;
		/* Note that the following code is identical to the copy constructor.
		   In future classes, we will avoid duplicating similar code. */
		currentSize = otherSet.currentSize;
		capacity = otherSet.capacity;
		elements = new Type[otherSet.capacity];
		for (int i = 0; i < currentSize; i++)
			elements[i] = otherSet.elements[i];
	}

	return *this;
}

/* Destructor */
template <class Type>
StaticSet<Type>::~StaticSet()
{
	delete [] elements; // Avoid memory leak
}

/*
 * add
 *
 * Add an element to the set if there's room and if it's not already present.
 * 
 * Parameters: e - Element to be added to the set
 */
template <class Type>
void StaticSet<Type>::add(const Type& e)
{
	/* Check if there's room and that the element isn't already there. */
	if (currentSize < capacity && !isElement(e))
		elements[currentSize++] = e; // currentSize incremented
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
bool StaticSet<Type>::remove(const Type& e)
{
	/* First, need to find the element */
	for (int i = 0; i < currentSize; i++)
		/* NOTE: Elements must be comparable.  If Type is a user-defined class,
		 * then that class must overload the comparison operator (==). */
		if (elements[i] == e) // Found it!
		{
			/* Move last element to position i to avoid gaps */
			elements[i] = elements[currentSize - 1];
			elements[currentSize - 1] = 0; // "delete" duplicate (might need casting)
			currentSize--;
			return true;
		}
	/* If we make it here, the element wasn't found */
	return false;
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
int StaticSet<Type>::removeAll(const Type& e)
{
	int counter = 0;
	while (remove(e))
		counter++;
	return counter;
}

/*
 * clear
 *
 * Remove all elements from the set
 */
template <class Type>
void StaticSet<Type>::clear()
{
	/* First clear out the data */
	for (int i = 0; i < currentSize; i++)
		elements[i] = 0;
	/* Now reset currentSize */
	currentSize = 0;
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
bool StaticSet<Type>::isElement(const Type& e) const
{
	for (int i = 0; i < currentSize; i++)
		if (elements[i] == e)
			return true;
	/* If we make it here, we didn't find it */
	return false;
}

/*
 * size
 *
 * Determines the amount of elements in the set.
 *
 * Returns: Amount of elements in the set
 */
template <class Type>
int StaticSet<Type>::size() const
{
	return currentSize;
}

/*
 * asArray
 *
 * Returns: Contents of the set as an array.
 * (Useful due to the lack of an iterator).
 */
template <class Type>
Type* StaticSet<Type>::asArray() const
{
	Type *elementsCopy = new Type[currentSize];

	for (int i = 0; i < currentSize; i++)
		elementsCopy[i] = elements[i];
	return elementsCopy;
	/* THINK: Why can't we simply return the elements array? */
}

/*
 * isEmpty
 *
 * Determines whether the set is empty.
 *
 * Returns: True if the set is empty, false otherwise
 */
template <class Type>
bool StaticSet<Type>::isEmpty() const
{
	return (currentSize == 0);
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
std::ostream& operator<<(std::ostream& os, const StaticSet<Type>& set)
{
	for (int i = 0; i < set.currentSize; i++)
		os << set.elements[i] << " ";
	os << "\n";

	return os;
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
StaticSet<Type> StaticSet<Type>::setunion(const StaticSet<Type>& otherSet) const
{
	StaticSet<Type> result(size() + otherSet.size()); // In case all elements are different
	/* First we copy this set's elements */
	for (int i = 0; i < size(); i++)
		result.add(elements[i]);
	/* Now we copy the other set's elements 
	 * NOTE: elements is private in the Static Set class, but we're still
	 *       within the Static Set class, so we can access otherSet.elements
	 *       The asArray method is for use by other classes/programs, not here. */
	for (int i = 0; i < otherSet.size(); i++)
		result.add(otherSet.elements[i]); // add() will avoid duplicates
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
StaticSet<Type> StaticSet<Type>::intersection(const StaticSet<Type>& otherSet) const
{
	StaticSet<Type> result(size()); // could use the size of the smallest of the two sets
	/* Copy the elements of the first set that are also in the second set. */
	for (int i = 0; i < size(); i++)
		if (otherSet.isElement(elements[i])) // Test if element is in otherSet
			result.add(elements[i]);
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
StaticSet<Type> StaticSet<Type>::difference(const StaticSet<Type>& otherSet) const
{
	StaticSet<Type> result(size()); // New set can't be bigger than current set
	/* Copy the elements of the first set that are not in the second set. */
	for (int i = 0; i < size(); i++)
		if (!otherSet.isElement(elements[i])) // Test if element is NOT in otherSet
			result.add(elements[i]);
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
bool StaticSet<Type>::isSubset(const StaticSet<Type>& otherSet) const
{
	/* Verify whether all elements of the first set are also in second set */
	for (int i = 0; i < size(); i++)
		if (!otherSet.isElement(elements[i]))
			return false;
	/* If we make it here, all elements were found in otherSet */
	return true;
}

#endif
