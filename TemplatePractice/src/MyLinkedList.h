/*
 * JoesWrappedUnsignedLinkedList.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Joe Baker
 *  Linked List of class WrappedUnsignedLinkedListNode
 *    which consists of a WrappedUnsigned (unsigned int)
 *    passed arguments to this LinkedList get copied
 */

#ifndef MYLINKEDLIST_H_
#define MYLINKEDLIST_H_

#include <iostream>
#include <iomanip>
#include <memory>

#include "MyLinkedListNode.h"

#pragma push_macro("default_values_width")
#ifdef default_values_width
#undef default_values_width
#endif
#define default_values_width 2

using T = int;

//template <typename T>
class MyLinkedList{
private:
	/*
	 * 	the caller should ensure that the 'value'
	 * 	is not already in the list by calling 'isMember(value)'
	 */
	void insert(const T &value) {
		m_head = new MyLinkedListNode(value, m_head);
		m_size++;
	}

	void copy(const MyLinkedList &other) {
		clear();
		if (other.m_head) {
			m_head = new MyLinkedListNode(other.m_head->m_data);
			m_size++;
			MyLinkedListNode *dst = m_head;
			MyLinkedListNode *src = other.m_head;
			while (src->m_next) {
				dst->m_next = new MyLinkedListNode(src->m_next->m_data);
				m_size++;
				src = src->m_next;
				dst = dst->m_next;
			}
		}
	}

	void remove(const T &value) {
		if (m_head == nullptr)
			return;
		MyLinkedListNode *node_to_delete;
		if (m_head->m_data == value) {
			node_to_delete = m_head;
			m_head = m_head->m_next;
			delete node_to_delete;
			m_size--;
		} else {
			MyLinkedListNode *p = m_head;
			while (p->m_next) {
				if (p->m_next->m_data == value) {
					node_to_delete = p->m_next;
					p->m_next = p->m_next->m_next;
					delete node_to_delete;
					m_size--;
					break;
				}
				p = p->m_next;
			}
		}
	}

//protected:
public:
	MyLinkedListNode *m_head;
	int m_size;

public:

	void clear(void) {
		MyLinkedListNode *node_to_delete;
		while (m_head) {
			node_to_delete = m_head;
			m_head = m_head->m_next;
			delete node_to_delete;
		}
		m_size = 0;
		m_head = nullptr;
	}

	bool isMember(const T &value) const {

		MyLinkedListNode *p = m_head;
		while(p) {
			if (p->m_data == value)
				return true;
			p = p->m_next;
		}
		return false;
	}

	/*	******************************************************************	*/
	/*							insertion and deletion						*/
	/*	******************************************************************	*/

	MyLinkedList operator+(const T &value) {
		MyLinkedList result(*this);
		if (!result.isMember(value))
			result.insert(value);
		return result;
	}

	MyLinkedList operator-(const T &value) {
		MyLinkedList result(*this);
		result.remove(value);
		return result;
	}

	MyLinkedList& operator+=(const T &value) {
		if (!isMember(value))
			insert(value);
		return *this;
	}

	MyLinkedList& operator-=(const T &value) {
		remove(value);
		return *this;
	}

	/*	******************************************************************	*/
	/*						merging and culling lists						*/
	/*	******************************************************************	*/

	MyLinkedList operator+(const MyLinkedList &other) {
		MyLinkedList result(*this);
		if (this != &other) {
			MyLinkedListNode *p = other.m_head;
			while (p) {
				if (!result.isMember(p->m_data)) {
					result.insert(p->m_data);
				}
				p = p->m_next;
			}
		}
		return result;
	}

	MyLinkedList operator-(const MyLinkedList &other) {
		MyLinkedList result(*this);
		if (this == &other) {
			result.clear();
		} else {
			MyLinkedListNode *p = other.m_head;
			while (p) {
				result.remove(p->m_data);
				p = p->m_next;
			}
		}
		return result;
	}

	MyLinkedList& operator+=(const MyLinkedList &other) {
		if (this != &other) {
			MyLinkedListNode *p = other.m_head;
			while (p) {
				if (!isMember(p->m_data)) {
					insert(p->m_data);
				}
				p = p->m_next;
			}
		}
		return *this;
	}

