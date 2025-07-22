/*
 * WrappedUnsignedLinkedListNode.h
 *
 *  Created on: Mar 24, 2025
 *      Author: joe
 */

#ifndef MYLINKEDLISTNODE_H_
#define MYLINKEDLISTNODE_H_

/* ********************************************************************	*/
/*																		*/
/*	class to implement a Linked List Node that contains an object		*/
/*		of Te "WrappedUnsigned".  The node makes a copy of the 		*/
/*		passed object													*/
/*																		*/
/* ********************************************************************	*/

#include <iostream>
#include <iomanip>

#pragma push_macro("default_data_width")
#ifdef default_data_width
#undef default_data_width
#endif
#define default_data_width 4

using T = int;
//template <Tename T>
class MyLinkedListNode {
public:
	T m_data;
	MyLinkedListNode *m_next;	// cannot use 'next' as an identifier

public:
	~MyLinkedListNode() {
		m_next = nullptr;
	}

	MyLinkedListNode() {
		m_data = 0;
		m_next = nullptr;
	}

	MyLinkedListNode(const T &_data, MyLinkedListNode* _next) :
						m_data(_data), m_next(_next) {}

	MyLinkedListNode(const T &_data) : m_data(_data), m_next(nullptr) {}

	MyLinkedListNode(const MyLinkedListNode& other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
		}
	}

	MyLinkedListNode& operator=(const MyLinkedListNode& other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
		}
		return *this;
	}

	MyLinkedListNode(MyLinkedListNode &&other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
			other.m_next = nullptr;
		}
	}

	MyLinkedListNode& operator=(MyLinkedListNode &&other) {
		if (this != &other) {
			m_data = other.m_data;
			m_next = other.m_next;
			other.m_next = nullptr;
		}
		return *this;
	}

	std::string toString(int width = default_data_width) const {
		std::stringstream ret_str;
		ret_str << "0x" << this
				<< " data: " << std::setw(width) << m_data
				<< " next: 0x" << m_next;
		return ret_str.str();
	}

	friend std::ostream& operator<<(std::ostream &out, const MyLinkedListNode &object) {
		out << object.toString();
		return out;
	}
};

#pragma pop_macro("default_data_width")

#endif /* MYLINKEDLISTNODE_H_ */
