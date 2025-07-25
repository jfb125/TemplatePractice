#include "MyOrderedSetTestBench.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include "WrappedUnsigned.h"
#include "MyOrderedSet.h"
#include "TestMyOrderedSet.h"

using namespace std;

//using  = WrappedUnsigned;
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

//static char newline[] = "\n";
static char testNameHeader[]  = "***************************** Test Name  ******************************";
static char testPhaseHeader[] =      "************************ Test Phase *************************";
static char testPhaseIndent[] = "    ";
//static char	error_string[]		= "ERROR! ";
//static char pass_string[]		= "Passed ";

void testMyLinkedListNode();

template <typename T>
void buildList(MyLinkedList<T> &dst, T *values, int num_values, Message_Level message_level);
template <typename T>
bool isUIntArrayMember(T *array, int num, T key);

void echoTestName(const char *);
void echoTestPhase(const char *);
template <typename T>
void echoSet(const char* prefix,  MyOrderedSet<T> &, const char* suffix);
template <typename T>
void echoLinkedList(const char* before, MyLinkedList<T> &, const char* after);

template <typename T>
std::string arrayToString(T *values, int count);
template <typename T>
bool verifyResults(	std::string before,
					T *expected, int expected_count,
					T *results, int result_count,
					std::string after, Message_Level lvl);

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
    testMyOrderedSet();

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
#if 0
#pragma push_macro("returnResult")
#undef returnResult
#define returnResult()	return announceMyLinkedListTest(num_tests_passed, num_tests)

bool announceMyLinkedListTest(int num_passed, int num_total) {

	if (num_passed == num_total) {
    	cout << "All tests passed: " << num_passed << " out of " << num_total << " run" << endl;
    	return true;
    } else {
    	cout << "FAILED : num tests run " << num_passed << " != num_tests run " << num_total << endl;
    	return false;
    }
}

//#define TEST_LIST_INSERTION_DELETION_OBJECT
//#define TEST_LIST_ADDITION_SUBTRACTION_LIST
//#define TEST_LIST_IS_MEMBER
//#define TEST_LIST_CONSTRUCTORS
//#define TEST_LIST_ASSIGNMENT_OPERATORS
//#define TEST_LIST_ADD_SUB_ASSIGMENT_LIST
//#define TEST_LIST_RELATIONAL_OPERATORS

