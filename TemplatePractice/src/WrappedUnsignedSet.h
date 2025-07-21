/*
 * WrappedUnsignedSet.h
 *
 *  Created on: Mar 25, 2025
 *      Author: joe
 */

#ifndef WRAPPEDUNSIGNEDSET_H_
#define WRAPPEDUNSIGNEDSET_H_

#include <iostream>
#include <iomanip>

#include "WrappedUnsignedLinkedList.h"

class WrappedUnsignedSet {

public:
	WrappedUnsignedLinkedList list;

	int size() const;

	std::string toString() const;
	std::string valuesString() const;
	int getValues(unsigned**) const;
	friend std::ostream& operator<<(std::ostream& out, const WrappedUnsignedSet &set);

	bool isMember(const WrappedUnsigned &) const;

	// inserting and deleting wrapped unsigneds
	WrappedUnsignedSet  operator +(const WrappedUnsigned &) const;
	WrappedUnsignedSet  operator -(const WrappedUnsigned &) const;
	WrappedUnsignedSet& operator += (const WrappedUnsigned &);
	WrappedUnsignedSet& operator -= (const WrappedUnsigned &);

	// set union - combine all elements of both sets
	WrappedUnsignedSet  operator +(const WrappedUnsignedSet&) const;
	WrappedUnsignedSet& operator +=(const WrappedUnsignedSet&);
	WrappedUnsignedSet  operator |(const WrappedUnsignedSet&) const;
	WrappedUnsignedSet& operator |=(const WrappedUnsignedSet&);

	// set difference - return elements unique to lhs
	WrappedUnsignedSet  operator -(const WrappedUnsignedSet&) const;
	WrappedUnsignedSet& operator -=(const WrappedUnsignedSet&);

	// return elements that are not in intersection
	WrappedUnsignedSet  operator ^(const WrappedUnsignedSet&) const;
	WrappedUnsignedSet& operator ^=(const WrappedUnsignedSet&);

	// set intersection return elements that are in both sets
	WrappedUnsignedSet  operator & (const WrappedUnsignedSet&) const;
	WrappedUnsignedSet& operator &= (const WrappedUnsignedSet&);

	WrappedUnsignedSet& clear(void);
	WrappedUnsignedSet& reset(void);

	// relational operators
	bool operator ==(const WrappedUnsignedSet&) const;
	bool operator !=(const WrappedUnsignedSet&) const;

	WrappedUnsignedSet();
	~WrappedUnsignedSet();
	WrappedUnsignedSet(const WrappedUnsignedSet &other);
	WrappedUnsignedSet& operator=(const WrappedUnsignedSet &other);
	WrappedUnsignedSet(WrappedUnsignedSet &&other) noexcept;
	WrappedUnsignedSet& operator=(WrappedUnsignedSet &&other) noexcept;
};

#endif /* WRAPPEDUNSIGNEDSET_H_ */
