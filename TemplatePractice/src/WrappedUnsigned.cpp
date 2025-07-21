/*
 * WrappedUnsigned.cpp
 *
 *  Created on: Mar 22, 2025
 *      Author: Joe Baker
 */

#include "WrappedUnsigned.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

using WrUInt = WrappedUnsigned;


#pragma push_macro("echo_this")
#pragma push_macro("echo_other")
#pragma push_macro("log_fname")

#define echo_this(wrapped) 		"this   " << wrapped << "->&" << &wrapped->x << ": " << wrapped->x
#define echo_other(wrapped)		"other  " << &wrapped << "->&" << &wrapped.x << ": " << wrapped.x
#define echo_retval(wrapped) 	"retval " << &wrapped << "->&" << &wrapped.x << ": " << wrapped.x


#define WRUINT_VERBOSE true
#define log_fname() do {\
  if (WRUINT_VERBOSE) printf(" '%-40s'\n", __PRETTY_FUNCTION__);\
} while(false)

#if WRUINT_VERBOSE
#define log(output)	output
#else
#define log(output)
#endif

/* ********************************************************	*/
/*					local constants							*/
/* ********************************************************	*/


/* ********************************************************	*/
/*						accessors							*/
/* ********************************************************	*/

unsigned WrappedUnsigned::get_x() const {	return x; 	};

std::string WrappedUnsigned::toString() const {
	std::stringstream return_value;
	return_value << "at address " << this << " has member x at address " << &this->x << " containing " << x;
	return return_value.str();
}

std::ostream& operator<<(std::ostream&out, const WrappedUnsigned& obj) {
	out << obj.x;
	return out;
}


/* ********************************************************	*/
/*						mutators							*/
/* ********************************************************	*/

/*						assignment							*/

WrUInt& WrappedUnsigned::operator=(const WrUInt& other) {

	if (this != &other) {
//		log_fname();
//		log(cout << "before 'x = other.x' in operator=(const WrUInt&), " << echo_this(this) << endl;)
//		log(cout << "before 'x = other.x' in operator=(const WrUInt&), " << echo_other(other) << endl;)
		x = other.x;
//		log(cout << "after  'x = other.x' in operator=(const WrUInt&), " << echo_this(this) << endl;)
//		log(cout << "after  'x = other.x' in operator=(const WrUInt&), " << echo_other(other) << endl;)
	}

	return *this;
}

/*						unary							*/

// prefix
WrUInt& WrappedUnsigned::operator++() {
//	log_fname();
//	log(cout << "before ++x: " << echo_this(this) << endl;)
	++x;
//	log(cout << "after  ++x: " << echo_this(this) << endl;)
	return *this;
}

// postfix
WrUInt WrappedUnsigned::operator++(const int ignored __attribute__((unused))) {
//	log_fname();
	WrUInt return_value(*this);
//	log(cout << "before ++x: " << echo_this(this) << endl;)
//	log(cout << "before ++x: " << echo_retval(return_value) << endl;)
	x++;
//	log(cout << "after  ++x: " << echo_this(this) << endl;)
//	log(cout << "after  ++x: " << echo_retval(return_value) << endl;)
	return return_value;
}

// prefix
WrUInt& WrappedUnsigned::operator--() {
//	log_fname();
//	log(cout << "before --x: " << echo_this(this) << endl;)
	--x;
//	log(cout << "after  --x: " << echo_this(this) << endl;)
	return *this;
}

// postfix
WrUInt WrappedUnsigned::operator--(const int ignored __attribute__((unused))) {
//	log_fname();
	WrUInt return_value(*this);
//	log(cout << "before x--: " << echo_this(this) << endl;)
//	log(cout << "before x--: " << echo_retval(return_value) << endl;)
	x--;
//	log(cout << "after  x--: " << echo_this(this) << endl;)
//	log(cout << "after  x--: " << echo_retval(return_value) << endl;)
	return return_value;
}

/*					compound assignment					*/

WrUInt& WrappedUnsigned::operator+=(const WrUInt& other) {

//	log_fname();
//	cout << "before x += other.x: " << echo_this(this) << endl;
//	cout << "before x += other.x: " << echo_other(other) << endl;
	x += other.x;
//	cout << "after  x += other.x: " << echo_this(this) << endl;
//	cout << "after  x += other.x: " << echo_other(other) << endl;
	return *this;
}

WrUInt& WrappedUnsigned::operator-=(const WrUInt& other) {

//	log_fname();
//	cout << "before - += other.x: " << echo_this(this) << endl;
//	cout << "before - += other.x: " << echo_other(other) << endl;
	x -= other.x;
//	cout << "after  - += other.x: " << echo_this(this) << endl;
//	cout << "after  - += other.x: " << echo_other(other) << endl;
	return *this;
}

WrUInt& WrappedUnsigned::operator*=(const WrUInt& other) {
	x *= other.x;
	return *this;
}

WrUInt& WrappedUnsigned::operator/=(const WrUInt& other) {
	x /= other.x;
	return *this;
}

WrUInt& WrappedUnsigned::operator%=(const WrUInt& other) {
	x %= other.x;
	return *this;
}


/* ********************************************************	*/
/*				comparison operators						*/
/* ********************************************************	*/

bool WrappedUnsigned::operator==(const WrUInt& other) const {

	if (x == other.x) 	return true;
						return false;
}

bool WrappedUnsigned::operator<(const WrUInt& other) const {

	if (x < other.x)	return true;
						return false;
}

bool WrappedUnsigned::operator!=(const WrUInt& other) const {

	return !(x == other.x);
}

