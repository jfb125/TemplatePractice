/*
 * WrappedUnsignedSet.h
 *
 *  Created on: Mar 25, 2025
 *      Author: joe
 *
 *   MySet uses a linked list as a container
 *     but implements adding & removing elements
 *     in a manner such that the list is always in order
 */

#ifndef MYORDEREDSET_H_
#define MYORDEREDSET_H_

#include <iostream>
#include <iomanip>

#include "MyLinkedList.h"

#pragma push_macro("default_values_width")
#ifdef default_values_width
#undef default_values_width
#endif
#define default_values_width 2

using T = int;

class MyOrderedSet {
private:
	void insertAfter(MyLinkedListNode **current, T value) {
		MyLinkedListNode *new_node;
		//	if *current == nullptr, it is pointing to head
		if (*current == nullptr)
			new_node = new MyLinkedListNode(value, nullptr);
		else
			new_node = new MyLinkedListNode(value, (*current)->m_next);
		(*current)->m_next = new_node;
		list.m_size++;
	}

	void deleteNext(MyLinkedListNode **current, T Value) {
		MyLinkedListNode *node_to_delete = (*current)->m_next;
		//	if (*current)->m_next->m_next exists, move m_next out of list
		if ((*current)->m_next) {
			(*current)->m_next = (*current)->m_next->m_next;
		}
		delete node_to_delete;
		list.m_size--;
	}

protected:
	MyLinkedList list;

public:
	int size() const {
		return list.m_size;
	}

	std::string toString() const {
		std::stringstream result;
		result << "set contains a list at " << &list << ": " << std::endl;
		result << list;
		return result.str();
	}

	std::string valuesString(int width = default_values_width) const {
		return list.valuesString(width);
	}

	int getValues(T **result) const {
		return list.getValues(result);
	}

	std::ostream& operator<<(std::ostream& out, const MyOrderedSet &set) {
		out << set.toString();
		return out;
	}

	bool isMember(const T &value) const {
		return list.isMember(value);
	}

	// inserting and deleting

	MyOrderedSet  operator+(const T &value) {
		MyOrderedSet result(*this);
		MyLinkedListNode *p = result.list.m_head;

		return result;
	}
	MyOrderedSet  operator-(const T &value) {
		list.remove(value);
		return *this;
	}
	MyOrderedSet& operator+= (const T &value) {
		if (!list.isMember(value))
			list.insert(value);
		return *this;
	}
	MyOrderedSet& operator-= (const T &value) {
		list.remove(value);
		return *this;
	}

	// set union - combine all elements of both sets
	MyOrderedSet  operator +(const MyOrderedSet &other) const {
		if (this != &other)
			list += other.list;
		return *this;
	}
	MyOrderedSet& operator +=(const MyOrderedSet &other) {
		if (this != &other)
			list += other.list;
		return this;
	}
	MyOrderedSet  operator |(const MyOrderedSet &other) const {
		if (this != &other) {
			list += other.list;
		}
		return *this;
	}
	MyOrderedSet& operator |=(const MyOrderedSet &other) {
		if (this != &other) {
			list += other.list;
		}
		return this;
	}

	// set difference - return elements unique to lhs
	MyOrderedSet  operator -(const MyOrderedSet&) const;
	MyOrderedSet& operator -=(const MyOrderedSet&);

	// return elements that are not in intersection
	MyOrderedSet  operator ^(const MyOrderedSet&) const;
	MyOrderedSet& operator ^=(const MyOrderedSet&);

	// set intersection return elements that are in both sets
	MyOrderedSet  operator & (const MyOrderedSet&) const;
	MyOrderedSet& operator &= (const MyOrderedSet&);

	MyOrderedSet& clear(void);
	MyOrderedSet& reset(void);

	// relational operators
	bool operator ==(const MyOrderedSet&) const;
	bool operator !=(const MyOrderedSet&) const;

	MyOrderedSet();
	~MyOrderedSet();
	MyOrderedSet(const MyOrderedSet &other);
	MyOrderedSet& operator=(const MyOrderedSet &other);
	MyOrderedSet(MyOrderedSet &&other) noexcept;
	MyOrderedSet& operator=(MyOrderedSet &&other) noexcept;
};

#pragma pop_macro("default_values_width")

#endif /* MYORDEREDSET_H_ */
