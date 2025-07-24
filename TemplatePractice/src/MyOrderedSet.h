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
	void append(T value) {
		if (isMember(value))
			return;
		if (m_list.m_head == nullptr) {
			m_list.m_head = new MyLinkedListNode(value);
			m_list.m_size++;
		} else {
			MyLinkedListNode *p = m_list.m_head;
			while (p->m_next) {
				p = p->m_next;
			}
			p->m_next = new MyLinkedListNode(value);
			m_list.m_size++;
		}
	}

	void copyInOrder(const MyOrderedSet &other) {

		clear();
		if (other.m_list.m_head) {
			m_list.m_head = new MyLinkedListNode(other.m_list.m_head->m_data);
			m_list.m_size++;
			MyLinkedListNode *src = other.m_list.m_head;
			MyLinkedListNode *dst = m_list.m_head;
			while (src->m_next) {
				dst->m_next = new MyLinkedListNode(src->m_next->m_data);
				m_list.m_size++;
				src = src->m_next;
				dst = dst->m_next;
			}
		}

	}

	void insertInOrder(T value) {

		//	if the value is already in the set, return
		if (isMember(value)) {
			return;
		}
		//	if the set is empty, put the value in a node at the head
		if (m_list.m_head == nullptr) {
			m_list.m_head = new MyLinkedListNode(value);
			m_list.m_size++;
			return;
		}
		//	if the value is not in the set, check head
		//	  b/c the later checks will only look at ->next
		if (m_list.m_head->m_data > value) {
			m_list.m_head = new MyLinkedListNode(value, m_list.m_head);
			m_list.m_size++;
			return;
		}
		//	the value is after head, search through ->m_next
		MyLinkedListNode *p = m_list.m_head;
		while (p->m_next) {
			if (p->m_next->m_data > value) {
				p->m_next = new MyLinkedListNode(value, p->m_next);
				m_list.m_size++;
				return;
			}
			p = p->m_next;
		}
		//	the value was larger than the largest member of the list
		p->m_next = new MyLinkedListNode(value);
		m_list.m_size++;
	}

	void remove(const T &value) {
		if (m_list.m_head == nullptr)
			return;

		MyLinkedListNode *node_to_delete;
		// check to see if value is at head b/c successive checks
		//	will be run on head->m_next ... ->m_next->m_next
		if (m_list.m_head->m_data == value) {
			node_to_delete = m_list.m_head;
			m_list.m_head = m_list.m_head->m_next;
			delete node_to_delete;
			m_list.m_size--;
			return;
		}
		// check the next all successive ->m_next to see if
		//	value is in list
		MyLinkedListNode *p = m_list.m_head;
		while (p->m_next) {
			if (p->m_next->m_data == value) {
				node_to_delete = p->m_next;
				p->m_next = p->m_next->m_next;
				delete node_to_delete;
				m_list.m_size--;
				break;
			}
			p = p->m_next;
		}
	}

protected:

