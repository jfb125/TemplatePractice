/*
 * WrappedUnsignedSet.cpp
 *
 *  Created on: Mar 25, 2025
 *      Author: joe
 */
#if 0
#include "WrappedUnsignedSet.h"

using namespace std;

using WrUInt = WrappedUnsigned;
using WrUIntNode = LinkedListNode;
using WrUIntList = WrappedUnsignedLinkedList;
using WrUIntSet = WrappedUnsignedSet;


WrUIntSet& insIntoSet(WrUIntSet *dst, const WrUInt& value_to_ins);
WrUIntSet& delFromSet(WrUIntSet *dst, const WrUInt& value_to_del);
WrUIntSet& intersectSet(WrUIntSet *dst, const WrUIntSet& other);
WrUIntSet& unionSet(WrUIntSet *dst, const WrUIntSet& other);
// *minuend gets replaced with the set (*minuend - subtrahend) and *minuend is returned
WrUIntSet& differenceSet(WrUIntSet *minuend, const WrUIntSet& subtrahend);
WrUIntSet& uniqueSet(WrUIntSet *dst, const WrUIntSet& other);


/* ****************************************************************	*/
/* ****************************************************************	*/
/*						accessors									*/
/* ****************************************************************	*/
/* ****************************************************************	*/

int WrappedUnsignedSet::size() const {
	return list.size;
}

std::string WrappedUnsignedSet::toString() const {
	return "TODO - WrUIntSet.toString()";
}

std::string WrappedUnsignedSet::valuesString() const {

	std::stringstream retval;

	retval << "{ ";
	if (list.size == 0) {
		retval << "empty }";
	} else {
		WrUIntNode *node = list.head;
		// do not print the last element from the while loop
		while (node->next) {
			retval << node->data << ", ";
			node = node->next;
		}
		retval << node->data << " }";
	}
	return retval.str();
}

int WrappedUnsignedSet::getValues(unsigned **dst) const {

	return list.getValues(dst);
}

bool WrappedUnsignedSet::isMember(const WrappedUnsigned &value) const {

	return list.isMember(value);
}

// relational operators
bool WrappedUnsignedSet::operator ==(const WrappedUnsignedSet& other) const {

	return list == other.list;
}
bool WrappedUnsignedSet::operator !=(const WrappedUnsignedSet& other) const {
	return list != other.list;
}

