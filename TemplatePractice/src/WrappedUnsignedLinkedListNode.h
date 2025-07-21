/*
 * WrappedUnsignedLinkedListNode.h
 *
 *  Created on: Mar 24, 2025
 *      Author: joe
 */

#ifndef WRAPPEDUNSIGNEDLINKEDLISTNODE_H_
#define WRAPPEDUNSIGNEDLINKEDLISTNODE_H_

/* ********************************************************************	*/
/*																		*/
/*	class to implement a Linked List Node that contains an object		*/
/*		of type "WrappedUnsigned".  The node makes a copy of the 		*/
/*		passed object													*/
/*																		*/
/* ********************************************************************	*/

#include <iostream>
#include <iomanip>

#include "WrappedUnsigned.h"

class WrappedUnsignedLinkedListNode {

public:
	WrappedUnsigned data;
	WrappedUnsignedLinkedListNode *next;	// cannot use 'next' as an identifier

	std::string toString();
	friend std::ostream& operator<<(std::ostream&out, WrappedUnsignedLinkedListNode& node);

 	WrappedUnsignedLinkedListNode& operator=(const WrappedUnsignedLinkedListNode& other);

	WrappedUnsignedLinkedListNode();
	WrappedUnsignedLinkedListNode(const WrappedUnsignedLinkedListNode&);
 	WrappedUnsignedLinkedListNode(const WrappedUnsigned&);	// ->next is set to nullptr
 	WrappedUnsignedLinkedListNode(const WrappedUnsigned&, WrappedUnsignedLinkedListNode*);
	~WrappedUnsignedLinkedListNode();	// does not delete the object '*next'
};

#endif /* WRAPPEDUNSIGNEDLINKEDLISTNODE_H_ */
