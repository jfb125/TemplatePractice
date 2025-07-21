/*
 * WrappedUnsignedLinkedListNode.h
 *
 *  Created on: Mar 24, 2025
 *      Author: joe
 */

#ifndef LINKEDLISTNODE_H_
#define LINKEDLISTNODE_H_

/* ********************************************************************	*/
/*																		*/
/*	class to implement a Linked List Node that contains an object		*/
/*		of type "WrappedUnsigned".  The node makes a copy of the 		*/
/*		passed object													*/
/*																		*/
/* ********************************************************************	*/

#include <iostream>
#include <iomanip>

#pragma push_macro("_data_width")
#ifdef _data_width
#undef _data_width
#endif
#define _data_width 4

using typ = int;
//template <typename T>
class LinkedListNode {
public:
	typ m_data;
	LinkedListNode *m_next;	// cannot use 'next' as an identifier

public:
	~LinkedListNode() {
		m_next = nullptr;
	}

	LinkedListNode() {
		m_data = 0;
		m_next = nullptr;
	}

	LinkedListNode(const typ &_data, LinkedListNode* _next) :
						m_data(_data), m_next(_next) {}

	LinkedListNode(const typ &_data) : m_data(_data), m_next(nullptr) {}

	LinkedListNode(const LinkedListNode& other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
		}
	}

	LinkedListNode& operator=(const LinkedListNode& other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
		}
		return *this;
	}

	LinkedListNode(LinkedListNode &&other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
			other.m_next = nullptr;
		}
	}

	LinkedListNode& operator=(LinkedListNode &&other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
			other.m_next = nullptr;
		}
		return *this;
	}

	std::string toString() {
		std::stringstream ret_str;
		ret_str << this
				<< " data: " << std::setw(_data_width) << m_data
				<< " next: " << m_next;
		return ret_str.str();
	}
#if 0
	friend std::ostream& operator<<(std::ostream &out, const LinkedListNode<T>& node) {
		out << node.toString();
		return out;
	}
#endif
};

#pragma pop_macro("_data_width")

#endif /* LINKEDLISTNODE_H_ */
