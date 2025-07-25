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

	virtual bool operator==(const ComparableObject& other) = 0;
	virtual bool operator!=(const ComparableObject& other) = 0;
	virtual bool operator>(const ComparableObject& other) = 0;
	virtual bool operator<(const ComparableObject& other) = 0;
	virtual bool operator<=(const ComparableObject& other) = 0;
	virtual bool operator>=(const ComparableObject& other) = 0;
	virtual int compare(const ComparableObject& other) = 0;

	/*	returns 'this' as an unsigned long	*/
	std::string toString() {
		std::stringstream result;
		result << "Comparable object @" << this << std::endl;
		return result.str();
	}

	ComparableObject();
	virtual ~ComparableObject();
};

#endif /* COMPARABLEOBJECT_H_ */