std::ostream& operator<<(std::ostream& out, const WrappedUnsignedSet &set) {

	out << "set->list @" << &set.list << ": " << std::endl << set.list;
	return out;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*						arithmetic operators						*/
/* ****************************************************************	*/
/* ****************************************************************	*/

/*				unsigneds				*/

WrappedUnsignedSet  WrappedUnsignedSet::operator+(const WrappedUnsigned& value_to_add) const {

	WrUIntSet retval(*this);
	retval.list += value_to_add;
	return retval;
}

WrappedUnsignedSet  WrappedUnsignedSet::operator-(const WrappedUnsigned& value_to_sub) const {

	WrUIntSet retval(*this);
	retval.list -= value_to_sub;
	return retval;
}

/*				sets					*/

//	union: element-wise OR

WrappedUnsignedSet  WrappedUnsignedSet::operator +(const WrappedUnsignedSet& other) const {

	WrUIntSet retval(*this);
	unionSet(&retval, other);
	return retval;
}

//	union: element-wise OR

WrappedUnsignedSet  WrappedUnsignedSet::operator |(const WrappedUnsignedSet& other) const {

	WrUIntSet retval(*this);
	unionSet(&retval, other);
	return retval;
}

//	set difference *this - other
WrappedUnsignedSet  WrappedUnsignedSet::operator -(const WrappedUnsignedSet& other) const {

	WrUIntSet retval(*this);
	differenceSet(&retval, other);
	return retval;
}

// return elements that are not in intersection: element-wise XOR
WrappedUnsignedSet  WrappedUnsignedSet::operator ^(const WrappedUnsignedSet& other) const {

	WrUIntSet retval(*this);
	uniqueSet(&retval, other);
	return retval;
}

// set intersection return elements that are in both sets: element-wise AND
WrappedUnsignedSet  WrappedUnsignedSet::operator &(const WrappedUnsignedSet& other) const {

	WrUIntSet retval(*this);
	intersectSet(&retval, other);
	return retval;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*						assignment operators						*/
/*					including clear() and reset()					*/
/* ****************************************************************	*/
/* ****************************************************************	*/

//	unsigneds

WrappedUnsignedSet& WrappedUnsignedSet::operator += (const WrappedUnsigned &value_to_add) {
	this->list += value_to_add;
	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::operator -= (const WrappedUnsigned &value_to_remove) {
	this->list -= value_to_remove;
	return *this;
}

//	sets

// set union: element-wise OR

WrappedUnsignedSet& WrappedUnsignedSet::operator +=(const WrappedUnsignedSet& other) {

	unionSet(this, other);
	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::operator |=(const WrappedUnsignedSet& other) {
	unionSet(this, other);
	return *this;
}

// set difference *this - other

WrappedUnsignedSet& WrappedUnsignedSet::operator -=(const WrappedUnsignedSet& other) {
	differenceSet(this, other);
	return *this;
}

// returns members that are not in both sets - member-wise  XOR

WrappedUnsignedSet& WrappedUnsignedSet::operator ^=(const WrappedUnsignedSet& other) {
	uniqueSet(this, other);
	return *this;
}

// returns set intersection members that are in both sets : member-wise XNOR

WrappedUnsignedSet& WrappedUnsignedSet::operator &= (const WrappedUnsignedSet& other) {
	intersectSet(this, other);
	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::operator=(const WrappedUnsignedSet &other) {

	if (this != &other) {
		list = other.list;
	}
	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::operator=(WrappedUnsignedSet &&other) noexcept {

	if (this != &other) {
		list = std::move(other.list);
		list.size = listSize(this->list);
		other.list.size = 0;
		other.list.head = nullptr;;
	}

	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::clear(void) {

	list.clear();
	return *this;
}

WrappedUnsignedSet& WrappedUnsignedSet::reset(void) {
	list.clear();
	return *this;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*					constructors & destructor						*/
/* ****************************************************************	*/
/* ****************************************************************	*/

WrappedUnsignedSet::WrappedUnsignedSet() {
}

WrappedUnsignedSet::WrappedUnsignedSet(const WrappedUnsignedSet &other) {

	if (this == &other)
		return;

	list = other.list;
}

WrappedUnsignedSet::WrappedUnsignedSet(WrappedUnsignedSet &&other) noexcept {

	if (this == &other)
		return;

	list = std::move(other.list);
}

WrappedUnsignedSet::~WrappedUnsignedSet() {
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*					private helper functions						*/
/* ****************************************************************	*/
/* ****************************************************************	*/

WrUIntSet& insIntoSet(WrUIntSet *dst, const WrUInt& value_to_ins) {

	dst->list += value_to_ins;
	return *dst;
}

WrUIntSet& delFromSet(WrUIntSet *dst, const WrUInt& value_to_del) {

	dst->list -= value_to_del;
	return *dst;
}

//	creates a set in *dst that contains only members that are in both sets
//  aka element-wise NOT(XOR) aka element-wise 'coincidence'
WrUIntSet& intersectSet(WrUIntSet *dst, const WrUIntSet& other) {

	// { anything } XNOR { anything } = { anything }
	// avoid copying a list onto itself
	if (dst == &other)
		return *dst;

	// { empty } XNOR { anything } = { empty }
	// { anything } XNOR { empty } = { empty }
	if (dst->size() == 0 || other.size() == 0) {
		dst->clear();
		return *dst;
	}

	WrUIntSet intersection_set;	// create empty destination set

	// comparing all elements of the smaller set size m to the larger set size n
	//	results in m passes through n elements = O(n) m x n
	// comparing all elements of the larger set size n to the smaller set m times
	//	results in n passes through m elements = O(n) n x m
	WrUIntNode *others_node_to_test = other.list.head;

	// this pass copies elements that
	while (others_node_to_test) {
		if (dst->isMember(others_node_to_test->data)) {
			insIntoSet(&intersection_set, others_node_to_test->data);
		}
		others_node_to_test = others_node_to_test->next;
	}

	*dst = intersection_set;
	return *dst;
}

// element wise OR
WrUIntSet& unionSet(WrUIntSet *dst, const WrUIntSet& other) {

	// avoid copying a list onto itself
	if (dst == &other)
		return *dst;

	dst->list += other.list;
	return *dst;
}

// minuend - subtrahend
WrUIntSet& differenceSet(WrUIntSet *minuend, const WrUIntSet& subtrahend) {

	if (minuend == &subtrahend) {
		minuend->clear();
		return *minuend;
	}

	// { empty } - { anything } = { empty }
	// { anything } - { empty } = { anything }
	if (minuend->size() == 0 || subtrahend.size() == 0)  {
		return *minuend;
	}

	WrUIntNode *subtrahend_node = subtrahend.list.head;

	while (subtrahend_node) {
		// if an element is in both nodes, removed it from answer
		if (minuend->isMember(subtrahend_node->data)) {
			minuend->list -= subtrahend_node->data;
		}
		subtrahend_node = subtrahend_node->next;
	}

	return *minuend;
}

// return values that are in one set, but not the other, element-wise XOR
WrUIntSet& uniqueSet(WrUIntSet *dst, const WrUIntSet& other) {

	// there are no unique values if a set ^ itself
	if (dst == &other) {
		dst->list.clear();
		return *dst;
	}

	// { empty } XNOR { anything } = { anything }
	if (dst->size() == 0) {
		*dst = other;
		return *dst;
	}

	// { anything } XNOR { empty } = { anything }
	if (other.size() == 0) {
		return *dst;
	}

	WrUIntNode *others_node_to_test = other.list.head;

	while (others_node_to_test) {
		if (dst->isMember(others_node_to_test->data)) {
			// if a value is common to both sets, remove it
			dst->list -= (others_node_to_test->data);
		} else {
			// if a value is in other set but not in dst, insert value
			dst->list += (others_node_to_test->data);
		}
		others_node_to_test = others_node_to_test->next;
	}

	return *dst;
}
#endif
