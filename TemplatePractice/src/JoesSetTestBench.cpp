#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

#include "ClassWrUIntSet_Tests.h"

using WrUInt = WrappedUnsigned;
using WrUIntNode = WrappedUnsignedLinkedListNode;
using WrUIntList = WrappedUnsignedLinkedList;
using WrUIntSet = WrappedUnsignedSet;
//using msglvl = Message_Level;

bool isMsgLvlNone(Message_Level level) 		{	return level == None;	}
bool isMsgLvlValues(Message_Level level) 	{	return (level & Values) != 0;	}
bool isMsgLvlAddresses(Message_Level level)	{	return (level & Addresses) != 0;	}
bool isMsgLvlResults(Message_Level level) 	{	return (level & Results) != 0;	}
bool isMsgLvlSummary(Message_Level level) 	{ 	return (level & Test_Summary) != 0; }
bool isMsgLvlValuesAndAddresses(Message_Level level) { return (level & Values) != 0 || (level & Addresses) != 0; }
bool isMsgLvlVerbose(Message_Level level) 	{
	return isMsgLvlValuesAndAddresses(level) || isMsgLvlResults(level) || isMsgLvlSummary(level);
}

static char newline[] = "\n";
static char testNameHeader[]  = "****************** Test Name  *******************";
static char testPhaseHeader[] =      "************* Test Phase **************";
static char testPhaseIndent[] = "    ";
static char	error_string[]		= "ERROR! ";
static char pass_string[]		= "Passed ";

void testWrappedUnsigned();
void testWrappedUnsignedLinkedListNode();

bool verifyResults(	const char *text_before,
					unsigned *expected, int expected_count,
					unsigned *results, int result_count,
					const char *text_after, Message_Level lvl);

void buildList(WrUIntList& dst, unsigned *values, int num_values, Message_Level message_level);
bool isUIntArrayMember(unsigned *array, int num, unsigned key);

void echoTestName(const char *);
void echoTestPhase(const char *);
void echoWrUInt(const char* before, WrUInt &, const char* after);
void echoLinkedList(const char* before, WrUIntList &, const char* after);


/* ********************************************************************	*/
/* ********************************************************************	*/
/*								main()									*/
/* ********************************************************************	*/
/* ********************************************************************	*/

