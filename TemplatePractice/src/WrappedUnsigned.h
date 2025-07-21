/*
 * ComparableObject.h
 *
 *  Created on: Mar 22, 2025
 *      Author: joe
 */

#ifndef WRAPPEDUNSIGNED_H_
#define WRAPPEDUNSIGNED_H_

#include <iostream>
#include <iomanip>

class WrappedUnsigned {
public:
	unsigned get_x() const;

	bool operator==(const WrappedUnsigned& other) const;
	bool operator!=(const WrappedUnsigned& other) const;
	bool operator>(const WrappedUnsigned& other) const;
	bool operator<(const WrappedUnsigned& other) const;
	bool operator<=(const WrappedUnsigned& other) const;
	bool operator>=(const WrappedUnsigned& other) const;

	WrappedUnsigned operator+(const WrappedUnsigned&);
	WrappedUnsigned operator-(const WrappedUnsigned&);
	WrappedUnsigned operator*(const WrappedUnsigned&);
	WrappedUnsigned operator/(const WrappedUnsigned&);	// performs integer division
	WrappedUnsigned operator%(const WrappedUnsigned&);	// modulo a.k.a. remainder

	WrappedUnsigned& operator++();							// prefix
	WrappedUnsigned operator++(const int);	// postfix
	WrappedUnsigned& operator--();							// prefix
	WrappedUnsigned operator--(const int);	// postfix

	WrappedUnsigned& operator+=(const WrappedUnsigned &other);
	WrappedUnsigned& operator-=(const WrappedUnsigned &other);
	WrappedUnsigned& operator*=(const WrappedUnsigned &other);
	WrappedUnsigned& operator/=(const WrappedUnsigned &other);
	WrappedUnsigned& operator%=(const WrappedUnsigned &other);

	WrappedUnsigned& operator=(const WrappedUnsigned& other);

	//	returns an integer < 0 if this < other
	//	returns an integer > 0 if this > other
	//	returns 0 otherwise
	int compare(const WrappedUnsigned& other) const;

	std::string toString() const;
	friend std::ostream& operator<<(std::ostream&out, const WrappedUnsigned& obj);

	WrappedUnsigned();
	WrappedUnsigned(unsigned x);
	WrappedUnsigned(const WrappedUnsigned& other);
	~WrappedUnsigned();

private:
	unsigned x;
};

#endif /* WRAPPEDUNSIGNED_H_	*/