public:
	MyLinkedList m_list;

	/*	******************************************************	*/
	/*					basic operations						*/
	/*	******************************************************	*/

	void clear(void) {
		m_list.clear();
	}

	bool isMember(const T &value) const {
		return m_list.isMember(value);
	}

	int size() const {
		return m_list.m_size;
	}

	/*	******************************************************	*/
	/*					formatted output						*/
	/*	******************************************************	*/

	std::string toString() const {
		std::stringstream result;
		result << "set contains a list at " << &m_list << ": m_head is " << m_list.m_head << std::endl;
		result << m_list;
		return result.str();
	}

	std::string valuesString(int width = default_values_width) const {
		return m_list.valuesString(width);
	}

	int getValues(T **result) const {
		return m_list.getValues(result);
	}

	friend std::ostream& operator<<(std::ostream& out, const MyOrderedSet &set) {
		out << set.toString();
		return out;
	}

	/*	******************************************************	*/
	/*				inserting and deleting a value				*/
	/*	******************************************************	*/

	MyOrderedSet  operator+(const T &value) {
		MyOrderedSet result(*this);
		result.insertInOrder(value);
		return result;
	}

	MyOrderedSet  operator-(const T &value) {
		MyOrderedSet result(*this);
		result.remove(value);
		return result;
	}

	MyOrderedSet& operator+= (const T &value) {
		insertInOrder(value);
		return *this;
	}

	MyOrderedSet& operator-= (const T &value) {
		remove(value);
		return *this;
	}

	/*	******************************************************	*/
	/*				union of two sets Inclusive-OR				*/
	/*	******************************************************	*/

	MyOrderedSet  operator+(const MyOrderedSet &other) const {
		MyOrderedSet result(*this);
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				result.insertInOrder(p->m_data);
				p = p->m_next;
			}
		}
		return result;
	}

	MyOrderedSet& operator+=(const MyOrderedSet &other) {
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				insertInOrder(p->m_data);
				p = p->m_next;
			}
		}
		return *this;
	}

	MyOrderedSet  operator|(const MyOrderedSet &other) const {
		MyOrderedSet result(*this);
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				result.insertInOrder(p->m_data);
				p = p->m_next;
			}
		}
		return result;
	}

	MyOrderedSet& operator|=(const MyOrderedSet &other) {
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				insertInOrder(p->m_data);
				p = p->m_next;
			}
		}
		return *this;
	}

	/*	******************************************************	*/
	/*					set difference							*/
	/*	******************************************************	*/

	MyOrderedSet  operator-(const MyOrderedSet &other) const {
		MyOrderedSet result(*this);
		if (this == &other) {
			result.clear();
		} else {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				result.remove(p->m_data);
				p = p->m_next;
			}
		}
		return result;
	}

	MyOrderedSet& operator-=(const MyOrderedSet &other) {
		if (this == &other) {
			clear();
		} else {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				remove(p->m_data);
				p = p->m_next;
			}
		}
		return *this;
	}

	/*	******************************************************	*/
	/*					set intersection AND					*/
	/*	******************************************************	*/

	MyOrderedSet  operator& (const MyOrderedSet &other) const {

		MyOrderedSet result;
		if (this != &other) {
			if (other.m_list.m_head) {
			}
			MyLinkedListNode *p = m_list.m_head;
			while (p) {
				if (other.isMember(p->m_data)) {
					result.append(p->m_data);
				}
				p = p->m_next;
			}
		} else {
			result = *this;
		}
		return result;
	}

	MyOrderedSet& operator &= (const MyOrderedSet &other) {
		if (this != &other) {
			MyLinkedListNode *p = m_list.m_head;
			while (p) {
				if (!other.isMember(p->m_data)) {
					remove(p->m_data);
				}
				p = p->m_next;
			}
		}
		return *this;
	}

	/*	******************************************************	*/
	/*						set exclusive OR					*/
	/*	******************************************************	*/

	MyOrderedSet  operator ^(const MyOrderedSet &other) const {
		MyOrderedSet result(*this);

		MyLinkedListNode *p = other.m_list.m_head;
		while (p) {
			if (isMember(p->m_data)) {
				// an element in the other is also in this
				result.remove(p->m_data);
			} else {
				// an element in the other is NOT in this
				result.insertInOrder(p->m_data);
			}
			p = p->m_next;
		}
		return result;
	}

	MyOrderedSet& operator ^=(const MyOrderedSet &other) {
		if (this == &other) {
			clear();
		} else {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				if (isMember(p->m_data)) {
					// if an element from other is in this list remove it
					remove(p->m_data);
				} else {
					// other wise, in other but not this, insert it
					insertInOrder(p->m_data);
				}
				p = p->m_next;
			}
		}
		return *this;
	}

	/*	******************************************************	*/
	/*					relational operators					*/
	/*	******************************************************	*/

	bool operator ==(const MyOrderedSet &other) const {
		return m_list == other.m_list;
	}
	bool operator !=(const MyOrderedSet &other) const {
		return m_list != other.m_list;
	}

	/*	******************************************************	*/
	/*					constructors / destructor				*/
	/*	******************************************************	*/

	MyOrderedSet() {}
	~MyOrderedSet() {
		m_list.clear();
	}

	MyOrderedSet(const MyOrderedSet &other) {
		m_list.m_head = nullptr;
		m_list.m_size = 0;
		copyInOrder(other);
	}

	MyOrderedSet& operator=(const MyOrderedSet &other) {
		if (this != &other) {
			clear();
			copyInOrder(other);
		}
		return *this;
	}

	MyOrderedSet(MyOrderedSet &&other) noexcept {
		if (this != &other) {
			clear();
			m_list.m_head = other.m_list.m_head;
			m_list.m_size = other.m_list.m_size;
			other.m_list.m_head = nullptr;
			other.m_list.m_size = 0;
		}
	}

	MyOrderedSet& operator=(MyOrderedSet &&other) noexcept {
		if (this != &other) {
			clear();
			m_list.m_head = other.m_list.m_head;
			m_list.m_size = other.m_list.m_size;
			other.m_list.m_head = nullptr;
			other.m_list.m_size = 0;
		}
		return *this;
	}
};


#pragma pop_macro("default_values_width")

#endif /* MYORDEREDSET_H_ */
