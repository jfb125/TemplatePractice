#ifndef CLASSWRUINTSET_TESTS_H
#define CLASSWRUINTSET_TESTS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include "PlayingCard.h"
#include "MyOrderedSet.h"
#include "MyOrderedSetTestBench.h"

// the test itself
bool testMyOrderedSet();

template <typename T>
std::string arrayToString(T *values, int count) {

	std::stringstream retval;

	retval << "{ ";

	if (count > 0) {
		for (int i = count ; i != 1; i--) {
			retval << std::setw(2) << *values++ << ", ";
		}
		retval << std::setw(2) << *values;
	} else {
		retval << "empty";
	}
	retval << " }";
	return retval.str();
}

template <typename T>
bool verifyResults(	std::string before,
					T *expected, int expected_count,
					T *results, int result_count,
					std::string after, Message_Level lvl) {

	bool passed = true;
	std::cout << before;

	if (isMsgLvlVerbose(lvl)) {
		std::cout << " expecting:        " << arrayToString(expected, expected_count) << std::endl;
		std::cout << " against received: " << arrayToString(results, result_count) << std::endl;
	}

	if (expected_count != result_count) {
		if (isMsgLvlSummary(lvl) || isMsgLvlResults(lvl)) {
			std::cout << "num expecteds " << expected_count << " != num results " << result_count << std::endl ;
			passed = false;
			goto VERIFY_SET_RESULT_END_OF_FUNCTION;
		}
	}

	for (int i = 0; i != expected_count; i++)  {
		if (true || isMsgLvlResults(lvl)) {
			if (expected[i] != results[i]) {
				std::cout << " " << std::setw(2) << i << " ERROR expected " << expected[i] << " != " << results[i] << std::endl;
				passed = false;
			} else {
				std::cout << " " << std::setw(2) << i << "       expected " << expected[i] << " == " << results[i] << std::endl;
			}
		}
	}

VERIFY_SET_RESULT_END_OF_FUNCTION:
	if (isMsgLvlSummary(lvl)) {
		std::cout << " verifyResults returning " << (passed ? "true" : "false") << std::endl;
	}
	std::cout << after;

	return passed;
}

#endif
