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
		if (m_list.m_head == nullptr) {
			m_list.m_head = new MyLinkedListNode(value);
			m_list.m_size++;
		} else {
			MyLinkedListNode *p = m_list.m_head;
			while (p->m_next) {
				p = p->m_next;
			}
			p->m_next = new MyLinkedListNode(value);
		}
	}

#if 0
	void insertAfter(MyLinkedListNode **current, T value) {
		MyLinkedListNode *new_node;
		//	if *current == nullptr, it is pointing to head
		if (*current == nullptr)
			new_node = new MyLinkedListNode(value, nullptr);
		else
			new_node = new MyLinkedListNode(value, (*current)->m_next);
		(*current)->m_next = new_node;
		m_list.m_size++;
	}

	void deleteNext(MyLinkedListNode **current, T Value) {
		MyLinkedListNode *node_to_delete = (*current)->m_next;
		//	if (*current)->m_next->m_next exists, move m_next out of list
		if ((*current)->m_next) {
			(*current)->m_next = (*current)->m_next->m_next;
		}
		delete node_to_delete;
		m_list.m_size--;
	}
#endif
	void insertInOrder(T value) {

		if (m_list.m_head == nullptr) {
			m_list.m_head = new MyLinkedListNode(value);
			m_list.m_size++;
			return;
		}
		if (m_list.m_head->m_data > value) {
			m_list.m_head = new MyLinkedListNode(value, m_list.m_head);
			m_list.m_size++;
			return;
		}
		MyLinkedListNode *p = m_list.m_head;
		while (p->m_next) {
			if (p->m_next->m_data > value) {
				p->m_next = new MyLinkedListNode(value, p->m_next->m_next);
				m_list.m_size++;
				return;
			}
		}
		p->m_next = new MyLinkedListNode(value);
		m_list.m_size++;
	}

	void remove(const T &value) {
		if (m_list.m_head == nullptr)
			return;

		MyLinkedListNode *node_to_delete;
		if (m_list.m_head->m_data == value) {
			node_to_delete = m_list.m_head;
			m_list.m_head = m_list.m_head->m_next;
			delete node_to_delete;
			m_list.m_size--;
		} else {
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
	}

protected:
	MyLinkedList m_list;

public:
	/*	******************************************************	*/
	/*					basic operations						*/
	/*	******************************************************	*/

	MyOrderedSet& clear(void) {
		m_list.clear();
		return this;
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
		result << "set contains a m_list at " << &m_list << ": " << std::endl;
		result << m_list;
		return result.str();
	}

	std::string valuesString(int width = default_values_width) const {
		return m_list.valuesString(width);
	}

	int getValues(T **result) const {
		return m_list.getValues(result);
	}

	std::ostream& operator<<(std::ostream& out, const MyOrderedSet &set) {
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
		return *this;
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
		return this;
	}

	MyOrderedSet  operator|(const MyOrderedSet &other) const {
		MyOrderedSet result(*this);
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				result.insertInOrder(p->m_data);
				p = p->m_data;
			}
		}
		return result;
	}

	MyOrderedSet& operator|=(const MyOrderedSet &other) {
		if (this != &other) {
			MyLinkedListNode *p = other.m_list.m_head;
			while (p) {
				insertInOrder(p->m_data);
				p = p->m_data;
			}
		}
		return this;
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
		return this;
	}

	/*	******************************************************	*/
	/*					set intersection AND					*/
	/*	******************************************************	*/

	MyOrderedSet  operator& (const MyOrderedSet &other) const {

		MyOrderedSet result;
		if (this != &other) {
			MyLinkedListNode *p = m_list.m_head;
			while (p) {
				if (other.isMember(p->m_data)) {
					result.append(p->m_data);
				}
				p = p->m_next;
			}
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
		return this;
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
		return this;
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
	~MyOrderedSet() {}

	MyOrderedSet(const MyOrderedSet &other) {
		m_list = other.m_list;
	}

	MyOrderedSet& operator=(const MyOrderedSet &other) {
		if (this != &other) {
			m_list = other.m_list;
		}
		return this;
	}

	MyOrderedSet(MyOrderedSet &&other) noexcept {
		if (this != &other) {
			m_list.m_head = other.m_list.m_head;
			m_list.m_size = other.m_list.m_size;
			other.m_list.m_head = nullptr;
			other.m_list.m_size = 0;
		}
	}

	MyOrderedSet& operator=(MyOrderedSet &&other) noexcept {
		if (this != &other) {
			m_list.m_head = other.m_list.m_head;
			m_list.m_size = other.m_list.m_size;
			other.m_list.m_head = nullptr;
			other.m_list.m_size = 0;
		}
		return this;
	}
};

#pragma pop_macro("default_values_width")

#endif /* MYORDEREDSET_H_ */
