/*
 * ComparableObject.h
 *
 *  Created on: Mar 22, 2025
 *      Author: joe
 */

#ifndef COMPARABLEOBJECT_H_
#define COMPARABLEOBJECT_H_

#include <iostream>
#include <iomanip>

class ComparableObject {
public:

	virtual bool operator==(const ComparableObject& other);
	virtual bool operator!=(const ComparableObject& other);
	virtual bool operator>(const ComparableObject& other);
	virtual bool operator<(const ComparableObject& other);
	virtual bool operator<=(const ComparableObject& other);
	virtual bool operator>=(const ComparableObject& other);
	virtual int compare(const ComparableObject& other);

	/*	returns 'this' as an unsigned long	*/
	virtual std::string toString();

	ComparableObject();
	virtual ~ComparableObject();
};

#endif /* COMPARABLEOBJECT_H_ */