	MyLinkedList& operator-=(const MyLinkedList &other) {
		if (this == &other) {
			clear();
		} else {
			MyLinkedListNode *p = other.m_head;
			while (p) {
				remove(p->m_data);
				p = p->m_next;
			}
		}
		return *this;
	}

	/*	******************************************************************	*/
	/*							relational operators						*/
	/*	******************************************************************	*/

	bool operator==(const MyLinkedList &other) const {
		if (this == &other)
			return true;

		if (m_size != other.m_size)
			return false;

		MyLinkedListNode *p_node;

		//	check every member of other to ensure
		//	  that every member of other is in this
		p_node = other.m_head;
		while(p_node) {
			if (!isMember(p_node->m_data)) {
				return false;
			}
			p_node = p_node->m_next;
		}

		//	check every member of this to ensure
		//	  that every member of this is in other
		p_node = m_head;
		while (p_node) {
			if (!other.isMember(p_node->m_data)) {
				return false;
			}
			p_node = p_node->m_next;
		}
		return true;
	}

	bool operator!=(const MyLinkedList &other) const {
		if (this == &other)
			return false;

		if (m_size != other.m_size)
			return true;

		MyLinkedListNode *p_node;

		// go through every member of 'other' to see
		//	if there is an element in 'other' that is not in 'this'
		p_node = other.m_head;
		while (p_node) {
			if (!isMember(p_node->m_data)) {
				return true;
			}
			p_node = p_node->m_next;
		}

		// go through every member of 'this' to see
		//	if there is an element in 'this' that is not in 'other'
		p_node = m_head;
		while (p_node) {
			if (!other.isMember(p_node->m_data)) {
				return true;
			}
			p_node = p_node->m_next;
		}
		return false;
	}

	/*	******************************************************************	*/
	/*			constructors / assignment operators / destructor			*/
	/*	******************************************************************	*/

	MyLinkedList() {
		m_head = nullptr;
		m_size = 0;
	}

	MyLinkedList(const MyLinkedList &other) {
		m_head = nullptr;
		m_size = 0;
		copy(other);
	}

	MyLinkedList& operator= (const MyLinkedList &other) {
		if (this != &other) {
			clear();
			copy(other);
		}
		return *this;
	}

	//	move constructor / assignment operator take ownership of the object's linked list
	MyLinkedList(MyLinkedList&& other) noexcept {
		if (this != &other) {
			clear();
			m_head = other.m_head;
			m_size = other.m_size;
			other.m_head = nullptr;
			other.m_size = 0;
		}
	}

	MyLinkedList& operator= (MyLinkedList&&other) noexcept {
		if (this != &other) {
			clear();
			m_head = other.m_head;
			m_size = other.m_size;
			other.m_head = nullptr;
			other.m_size = 0;
		}
		return *this;
	}

	~MyLinkedList() {
		clear();
	}

	std::string toString() const {
		std::stringstream result;
		result << "list contains " << m_size << " elements:" << std::endl;
		if (m_head) {
			MyLinkedListNode *p = m_head;
			while (p->m_next) {
				result << "  " << *p << std::endl;
				p = p->m_next;
			}
			result << "  " << *p;
		} else {
			result << "  " << "empty list";
		}
		return result.str();
	}

	int getValues(T **p_result) const {
		*p_result = new T[m_size];
		MyLinkedListNode *src = m_head;
		T *dst = *p_result;
		while (src) {
			*dst = src->m_data;
			dst++;
			src = src->m_next;
		}
		return m_size;
	}

	std::string valuesString(int width = default_values_width) const {
		std::stringstream result;
		result << "{ ";
		if (m_head) {
			MyLinkedListNode *p = m_head;
			while (p->m_next) {
				result << std::setw(width) << p->m_data << ", ";
				p = p->m_next;
			}
			result << p->m_data;
		} else {
			result << "<empty list>";
		}
		result << " }";
		return result.str();
	}

	int listSize() const {
		return m_size;
	}

	friend std::ostream& operator<<(std::ostream& out, const MyLinkedList& object) {
		out << object.toString();
		return out;
	}
};

#pragma pop_macro("default_values_width")

#endif /* MYLINKEDLIST_H_ */
