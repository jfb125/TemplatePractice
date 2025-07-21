#ifndef SETTESTBENCH_H
#define SETTESTBENCH_H

#include <iostream>
#include <iomanip>
#include <cstring>

#include "SetTestBench.h"

enum Message_Level {
	None = 0,
	Values = 1,
	Addresses = 2,
	Results = 4,
	Test_Summary = 8,
	Values_And_Addresses = Values | Addresses,
	Verbose = Values_And_Addresses | Results | Test_Summary
};

bool isMsgLvlNone(Message_Level level);
bool isMsgLvlValues(Message_Level level);
bool isMsgLvlAddresses(Message_Level level);
bool isMsgLvlResults(Message_Level level);
bool isMsgLvlSummary(Message_Level level);
bool isMsgLvlValuesAndAddresses(Message_Level level);
bool isMsgLvlVerbose(Message_Level level);

#define uint_size(_array_)	(sizeof((_arrary_))/sizeof(unsigned))

bool testWrappedUnsignedLinkedList();
void testWrappedUnsigned();
void testWrappedUnsignedLinkedListNode();

std::string uiarray_toString(const unsigned *values, int count);

bool verifyResults(	const char *before,
					unsigned *expected, int expected_count,
					unsigned *results, int result_count,
					char *after, Message_Level lvl);

void echoTestName(const char *);
void echoTestPhase(const char *);
void echoWrUInt(const char* before, WrappedUnsigned&, const char* after);
void echoLinkedList(const char* before, WrappedUnsignedLinkedList&, const char* after);

#endif