bool WrappedUnsigned::operator>(const WrUInt& other) const {

	// this is neither == to other nor this < other
	return !(x == other.x) && !(x < other.x);
}

bool WrappedUnsigned::operator<=(const WrUInt& other) const {

	//	this is either == other or this < other other
	return x == other.x || x < other.x;
}

bool WrappedUnsigned::operator>=(const WrUInt& other) const {

	// this neither > other nor this == other
	return !(x < other.x);
}

// need to protect against unsigned overflow
int WrappedUnsigned::compare(const WrUInt& other) const {
//	log_fname();
	if (x == other.x)	return 0;
	if (x < other.x)	return -1;
						return 1;
}


/* ********************************************************	*/
/*				arithmetic operators						*/
/* ********************************************************	*/

WrUInt WrappedUnsigned::operator+(const WrUInt& other) {

//	log_fname();
//	cout << "before constructor WrUInt retval(" << x << " + " << other.x << ") " << echo_this(this)  << endl;
//	cout << "before constructor WrUInt retval(" << x << " + " << other.x << ") " << echo_other(other)  << endl;
	WrUInt retval(x + other.x);
//	cout << "after constructor WrUInt retval(" << x << " + " << other.x << ") " << echo_this(this)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " + " << other.x << ") " << echo_other(other)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " + " << other.x << ") " << echo_retval(retval)  << endl;
	return retval;
}

WrUInt WrappedUnsigned::operator-(const WrUInt& other) {

//	log_fname();
//	cout << "before constructor WrUInt retval(" << x << " - " << other.x << ") " << echo_this(this)  << endl;
//	cout << "before constructor WrUInt retval(" << x << " - " << other.x << ") " << echo_other(other)  << endl;
	WrUInt retval(x - other.x);
//	cout << "after constructor WrUInt retval(" << x << " - " << other.x << ") " << echo_this(this)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " - " << other.x << ") " << echo_other(other)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " - " << other.x << ") " << echo_retval(retval)  << endl;
	return retval;
}

WrUInt	WrappedUnsigned::operator*(const WrUInt& other) {

//	log_fname();
//	cout << "before constructor WrUInt retval(" << x << " * " << other.x << ") " << echo_this(this)  << endl;
//	cout << "before constructor WrUInt retval(" << x << " * " << other.x << ") " << echo_other(other)  << endl;
	WrUInt retval(x * other.x);
//	cout << "after constructor WrUInt retval(" << x << " * " << other.x << ") " << echo_this(this)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " * " << other.x << ") " << echo_other(other)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " * " << other.x << ") " << echo_retval(retval)  << endl;
	return retval;
}

WrUInt WrappedUnsigned::operator/(const WrUInt& other) {

//	log_fname();
//	cout << "before constructor WrUInt retval(" << x << " / " << other.x << ") " << echo_this(this)  << endl;
//	cout << "before constructor WrUInt retval(" << x << " / " << other.x << ") " << echo_other(other)  << endl;
	WrUInt retval(x / other.x);
//	cout << "after constructor WrUInt retval(" << x << " / " << other.x << ") " << echo_this(this)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " / " << other.x << ") " << echo_other(other)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " / " << other.x << ") " << echo_retval(retval)  << endl;
	return retval;
}

WrUInt 	WrappedUnsigned::operator%(const WrUInt& other) {

//	log_fname();
//	cout << "before constructor WrUInt retval(" << x << " % " << other.x << ") " << echo_this(this)  << endl;
//	cout << "before constructor WrUInt retval(" << x << " % " << other.x << ") " << echo_other(other)  << endl;
	WrUInt retval(x % other.x);
//	cout << "after constructor WrUInt retval(" << x << " % " << other.x << ") " << echo_this(this)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " % " << other.x << ") " << echo_other(other)  << endl;
//	cout << "after constructor WrUInt retval(" << x << " % " << other.x << ") " << echo_retval(retval)  << endl;
	return retval;
}


/* ********************************************************	*/
/*			 		constructors / destructors				*/
/* ********************************************************	*/

WrappedUnsigned::WrappedUnsigned() {

//	cout << "default constructor" << endl;
	x = 0;
}

WrappedUnsigned::WrappedUnsigned(unsigned _x) : x(_x) {

//	log_fname();
//	cout << "constructor that takes unsigned" << endl;
}

WrappedUnsigned::WrappedUnsigned(const WrappedUnsigned& other) {

//	log_fname();
	if (this == &other) {
//		log(cout << "copy constructor passed itself" << endl;);
//		log(cout << "other " << echo_other(other));
//		log(cout << "this  " << echo_this(this));
		return;
	}

//	cout << "copy constructor" << endl;
//	log(cout << "before 'x = other.x' in copy_constructor(const WrUInt&): " << echo_this(this) << endl;)
//	log(cout << "before 'x = other.x' in copy_constructor(const WrUInt&): " << echo_other(other) << endl;)
	x = other.x;
//	log(cout << "after  'x = other.x' in copy_constructor(const WrUInt&): " << echo_this(this) << endl;)
//	log(cout << "after  'x = other.x' in copy_constructor(const WrUInt&): " << echo_other(other) << endl;)
}

WrappedUnsigned::~WrappedUnsigned() {
	// TODO Auto-generated destructor stub
}

#ifdef WRUINT_VERBOSE
#undef WRUINT_VERBOSE
#endif

#pragma pop_macro("log_fname")
#pragma pop_macro("echo_this")
#pragma pop_macro("echo_other")