bool testWrappedUnsignedLinkedList() {

using TypeUnderTest = int;
using TUT = TypeUnderTest;

	Message_Level level = Verbose;

#if defined(TEST_LIST_INSERTION_DELETION_OBJECT) or \
	defined(TEST_LIST_IS_MEMBER) or \
	defined(TEST_LIST_CONSTRUCTORS) or \
	defined(TEST_LIST_ASSIGNMENT_OPERATORS)

	TUT stressing_inputs[] 	= { 5, 3, 7, 4, 6, 8, 2, 1 };
	TUT stressing_result[]		= { 1, 2, 3, 4, 5, 6, 7, 8 };
	int stressing_inputs_size = sizeof(stressing_inputs) / sizeof(T);
	int stressing_result_size = sizeof(stressing_result) / sizeof(T);

	TUT repeating_inputs[]		= { 1, 2, 3, 4, 4, 4, 3, 1 };
	TUT repeating_result[]		= { 1, 2, 3, 4 };
	int repeating_inputs_size = sizeof(repeating_inputs) / sizeof(T);
	int repeating_result_size = sizeof(repeating_result) / sizeof(T);

	TUT *inputs;
	TUT *outputs;
	int num_outputs;
#endif

	int num_tests = 0;
	int num_tests_passed = 0;

// 	Message_Level level = Verbose;

	// this test is verified by looking at the log file
    echoTestName("default constructor");
    num_tests++;
    MyLinkedList<TUT> list;
    if (list.m_size != 0 || list.m_head != nullptr) {
    	cout << error_string << "default ctor did not create empty list @" << &list << ": " << list << endl << endl;
    } else {
    	num_tests_passed++;
		cout << pass_string  << "default ctor created @" << &list << ": " << list << endl << endl;
    }

    // assignment operators test needs 'copied_list' to exist
#if defined(TEST_LIST_CONSTRUCTORS) or defined(TEST_LIST_ASSIGNMENT_OPERATORS)
    // these tests are verfied by looking at the log file
    //	that is with there is no 'isMsgLv.______()
    echoTestName("copy constructor");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    MyLinkedList copied_list(list);
    cout 	<< " after 'MyLinkedList copied_list(list);'" << endl
    		<< " with list   " << &list << ":" << endl << list << endl << endl
			<< " copied_list " << &copied_list << ":" << endl << copied_list << endl << endl;

    cout << "list.valuesString()        returns " << list.valuesString() << endl;
    cout << "copied_list.valuesString() returns " << copied_list.valuesString() << endl;
    cout << endl;
    num_tests_passed++;

    echoTestName("move constructor");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    MyLinkedList moved_list(move(list));
    cout 	<< " after 'MyLinkedList moved_list(list);'" << endl
    		<< " with list   " << &list << ":" << endl << list << endl << endl
			<< " moved_list  " << &moved_list << ":" << endl << moved_list << endl << endl;

    cout << "list.valuesString()        returns " << list.valuesString() << endl;
    cout << "moved_list.valuesString()  returns " << moved_list.valuesString() << endl;
    cout << endl;
    num_tests_passed++;

    //	verify that assigned set is different that copied set
    //	this verification is necessary because both copy constructor & assignment operator
    //	  use the same helper function in the class defintion file.cpp for MyLinkedList
    echoTestName("operator=(const MyLinkedList&)");
    num_tests++;
    buildList(list, repeating_inputs, repeating_inputs_size, Verbose);
    MyLinkedList assigned_list;
    cout << "created empty list: " << &assigned_list << ":" << endl << assigned_list << endl;
    assigned_list = list;
    cout << "copy assigned " << &list << ":" << endl << list << endl << endl
    	 << "to            " << &assigned_list << ":" << endl << assigned_list << endl << endl
		 << "which is different than previously created copy_constructor() list: " << endl << endl
		 << "copied        " << &copied_list << ":" << endl << copied_list << endl
		 << endl << endl;
    num_tests_passed++;

    echoTestName("operator=(const MyLinkedList&&)");
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
    echoTestName("operator+=(&) with stressing sequence");
    num_tests++;
    // insert values into the list in order of before head, after tail, 2nd, 2nd from last, middle
    list.clear();
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list += inputs[i];
    	if (isMsgLvlVerbose(level)) {
    		cout << "+= (" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	if (!verifyResults("after inserting & ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level)) {
   		cout << error_string << array_toString(stressing_result, stressing_result_size) << " vs " << array_toString(outputs, num_outputs) << endl;
   	} else {
   		num_tests_passed++;
   	}
    delete outputs;
    cout << endl;

    echoTestName("operator+=(const &) with repeated sequence");
    list.clear();
    inputs = repeating_inputs;
    for (int i = 0; i != repeating_inputs_size; i++) {
    	list += (inputs[i]);
    	if (isMsgLvlValuesAndAddresses(level)) {
    		cout << "+= (" << inputs[i] << ") to list @" << &list << " list is now" << endl;
    		cout << list << endl << endl;
    	}
    }
    num_outputs = list.getValues(&outputs);
   	verifyResults("after += & ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    echoTestName("operator+=(T) with stressing sequence");
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
   	verifyResults("after inserting Ts ", stressing_result, stressing_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    echoTestName("operator+=(T) with repeated sequence");
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
   	verifyResults("after inserting Ts ", repeating_result, repeating_result_size, outputs, num_outputs, newline, level);
    delete outputs;
    cout << endl;

    // delete values from the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator-=(const &)");
    cout << "building " << array_toString(repeating_result, repeating_result_size) << endl;
    buildList(list, repeating_result, repeating_result_size, Verbose);
    cout << endl;
    inputs = stressing_inputs;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	list -= (inputs[i]);
    	if (isMsgLvlVerbose(level)) {
    		cout << "removed (" << inputs[i] << ") from list @" << &list << " list is now" << endl;
    		cout << list << endl;
    	}
    }
    if (list.m_size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
    cout << endl;

    // delete values from the list in order of before head, after tail, 2nd, 2nd from last, middle
    echoTestName("operator-=(T)");
    cout << "building " << array_toString(repeating_result, repeating_result_size) << endl;
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
    if (list.m_size != 0) {
    	cout << "removing all elements of list failed, list not empty: " << &list << " " << list << endl;
    } else {
    	cout << "removed all elements of list, list is now empty: " << &list << " " << list << endl;
    }
    cout << endl;
#endif

#ifdef TEST_LIST_IS_MEMBER
    echoTestName("isMember(T)");
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

    echoTestName("isMember(&)");
    list.clear();
    buildList(list, repeating_inputs, repeating_inputs_size, None);
    cout << "built list for isMember() test: " << &list << " " << list.valuesString() << endl;
    for (int i = 0; i != stressing_inputs_size; i++) {
    	if (isUIntArrayMember(repeating_inputs, repeating_inputs_size, stressing_inputs[i])) {
    		// values is in repeating_inputs[], result should return true
    		if (list.isMember((stressing_inputs[i]))) {
    			cout << "       list.isMember((" << stressing_inputs[i] << ")) returns true" << endl;
    		} else {
    			cout << "ERROR: list.isMember((" << stressing_inputs[i] << ")) returns false" << endl;
    		}
    	} else {
    		// values is not in repeating_inputs[], result should return false
        	if (list.isMember((stressing_inputs[i]))) {
        		cout << "ERROR: list.isMember((" << stressing_inputs[i] << ")) returns true" << endl;
        	} else {
        		cout << "       list.isMember((" << stressing_inputs[i] << ")) returns false" << endl;
        	}
    	}
    }
    if (!list.isMember((0))) {
   		cout << "       list.isMember((0)) returns false" << endl;
   	} else {
   		cout << "ERROR: list.isMember((0)) returns true" << endl;
   	}
    cout << endl;
#endif

#if defined(TEST_LIST_ADDITION_SUBTRACTION_LIST)\
	or defined(TEST_LIST_ADD_SUB_ASSIGMENT_LIST)\
	or defined(TEST_LIST_RELATIONAL_OPERATORS)
    T lower_list[] = { 1, 2, 3, 4, 5, 6 };
    T upper_list[] = { 4, 5, 6, 7, 8, 9 };
    T total_list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int lower_list_size = 6;
    int upper_list_size = 6;
    int total_list_size = 9;
    MyLinkedList list1;
    MyLinkedList list2;
    MyLinkedList list3;
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
#endif

/* ****************************************************************	*/
/*			test class WrappedUnsignedLinkedListNode				*/
/* ****************************************************************	*/

void testMyLinkedListNode() {

using TypeUnderTest = int;
using TUT = TypeUnderTest;

	echoTestName(" MyLinkedListNode Constructors");
	MyLinkedListNode<TUT> node1;
	cout << "default constructor: created node1 @" << node1.toString() << endl;
	MyLinkedListNode node2((2));
	cout << "c'tor((2)):    created node2 @" << node2.toString() << endl;
	MyLinkedListNode node3(node2);
	cout << "c'tor(node2):        created node3 @" << node3.toString() << endl;

	MyLinkedListNode<TUT> node4;
	node4.m_data = node3.m_data;
	node4.m_next = &node1;
	cout 	<< "node4 contains.m_data = node3.m_data " << node3.m_data
			<< " and points to node1 " << &node1 << endl
			<< " node4: " << node4.toString() << endl;
	MyLinkedListNode<TUT> node5(node2.m_data = 10, &node3);
	cout 	<< "node 5 contains.m_data from node2 " << node2.m_data
			<< " and points to node 3: " << &node3 << endl
			<< " which results in node5: " << node5.toString() << endl;
	cout 	<< "after assignment node3(" << node3 << ") = node 5(" << node5 << ") " << endl
			<< (node3 = node5) << node3.toString() << endl;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*							utilities								*/
/* ****************************************************************	*/
/* ****************************************************************	*/

template <typename T>
void buildList(MyLinkedList<T> &dst, T *values, int num_values, Message_Level level) {

	dst.clear();
	for (int i = 0; i != num_values; i++) {
		dst += (values[i]);
	}

	if (isMsgLvlVerbose(level)) {
		cout << "created list @" << &dst << ":" << endl;
		cout << dst << endl;
	}
}

template <typename T>
bool isUIntArrayMember(T *array, int num, T key) {

	for (int i = 0; i != num; i++) {
		if (array[i] == key)
			return true;
	}

	return false;
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

template <typename T>
void echoLinkedList(const char* before, MyLinkedList<T> &list, const char *after) {
	cout << before << "@" << &list << ": " << list.toString() << after;
}

template <typename T>
void echoSet(const char* prefix,  MyOrderedSet<T> &value, const char* suffix) {
	cout << prefix << value.toString() << suffix;
}