int main(int argc, char **argv) {

	for (int i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}
	cout << endl;

    cout << "JoesSetTestBench.cpp ran on: " << __DATE__ << " at " << __TIME__ << endl << endl;

    // in order of inheritance
//    testWrappedUnsigned();
//    testWrappedUnsignedLinkedListNode();
//    testWrappedUnsignedLinkedList();
    testWrappedUnsignedSet();

    cout << endl << "JoesSetTestBench.cpp done" << endl;
    return EXIT_SUCCESS;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*						test functions								*/
/* ****************************************************************	*/
/* ****************************************************************	*/

/* ****************************************************************	*/
/*					test class WrappedUnsignedLinkedList			*/
/* ****************************************************************	*/

#pragma push_macro("returnResult")
#undef returnResult
#define returnResult()	return announceWrUIntListTest(num_tests_passed, num_tests)

bool announceWrUIntListTest(int num_passed, int num_total) {

	if (num_passed == num_total) {
    	cout << "All tests passed: " << num_passed << " out of " << num_total << " run" << endl;
    	return true;
    } else {
    	cout << "FAILED : num tests run " << num_passed << " != num_tests run " << num_total << endl;
    	return false;
    }
}

#define TEST_LIST_ADDITION_SUBTRACTION_WRUINT
//#define TEST_LIST_ADDITION_SUBTRACTION_UNSIGNED
//#define TEST_LIST_ADDITION_SUBTRACTION_LIST
//#define TEST_LIST_IS_MEMBER
//#define TEST_LIST_CONSTRUCTORS
//#define TEST_LIST_ASSIGNMENT_OPERATORS
//#define TEST_LIST_ADD_SUB_ASSIGMENT_LIST
//#define TEST_LIST_RELATIONAL_OPERATORS

bool testWrappedUnsignedLinkedList() {

	Message_Level level = Verbose;

#if defined(TEST_LIST_ADDITION_SUBTRACTION_WRUINT) or \
	defined(TEST_LIST_ADDITION_SUBTRACTION_UNSIGNED) or \
	defined(TEST_LIST_IS_MEMBER) or \
	defined(TEST_LIST_CONSTRUCTORS) or \
	defined(TEST_LIST_ASSIGNMENT_OPERATORS)

	unsigned stressing_inputs[] 	= { 5, 3, 7, 4, 6, 8, 2, 1 };
	unsigned stressing_result[]		= { 1, 2, 3, 4, 5, 6, 7, 8 };
	int stressing_inputs_size = sizeof(stressing_inputs) / sizeof(unsigned);
	int stressing_result_size = sizeof(stressing_result) / sizeof(unsigned);

	unsigned repeating_inputs[]		= { 1, 2, 3, 4, 4, 4, 3, 1 };
	unsigned repeating_result[]		= { 1, 2, 3, 4 };
	int repeating_inputs_size = sizeof(repeating_inputs) / sizeof(unsigned);
	int repeating_result_size = sizeof(repeating_result) / sizeof(unsigned);

	unsigned *inputs;
	unsigned *outputs;
	int num_outputs;
#endif

	int num_tests = 0;
	int num_tests_passed = 0;

// 	Message_Level level = Verbose;

	// this test is verified by looking at the log file
    echoTestName("default constructor");
    num_tests++;
    WrUIntList list;
    if (list.size != 0 || list.head != nullptr) {
    	cout << error_string << "default ctor did not create empty list @" << &list << ": " << list << endl << endl;
    } else {
    	num_tests_passed++;
		cout << pass_string  << "default ctor created @" << &list << ": " << list << endl << endl;
    }

#ifdef TEST_LIST_ADDITION_SUBTRACTION_WRUINT
    // insert values into the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator+(const WrUInt&) with stressing sequence");
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list = list + WrUInt(inputs[i]);
    	if (isMsgLvlVerbose(level)) {
    		cout << "list + WrUInt(" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting WrUInt& ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level);
    delete[] outputs;

    echoTestName("operator+(const WrUInt&) with repeated sequence");
    list.clear();
    inputs = repeating_inputs;
    for (int i = 0; i != repeating_inputs_size; i++) {
    	list = list + WrUInt(inputs[i]);
    	if (isMsgLvlValuesAndAddresses(level)) {
    		cout << "list + WrUInt(" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting WrUInt& ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;

    // delete values from the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator-(const WrUInt&)");
    cout << "building " << uiarray_toString(repeating_result, repeating_result_size) << endl;
    buildList(list, repeating_result, repeating_result_size, Verbose);
    cout << endl;
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list = list - WrUInt(inputs[i]);
    	if (isMsgLvlVerbose(level)) {
    		cout << "list - WrUInt(" << inputs[i] << ") from list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    if (list.size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
#endif

#ifdef TEST_LIST_ADDITION_SUBTRACTION_UNSIGNED
    // insert values into the list in order
    echoTestName("operator+(unsigned) with stressing sequence");
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list = list + inputs[i];
    	if (isMsgLvlVerbose(level)) {
    		cout << "list + unsigned " << inputs[i] << " to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting unsigned ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level);
    delete outputs;

    echoTestName("operator+(unsigned) with repeated sequence");
    list.clear();
    inputs = repeating_inputs;
    for (int i = 0; i != repeating_inputs_size; i++) {
    	list = list + inputs[i];
    	if (isMsgLvlValuesAndAddresses(level)) {
    		cout << "list + unsigned " << inputs[i] << " to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting unsigned ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;

    // insert values into the list in order
    echoTestName("operator-(unsigned)");
    cout << "building " << uiarray_toString(repeating_result, repeating_result_size) << endl;
    buildList(list, repeating_result, repeating_result_size, Verbose);
    cout << endl;
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list = list - inputs[i];
    	if (isMsgLvlVerbose(level)) {
    		cout << "list - unsigned " << inputs[i] << " from list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    if (list.size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
#endif

    // assignment operators test needs 'copied_list' to exist
#if defined(TEST_LIST_CONSTRUCTORS) or defined(TEST_LIST_ASSIGNMENT_OPERATORS)
    // these tests are verfied by looking at the log file
    //	that is with there is no 'isMsgLv.______()
    echoTestName("copy constructor");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    WrUIntList copied_list(list);
    cout 	<< " after 'WrUIntList copied_list(list);'" << endl
    		<< " with list   " << &list << ":" << endl << list << endl << endl
			<< " copied_list " << &copied_list << ":" << endl << copied_list << endl << endl;

    cout << "list.valuesString()        returns " << list.valuesString() << endl;
    cout << "copied_list.valuesString() returns " << copied_list.valuesString() << endl;
    cout << endl;
    num_tests_passed++;

    echoTestName("move constructor");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    WrUIntList moved_list(move(list));
    cout 	<< " after 'WrUIntList moved_list(list);'" << endl
    		<< " with list   " << &list << ":" << endl << list << endl << endl
			<< " moved_list  " << &moved_list << ":" << endl << moved_list << endl << endl;

    cout << "list.valuesString()        returns " << list.valuesString() << endl;
    cout << "moved_list.valuesString()  returns " << moved_list.valuesString() << endl;
    cout << endl;
    num_tests_passed++;

    //	verify that assigned set is different that copied set
    //	this verification is necessary because both copy constructor & assignment operator
    //	  use the same helper function in the class defintion file.cpp for WrUIntList
    echoTestName("operator=(const WrUIntList&)");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    WrUIntList assigned_list;
    cout << "created empty list: " << &assigned_list << ":" << endl << assigned_list << endl;
    assigned_list = list;
    cout << "copy assigned " << &list << ":" << endl << list << endl << endl
    	 << "to            " << &assigned_list << ":" << endl << assigned_list << endl << endl
		 << "which is different than previously created copy_constructor() list: " << endl << endl
		 << "copied        " << &copied_list << ":" << endl << copied_list << endl
		 << endl << endl;
    num_tests_passed++;

    echoTestName("operator=(const WrUIntList&&)");
    num_tests++;
    assigned_list.clear();
    cout << "created empty list: " << &assigned_list << ":" << endl << assigned_list << endl;
    cout << "move assigned " << &list << ":" << endl << list << endl << endl;
    assigned_list = move(list);
    cout << "to            " << &assigned_list << ":" << endl << assigned_list << endl << endl
		 << "which is different than previously created copy_constructor() list: " << endl << endl
		 << "copied        " << &copied_list << ":" << endl << copied_list << endl << endl
		 << "original list " << &list << ":" << endl << list << endl << endl
		 << endl << endl;
    num_tests_passed++;
#endif

#ifdef TEST_LIST_ASSIGNMENT_OPERATORS
    echoTestName("operator+=(WrUInt&) with stressing sequence");
    num_tests++;
    // insert values into the list in order of before head, after tail, 2nd, 2nd from last, middle
    list.clear();
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list += WrUInt(inputs[i]);
    	if (isMsgLvlVerbose(level)) {
    		cout << "+= WrUInt(" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	if (!verifyResults("after inserting WrUInt& ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level)) {
   		cout << error_string << uiarray_toString(stressing_result, stressing_result_size) << " vs " << uiarray_toString(outputs, num_outputs) << endl;
   	} else {
   		num_tests_passed++;
   	}
    delete outputs;
    cout << endl;

    echoTestName("operator+=(const WrUInt&) with repeated sequence");
    list.clear();
    inputs = repeating_inputs;
    for (int i = 0; i != repeating_inputs_size; i++) {
    	list += WrUInt(inputs[i]);
    	if (isMsgLvlValuesAndAddresses(level)) {
    		cout << "+= WrUInt(" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after += WrUInt& ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    echoTestName("operator+=(unsigned) with stressing sequence");
    // insert values into the list in order of before head, after tail, 2nd, 2nd from last, middle
    list.clear();
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list += inputs[i];
    	if (isMsgLvlVerbose(level)) {
    		cout << "+= " << inputs[i] << " to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting unsigneds ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    echoTestName("operator+=(unsigned) with repeated sequence");
    list.clear();
    inputs = repeating_inputs;
    for (int i = 0; i != repeating_inputs_size; i++) {
    	list += inputs[i];
    	if (isMsgLvlValuesAndAddresses(level)) {
    		cout << "+= " << inputs[i] << " to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after inserting unsigneds ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    // delete values from the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator-=(const WrUInt&)");
    cout << "building " << uiarray_toString(repeating_result, repeating_result_size) << endl;
    buildList(list, repeating_result, repeating_result_size, Verbose);
    cout << endl;
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list -= WrUInt(inputs[i]);
    	if (isMsgLvlVerbose(level)) {
    		cout << "removed WrUInt(" << inputs[i] << ") from list @" << &list << " list is now" << endl;
    		cout << list << endl;
    	}
    }
    if (list.size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
    cout << endl;

    // delete values from the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator-=(unsigned)");
    cout << "building " << uiarray_toString(repeating_result, repeating_result_size) << endl;
    buildList(list, repeating_result, repeating_result_size, Verbose);
    cout << endl;
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list -= inputs[i];
    	if (isMsgLvlVerbose(level)) {
    		cout << "removed " << inputs[i] << " from list @" << &list << " list is now" << endl;
    		cout << list << endl;
    	}
    }
    if (list.size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
    cout << endl;
#endif

#ifdef TEST_LIST_IS_MEMBER
    echoTestName("isMember(unsigned)");
    list.clear();
    buildList(list, stressing_inputs, stressing_inputs_size, None);
    cout << "built list for isMember() test: " << &list << " " << list.valuesString() << endl;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	if (list.isMember(stressing_inputs[i])) {
    		cout << "       list.isMember(" << stressing_inputs[i] << ") returns true" << endl;
    	} else {
    		cout << "ERROR: list.isMember(" << stressing_inputs[i] << ") returns false" << endl;
    	}
    }
    if (!list.isMember(0)) {
   		cout << "       list.isMember(0) returns false" << endl;
   	} else {
   		cout << "ERROR: list.isMember(0) returns true" << endl;
   	}
    cout << endl;

    echoTestName("isMember(WrUInt&)");
    list.clear();
    buildList(list, repeating_inputs, repeating_inputs_size, None);
    cout << "built list for isMember() test: " << &list << " " << list.valuesString() << endl;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	if (isUIntArrayMember(repeating_inputs, repeating_inputs_size, stressing_inputs[i])) {
    		// values is in repeating_inputs[], result should return true
    		if (list.isMember(WrUInt(stressing_inputs[i]))) {
    			cout << "       list.isMember(WrUInt(" << stressing_inputs[i] << ")) returns true" << endl;
    		} else {
    			cout << "ERROR: list.isMember(WrUInt(" << stressing_inputs[i] << ")) returns false" << endl;
    		}
    	} else {
    		// values is not in repeating_inputs[], result should return false
        	if (list.isMember(WrUInt(stressing_inputs[i]))) {
        		cout << "ERROR: list.isMember(WrUInt(" << stressing_inputs[i] << ")) returns true" << endl;
        	} else {
        		cout << "       list.isMember(WrUInt(" << stressing_inputs[i] << ")) returns false" << endl;
        	}
    	}
    }
    if (!list.isMember(WrUInt(0))) {
   		cout << "       list.isMember(WrUInt(0)) returns false" << endl;
   	} else {
   		cout << "ERROR: list.isMember(WrUInt(0)) returns true" << endl;
   	}
    cout << endl;
#endif

#if defined(TEST_LIST_ADDITION_SUBTRACTION_LIST)\
	or defined(TEST_LIST_ADD_SUB_ASSIGMENT_LIST)\
	or defined(TEST_LIST_RELATIONAL_OPERATORS)
    unsigned lower_list[] = { 1, 2, 3, 4, 5, 6 };
    unsigned upper_list[] = { 4, 5, 6, 7, 8, 9 };
    unsigned total_list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int lower_list_size = 6;
    int upper_list_size = 6;
    int total_list_size = 9;
    WrUIntList list1;
    WrUIntList list2;
    WrUIntList list3;
#endif

#ifdef TEST_LIST_ADDITION_SUBTRACTION_LIST
    echoTestName("add/sub operators(list)");

    // add two empty sets
    echoTestPhase("add two empty lists");
    list1.clear();
    list2.clear();
    list3.clear();

    cout << endl;
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("add list + empty");
    buildList(list1, lower_list, lower_list_size, None);
    list2.clear();
    list3.clear();
    // add an empty set to an existing set
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    // add an empty set to an existing set
    echoTestPhase("add empty + list");
    list2 = list1;
    list1.clear();
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("add lower + upper");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("add upper + lower");
    buildList(list1, upper_list, upper_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("add list1 + list1");
    buildList(list1, upper_list, upper_list_size, None);
    list2.clear();
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list1 @" << &list1 << ": " << endl
			<< list1 << endl << endl << " = " << endl << endl;
    list3 = list1 + list1;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("add list1 + list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 + list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestPhase("list1 = list1 + list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list1 = list1 + list2;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

    echoTestPhase("list2 = list1 + list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list2 = list1 + list2;
    cout	<< "list2 @" << &list2 << ": " << endl
    		<< list2 << endl << endl;

    echoTestPhase("list2 = list1 + list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " + " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list2 = list1 + list2;
    cout	<< "list2 @" << &list2 << ": " << endl
    		<< list2 << endl << endl;

    /* ********************************************	*/
    /*					subtraction					*/
    /* ********************************************	*/

    echoTestName("add/sub operators(list)");

    echoTestPhase("list3 = empty - empty");
    buildList(list1, lower_list, lower_list_size, None);
    list1.clear();
    list2.clear();
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 - list2;
    cout	<< "list2 @" << &list2 << ": " << endl
    		<< list2 << endl << endl;

    echoTestPhase("list3 = list1 - empty");
    buildList(list1, lower_list, lower_list_size, None);
    list2.clear();
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 - list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestName("list3 = list1 - list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 - list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestName("list3 = list1 - list2");
    buildList(list1, upper_list, upper_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list3 = list1 - list2;
    cout	<< "list3 @" << &list3 << ": " << endl
    		<< list3 << endl << endl;

    echoTestName("list1 = list1 - list1");
    buildList(list1, lower_list, lower_list_size, None);
    list2.clear();
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list1 @" << &list1 << ": " << endl
			<< list1 << endl << endl << " = " << endl << endl;
    list1 = list1 - list1;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

    echoTestName("list1 = list1 - list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " - " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list1 = list1 - list2;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

    echoTestName("list1 = list2 - list1");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, upper_list, upper_list_size, None);
    list3.clear();
    cout 	<< "list2 @" << &list2 << ": " << endl
    		<< list2 << endl << endl << " - " << endl << endl
    		<< "list1 @" << &list1 << ": " << endl
			<< list1 << endl << endl << " = " << endl << endl;
    list1 = list2 - list1;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

#endif

#ifdef TEST_LIST_ADD_SUB_ASSIGMENT_LIST
    echoTestName("add/sub assignment operators(list)");

    echoTestName("list1 += list2");
    buildList(list1, upper_list, upper_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " += " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list1 += list2;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

    echoTestName("list1 -= list2");
    buildList(list1, upper_list, upper_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    list3.clear();
    cout 	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl << " -= " << endl << endl
    		<< "list2 @" << &list2 << ": " << endl
			<< list2 << endl << endl << " = " << endl << endl;
    list1 -= list2;
    cout	<< "list1 @" << &list1 << ": " << endl
    		<< list1 << endl << endl;

#endif

#ifdef TEST_LIST_RELATIONAL_OPERATORS
    list1.clear();
    list2.clear();
    list3.clear();

    echoTestName("empty == empty");
    list1.clear();
    list2.clear();
    cout << " list1 @" << &list1 << " " << list1.valuesString()
    	 << " == list2 @" << &list2 << " " << list2.valuesString() << " returns "
		 << (list1 == list2 ? "true" : "false") << endl << endl;

    echoTestName("list1 == empty");
    buildList(list1, lower_list, lower_list_size, None);
    list2.clear();
    cout << " list1 @" << &list1 << " " << list1.valuesString()
    	 << " == list2 @" << &list2 << " " << list2.valuesString() << " returns "
		 << (list1 == list2 ? "true" : "false") << endl << endl;

    echoTestName("list1 == list1");
    buildList(list1, lower_list, lower_list_size, None);
    list2.clear();
    cout << " list1 @" << &list1 << " " << list1.valuesString()
    	 << " == list1 @" << &list1 << " " << list1.valuesString() << " returns "
		 << (list1 == list1 ? "true" : "false") << endl << endl;

    echoTestName("list1 == list2");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list2, lower_list, lower_list_size, None);
    cout << " list1 @" << &list1 << " " << list1.valuesString()
    	 << " == list2 @" << &list2 << " " << list2.valuesString() << " returns "
		 << (list1 == list2 ? "true" : "false") << endl << endl;

    echoTestName("list1 == list3");
    buildList(list1, lower_list, lower_list_size, None);
    buildList(list3, total_list, total_list_size, None);
    cout << " list1 @" << &list1 << " " << list1.valuesString()
    	 << " == list3 @" << &list3 << " " << list3.valuesString() << " returns "
		 << (list1 == list3 ? "true" : "false") << endl << endl;

    echoTestName("list3 == list1");
    cout << " list3 @" << &list3 << " " << list3.valuesString()
    	 << " == list1 @" << &list1 << " " << list1.valuesString() << " returns "
		 << (list3 == list1 ? "true" : "false") << endl << endl;

    echoTestName("list3 != list1");
    cout << " list3 @" << &list3 << " " << list3.valuesString()
    	 << " != list1 @" << &list1 << " " << list1.valuesString() << " returns "
		 << (list3 != list1 ? "true" : "false") << endl << endl;

#endif

    returnResult();
}
#undef returnResult
#pragma pop_macro("returnResult")


/* ****************************************************************	*/
/*			test class WrappedUnsignedLinkedListNode				*/
/* ****************************************************************	*/

void testWrappedUnsignedLinkedListNode() {

	echoTestName(" WrUIntNode Constructors");
	WrUIntNode node1;
	cout << "default constructor: created node1 @" << node1.toString() << endl;
	WrUIntNode node2(WrUInt(2));
	cout << "c'tor(WrUInt(2)):    created node2 @" << node2.toString() << endl;
	WrUIntNode node3(node2);
	cout << "c'tor(node2):        created node3 @" << node3.toString() << endl;

	WrUIntNode node4;
	node4.data = node3.data;
	node4.next = &node1;
	cout 	<< "node4 contains data = node3.data " << node3.data
			<< " and points to node1 " << &node1 << endl
			<< " node4: " << node4.toString() << endl;
	WrUIntNode node5(node2.data = 10, &node3);
	cout 	<< "node 5 contains data from node2 " << node2.data
			<< " and points to node 3: " << &node3 << endl
			<< " which results in node5: " << node5.toString() << endl;
	cout 	<< "after assignment node3(" << node3 << ") = node 5(" << node5 << ") " << endl
			<< (node3 = node5) << node3.toString() << endl;
}


/* ****************************************************************	*/
/*					test class WrappedUnsigned						*/
/* ****************************************************************	*/

//#define TEST_WRUINT_DEFAULT_CONSTRUCTOR
//#define TEST_WRUINT_COPY_CONSTRUCTOR
//#define TEST_WRUINT_ASSIGNMENT_OPERATORS
//#define TEST_WRUINT_UNARY_OPERATORS
//#define TEST_WRUINT_COMPOUND_ASSIGNMENT_OPERATORS
//#define TEST_WRUINT_ARITHMETIC_OPERATORS
#define TEST_WRUINT_COMPARE_FUNCTIONS
#define TEST_WRUINT_COMPARE_OPERATORS

void testWrappedUnsigned() {

	echoTestName("test WrappedUnsigned");

#ifdef TEST_WRUINT_DEFAULT_CONSTRUCTOR
	echoTestName("default constructor");
	cout << "creating default x" << endl;
	WrUInt x;
	echoWrUInt("x: ", x, newline);
	cout << endl;
#endif

#ifdef TEST_WRUINT_ASSIGNMENT_OPERATORS
	echoTestName("operator=(unsigned)");
	echoWrUInt("x: ", x, newline);
	cout << "x = 1 returns " << (x = 1).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << endl;
#endif

#ifdef TEST_WRUINT_COPY_CONSTRUCTOR
	echoTestName("copy constructor");
	echoWrUInt("x: ", x, newline);
	cout << "WrUInt y(x);" << endl;
	WrUInt y(x);
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;
#else
	cout << "creating default y" << endl;
	WrUInt y;
	echoWrUInt("y: ", y, newline);
	cout << endl;
#endif

#ifdef TEST_WRUINT_ASSIGNMENT_OPERATORS
	echoTestName("operator=(WrUInt&)");
	WrUInt z;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	echoWrUInt("z: ", z, newline);
	cout << "z = x returns " << (z = x).toString() << endl;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	echoWrUInt("z: ", z, newline);
	cout << endl;
#else
	cout << "creating default z" << endl;
	WrUInt z;
	echoWrUInt("z: ", z, newline);
	cout << endl;
#endif

#ifdef TEST_WRUINT_UNARY_OPERATORS
	echoTestName("unary operators ++ --");
	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = ++x returns " << (y = ++x).toString() << endl;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;

	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = x++ returns " << (y = x++).toString() << endl;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;

	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = --x returns " << (y = --x).toString() << endl;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;

	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = x-- returns " << (y = x--).toString() << endl;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;

	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = ++x--" << newline;
	y = ++x--;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;

	x = 1;
	y = 5;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << "y = --x++" << newline;
	y = --x++;
	echoWrUInt("x: ", x, newline);
	echoWrUInt("y: ", y, newline);
	cout << endl;
#endif


#ifdef TEST_WRUINT_COMPOUND_ASSIGNMENT_OPERATORS
	echoTestName("*** COMPOUND ASSIGNMENT OPERATORS *** ");
	y = 3;
	cout << "x = " << x << " y = " << y << endl;

	// +=

	echoTestName("operator+=(unsigned)");
	x = 10;
	cout << "x += 3 returns " << (x += 3).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	echoTestName("operator+=(WrUInt&)");
	x = 10;
	cout << "x += y returns " << (x += y).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	// -=

	echoTestName("operator-=(unsigned)");
	x = 10;
	cout << "x -= 3 returns " << (x -= 3).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	echoTestName("operator-=(WrUInt&)");
	x = 10;
	cout << "x -= y returns " << (x -= y).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	// *=

	echoTestName("operator*=(unsigned)");
	x = 10;
	cout << "x *= 3 returns " << (x *= 3).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	echoTestName("operator*=(WrUInt&)");
	x = 10;
	cout << "x *= y returns " << (x *= y).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	// /=

	echoTestName("operator/=(unsigned)");
	x = 10;
	cout << "x /= 3 returns " << (x /= 3).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	echoTestName("operator/=(WrUInt&)");
	x = 10;
	cout << "x /= y returns " << (x /= y).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	// %=

	echoTestName("operator%=(unsigned)");
	x = 10;
	cout << "x %= 3 returns " << (x %= 3).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;

	echoTestName("operator%=(WrUInt&)");
	x = 10;
	cout << "x %= y returns " << (x %= y).toString() << endl;
	echoWrUInt("x: ", x, newline);
	cout << newline;
#endif

#ifdef TEST_WRUINT_ARITHMETIC_OPERATORS
	echoTestName(" ARITHMETIC OPERATORS");

	//	+

	echoTestName("operator+(unsigned)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x + 2 returns " << (z = x + 2).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	echoTestName("operator+(WrUInt&)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x + y returns " << (z = x + y).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	//	-

	echoTestName("operator-(unsigned)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x - 2 returns " << (z = x - 2).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	echoTestName("operator-(WrUInt&)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x - y returns " << (z = x - y).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	// *

	echoTestName("operator*(unsigned)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x * 2 returns " << (z = x * 2).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	echoTestName("operator*(WrUInt&)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x * y returns " << (z = x * y).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	// /

	echoTestName("operator/(unsigned)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x / 2 returns " << (z = x / 2).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	echoTestName("operator/(WrUInt&)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x / y returns " << (z = x / y).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	// %

	echoTestName("operator+(unsigned)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x % 2 returns " << (z = x % 2).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;

	echoTestName("operator+(WrUInt&)");
	x = 9; y = 2; z = 0;
	cout << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << "z = x % y returns " << (z = x % y).toString() << endl;
	echoWrUInt("x = ", x, newline);
	echoWrUInt("y = ", y, newline);
	echoWrUInt("z = ", z, newline);
	cout << endl;
#endif

#ifdef TEST_WRUINT_COMPARE_FUNCTIONS
{
	WrUInt x(2);
	WrUInt y(4);
	WrUInt z(2);
	echoTestName("compare(WrUInt())");
	cout << "x(" << x << ").compare(y(" << y << ")) returns: " << x.compare(y) << endl;
	cout << "y(" << y << ").compare(x(" << x << ")) returns: " << y.compare(x) << endl;
	cout << "x(" << x << ").compare(x(" << x << ")) returns: " << x.compare(x) << endl;
	cout << "x(" << x << ").compare(z(" << z << ")) returns: " << x.compare(z) << endl;
	cout << endl;

	echoTestName("compare(unsigned)");
	cout << "(x: " << x << ").compare(unsigned 1) returns: " << x.compare(1) << endl;
	cout << "(x: " << x << ").compare(unsigned 2) returns: " << x.compare(2) << endl;
	cout << "(x: " << x << ").compare(unsigned 3) returns: " << x.compare(3) << endl;
	cout << endl;
}
#endif

#ifdef TEST_WRUINT_COMPARE_OPERATORS
{
	echoTestName("comparison operators(unsigned)");
	WrUInt x(2);
	cout << "x(" << x << ") >  1 returns: " << (x >  1 ? "true" : "false") << endl;
	cout << "x(" << x << ") >  2 returns: " << (x >  2 ? "true" : "false") << endl;
	cout << "x(" << x << ") >  3 returns: " << (x >  3 ? "true" : "false") << endl;
	cout << "x(" << x << ") >= 1 returns: " << (x >= 1 ? "true" : "false") << endl;
	cout << "x(" << x << ") >= 2 returns: " << (x >= 2 ? "true" : "false") << endl;
	cout << "x(" << x << ") >= 3 returns: " << (x >= 3 ? "true" : "false") << endl;
	cout << "x(" << x << ") <  1 returns: " << (x <  1 ? "true" : "false") << endl;
	cout << "x(" << x << ") <  2 returns: " << (x <  2 ? "true" : "false") << endl;
	cout << "x(" << x << ") <  3 returns: " << (x <  3 ? "true" : "false") << endl;
	cout << "x(" << x << ") <= 1 returns: " << (x <= 1 ? "true" : "false") << endl;
	cout << "x(" << x << ") <= 2 returns: " << (x <= 2 ? "true" : "false") << endl;
	cout << "x(" << x << ") <= 3 returns: " << (x <= 3 ? "true" : "false") << endl;
	cout << "x(" << x << ") == 1 returns: " << (x == 1 ? "true" : "false") << endl;
	cout << "x(" << x << ") == 2 returns: " << (x == 2 ? "true" : "false") << endl;
	cout << "x(" << x << ") != 1 returns: " << (x != 1 ? "true" : "false") << endl;
	cout << "x(" << x << ") != 2 returns: " << (x != 2 ? "true" : "false") << endl;
	cout << endl;

	echoTestName("comparison operators(WrUInt())");
	x = 2;
	WrUInt y(2);
	WrUInt z(1);
	WrUInt w(3);
	cout << "x(" << x << ") >  z(" << z << ") returns: " << (x >  z ? "true" : "false") << endl;
	cout << "x(" << x << ") >  y(" << y << ") returns: " << (x >  y ? "true" : "false") << endl;
	cout << "x(" << x << ") >  x(" << x << ") returns: " << (x >  x ? "true" : "false") << endl;
	cout << "x(" << x << ") >  w(" << w << ") returns: " << (x >  w ? "true" : "false") << endl;
	cout << "x(" << x << ") >= z(" << z << ") returns: " << (x >= z ? "true" : "false") << endl;
	cout << "x(" << x << ") >= y(" << y << ") returns: " << (x >= y ? "true" : "false") << endl;
	cout << "x(" << x << ") >= x(" << x << ") returns: " << (x >= x ? "true" : "false") << endl;
	cout << "x(" << x << ") >= w(" << w << ") returns: " << (x >= w ? "true" : "false") << endl;
	cout << "x(" << x << ") <  z(" << z << ") returns: " << (x <  z ? "true" : "false") << endl;
	cout << "x(" << x << ") <  y(" << y << ") returns: " << (x <  y ? "true" : "false") << endl;
	cout << "x(" << x << ") <  x(" << x << ") returns: " << (x <  x ? "true" : "false") << endl;
	cout << "x(" << x << ") <  w(" << w << ") returns: " << (x <  w ? "true" : "false") << endl;
	cout << "x(" << x << ") <= z(" << z << ") returns: " << (x <= z ? "true" : "false") << endl;
	cout << "x(" << x << ") <= y(" << y << ") returns: " << (x <= y ? "true" : "false") << endl;
	cout << "x(" << x << ") <= x(" << x << ") returns: " << (x <= x ? "true" : "false") << endl;
	cout << "x(" << x << ") <= w(" << w << ") returns: " << (x <= w ? "true" : "false") << endl;
	cout << "x(" << x << ") == z(" << z << ") returns: " << (x == z ? "true" : "false") << endl;
	cout << "x(" << x << ") == y(" << y << ") returns: " << (x == y ? "true" : "false") << endl;
	cout << "x(" << x << ") == x(" << x << ") returns: " << (x == x ? "true" : "false") << endl;
	cout << "x(" << x << ") != z(" << z << ") returns: " << (x != z ? "true" : "false") << endl;
	cout << "x(" << x << ") != y(" << y << ") returns: " << (x != y ? "true" : "false") << endl;
	cout << "x(" << x << ") != x(" << x << ") returns: " << (x != x ? "true" : "false") << endl;
	cout << endl;
}
#endif
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*							utilities								*/
/* ****************************************************************	*/
/* ****************************************************************	*/

void buildList(WrUIntList& dst, unsigned *values, int num_values, Message_Level level) {

	dst.clear();
	for (int i = 0; i != num_values; i++) {
		dst += WrUInt(values[i]);
	}

	if (isMsgLvlVerbose(level)) {
		cout << "created list @" << &dst << ":" << endl;
		cout << dst << endl;
	}
}

bool isUIntArrayMember(unsigned *array, int num, unsigned key) {

	for (int i = 0; i != num; i++) {
		if (array[i] == key)
			return true;
	}

	return false;
}


bool verifyResults(	const char *before,
					unsigned *expected, int expected_count,
					unsigned *results, int results_count,
					char *after, Message_Level lvl)
{
	bool passed = true;

	cout << before;

	if (isMsgLvlVerbose(lvl)) {
		cout << " expecting: " << uiarray_toString(expected, expected_count);
		cout << " against received: " << uiarray_toString(results, results_count) << endl;
	}

	if (expected_count != results_count) {
		if (isMsgLvlSummary(lvl) || isMsgLvlResults(lvl)) {
			cout << "num expecteds " << expected_count << " != num results " << results_count << endl;
			return false;
		}
	}

	for (int i = 0; i != expected_count; i++)  {
		if (true || isMsgLvlResults(lvl)) {
			if (expected[i] != results[i]) {
				cout << " " << setw(2) << i << " ERROR expected " << expected[i] << " != " << results[i] << endl;
				passed = false;
			} else {
				cout << " " << setw(2) << i << "       expected " << expected[i] << " == " << results[i] << endl;
			}
		}
	}

	if (isMsgLvlSummary(lvl)) {
		cout << " verifyResults returning " << (passed ? "true" : "false");
	}
	cout << after;

	return passed;
}


/* ****************************************************************************************	*/
/* ****************************************************************************************	*/
/*									printing functions										*/
/* ****************************************************************************************	*/
/* ****************************************************************************************	*/

void echoTestName(const char *text) {

	// math to center *text
	int padding_width  = strlen(testNameHeader) - strlen(text);
	int padding_before = padding_width / 2;
	// if padding_width is odd, ad an extra space to the right side of the line
	int padding_after  = padding_before + padding_width % 2;

	cout << endl;
	cout << testNameHeader << endl << endl;
	cout << setw(padding_before) << left << "*"
		 << text
		 << setw(padding_after) << right << "*" << endl << endl;
	cout << testNameHeader << endl;
	cout << endl;
}

void echoTestPhase(const char *text) {

	int padding_width  = strlen(testPhaseHeader) - strlen(text);
	int padding_before = padding_width / 2;
	int padding_after  = padding_before + padding_width % 2;

	cout << testPhaseIndent <<testPhaseHeader << endl;
	cout << testPhaseIndent << setw(padding_before) << left << "*"
		 << text
		 << setw(padding_after) << right << "*" << endl;
	cout << testPhaseIndent << testPhaseHeader << endl << endl;
}

void echoLinkedList(const char* before, WrUIntList &list, const char *after) {
	cout << before << "@" << &list << ": " << list.toString() << after;
}


void echoWrUInt(const char* before, WrUInt & value, const char* after) {
	cout << before << value.toString() << after;
}

std::string uiarray_toString(const unsigned *values, int count) {

	std::stringstream retval;

	retval << "{ ";

	for (int i = count ; i != 0; i--) {
		retval << setw(2) << *values++;
	}

	retval << " }";
	return retval.str();
}

