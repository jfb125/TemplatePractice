/*
 * JoesWrappedUnsignedLinkedList.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Joe Baker
 *  Linked List of class WrappedUnsignedLinkedListNode
 *    which consists of a WrappedUnsigned (unsigned int)
 *    passed arguments to this LinkedList get copied
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <iostream>
#include <iomanip>

#include "LinkedListNode.h"

//template <Tename T>
using T = int;

class LinkedList{
protected:
	LinkedListNode *m_head;
	int m_size;
public:

	bool isMember(const T &data) const {

		LinkedListNode *p = m_head;
		while(p) {
			if (p->m_data == data)
				return true;
			p = p->m_next;
		}
		return false;
	}

	// insert & delete operators
	LinkedList operator+(const T &data) {

		if (m_head == nullptr) {
			m_head = new LinkedListNode(data);
			m_size = 1;
		} else {
			LinkedListNode *p = m_head;
			// find the last node, which has no next node
			while (p->m_next) {
				p = p->m_next;
			}
			p->m_next = new LinkedListNode(data);
			m_size++;
		}
		return *this;
	}

	LinkedList operator-(const T &data) {
		if (isMember(data)) {
			LinkedListNode *node_to_be_deleted;
			if (m_head->m_data == data) {
				node_to_be_deleted = m_head;
				m_head = m_head->m_next;
			} else {
				LinkedListNode *p = m_head;
				while (p->m_next->m_data != data) {
					p = p->m_next;
				}
				node_to_be_deleted = p->m_next;
				p->m_next = p->m_next->m_next;
			}
			delete node_to_be_deleted;
			m_size--;
		}
		return *this;
	}

	LinkedList& operator +=(const T &data) {

		if (m_head == nullptr) {
			m_head = new LinkedListNode(data);
		}
		LinkedListNode *p = m_head;
		while (p->m_next) {
			p = p->m_next;
		}
		p->m_next = new LinkedListNode(data);
		m_size++;
		return this;
	}

	LinkedList& operator -=(const T &data) {
		if (isMember(data)) {
			LinkedListNode *node_to_be_deleted;
			if (m_head->m_data == data) {
				node_to_be_deleted = m_head;
				m_head = m_head->m_next;
			} else {
				LinkedListNode *p = m_head;
				while (p->m_next->m_data != data) {
					p = p->m_next;
				}
				node_to_be_deleted = p->m_next;
				p->m_next = p->m_next->m_next;
			}
			delete node_to_be_deleted;
			m_size--;
		}
		return this;
	}

	// merge & cull operators
	LinkedList operator+(const LinkedList&) {

	}
	LinkedList operator-(const LinkedList&);
	LinkedList& operator +=(const LinkedList&);
	LinkedList& operator -=(const LinkedList&);

	void clear(void);

	bool operator==(const LinkedList&) const;
	bool operator!=(const LinkedList&) const;

	LinkedList();
	LinkedList(const LinkedList& other);
	LinkedList& operator = (const LinkedList &);
	//	move constructor / assignment operator take ownership of the object's linked list
	LinkedList(LinkedList&& other) noexcept;
	LinkedList& operator= (LinkedList&&other) noexcept ;
	~LinkedList();

	std::string toString() const;
	int getValues(T **dst) const;
	std::string valuesString(int uwidth = 2) const;
	std::ostream& operator<<(std::ostream& out, const LinkedList& list);
	int listSize() const;
};

#endif /* LINKEDLIST_H_ */
