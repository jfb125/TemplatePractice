/*
 * JoesWrappedUnsignedLinkedList.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Joe Baker
 *  Linked List of class WrappedUnsignedLinkedListNode
 *    which consists of a WrappedUnsigned (unsigned int)
 *    passed arguments to this LinkedList get copied
 */

#ifndef WRAPPEDUNSIGNEDLINKEDLIST_H_
#define WRAPPEDUNSIGNEDLINKEDLIST_H_

#include <iostream>
#include <iomanip>

#include "WrappedUnsigned.h"
#include "WrappedUnsignedLinkedListNode.h"

class WrappedUnsignedLinkedList{

public:
	WrappedUnsignedLinkedListNode *head;
	int size;

	bool isMember(const WrappedUnsigned &) const;

	// insert & delete operators
	WrappedUnsignedLinkedList operator+(const WrappedUnsigned &);
	WrappedUnsignedLinkedList operator-(const WrappedUnsigned &);
	WrappedUnsignedLinkedList& operator +=(const WrappedUnsigned &);
	WrappedUnsignedLinkedList& operator -=(const WrappedUnsigned &);

	// merge & cull operators
	WrappedUnsignedLinkedList operator+(const WrappedUnsignedLinkedList&);
	WrappedUnsignedLinkedList operator-(const WrappedUnsignedLinkedList&);
	WrappedUnsignedLinkedList& operator +=(const WrappedUnsignedLinkedList&);
	WrappedUnsignedLinkedList& operator -=(const WrappedUnsignedLinkedList&);

	void clear(void);

	bool operator==(const WrappedUnsignedLinkedList&) const;
	bool operator!=(const WrappedUnsignedLinkedList&) const;

	WrappedUnsignedLinkedList();
	WrappedUnsignedLinkedList(const WrappedUnsignedLinkedList& other);
	WrappedUnsignedLinkedList& operator = (const WrappedUnsignedLinkedList &);
	//	move constructor / assignment operator take ownership of the object's linked list
	WrappedUnsignedLinkedList(WrappedUnsignedLinkedList&& other) noexcept;
	WrappedUnsignedLinkedList& operator= (WrappedUnsignedLinkedList&&other) noexcept ;
	~WrappedUnsignedLinkedList();

	std::string toString() const;
	int getValues(unsigned **dst) const;
	std::string valuesString(int uwidth = 2) const;
	friend std::ostream& operator<<(std::ostream& out, const WrappedUnsignedLinkedList& list);
};

int listSize(const WrappedUnsignedLinkedList&);

#endif /* WRAPPEDUNSIGNEDLINKEDLIST_H_ */
