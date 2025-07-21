/*
 * WrappedUnsignedLinkedListNode.cpp
 *
 *  Created on: Mar 24, 2025
 *      Author: joe
 */

#include "WrappedUnsignedLinkedListNode.h"

using namespace std;

using WrUInt = WrappedUnsigned;
using WrUIntNode = WrappedUnsignedLinkedListNode;

static const int _data_width = 4;

std::string WrappedUnsignedLinkedListNode::toString() {
	std::stringstream ret_str;
	ret_str << this
			<< " data: " << std::setw(_data_width) << data
			<< " next: " << next;
	return ret_str.str();
}

std::ostream& operator<<(std::ostream&out, WrappedUnsignedLinkedListNode& node) {
	out << ".data: " << node.data << " .next: " << node.next;
	return out;
}


//	do not delete the next ptr b/c the next node may still be a member of the list
WrappedUnsignedLinkedListNode::~WrappedUnsignedLinkedListNode() {
	next = nullptr;
}

WrappedUnsignedLinkedListNode::WrappedUnsignedLinkedListNode() {
	data = 0;
	next = nullptr;
}

WrappedUnsignedLinkedListNode::WrappedUnsignedLinkedListNode(const WrUIntNode& other) {
	if (this == &other)
		return;

	data = other.data;
	next = other.next;
}

WrappedUnsignedLinkedListNode::WrappedUnsignedLinkedListNode(const WrUInt &_data, WrUIntNode* _next) :
	data(_data), next(_next) {
}

WrappedUnsignedLinkedListNode::WrappedUnsignedLinkedListNode(const WrUInt &_data) {
	data = _data;
	next = nullptr;
}

WrUIntNode& WrappedUnsignedLinkedListNode::operator=(
								const WrUIntNode& other) {
	if (this == &other)
		return *this;

	data = other.data;
	next = other.next;

	return *this;
}
