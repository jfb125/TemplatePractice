#include "ClassWrUIntSet_Tests.h"
#include "WrappedUnsignedSet.h"

using namespace std;

using WrUInt = WrappedUnsigned;
using WrUIntNode = LinkedListNode;
using WrUIntList = WrappedUnsignedLinkedList;
using WrUIntSet = WrappedUnsignedSet;

#undef SET_OPERATION
#define SET_OPERATIONS\
	SET_OPERATION(SET_UNION_PLUS = 0, "+")\
	SET_OPERATION(SET_UNION_OR, "|")\
	SET_OPERATION(SET_DIFFERENCE, "-")\
	SET_OPERATION(SET_INTERSECTION, "&")\
	SET_OPERATION(SET_UNIQUE, "^")\
	SET_OPERATION(SET_EQUAL, "==")\
	SET_OPERATION(SET_INEQUAL, "!=")\
	SET_OPERATION(SET_ASSIGN, "=")\
	SET_OPERATION(SET_ASSIGN_UNION_PLUS, "+=")\
	SET_OPERATION(SET_ASSIGN_UNION_OR, "|=")\
	SET_OPERATION(SET_ASSIGN_DIFFERENCE, "-=")\
	SET_OPERATION(SET_ASSIGN_INTERSECTION, "&=")\
	SET_OPERATION(SET_ASSIGN_UNIQUE, "^=")\
	SET_OPERATION(SET_ASSIGN_UNSIGNED_ADD, "+=")\
	SET_OPERATION(SET_ASSIGN_UNSIGNED_SUB, "-=")\
	SET_OPERATION(SET_UNSIGNED_ADD, "+")\
	SET_OPERATION(SET_UNSIGNED_SUB, "-")\
	SET_OPERATION(SET_NOP, "nop")

#define SET_OPERATION(enm, str) enm,
enum Set_Operations { SET_OPERATIONS };
#undef SET_OPERATION
#define SET_OPERATION(enm, str) str,
static const char* set_operation_strings[] = { SET_OPERATIONS "" }; // create , "" } final string
static const char**s_op_str = set_operation_strings;
#undef SET_OPERATION
#undef SET_OPERATIONS

static char newline[] 			= "\n";
static char error_str[]			= "!!! ERROR !!! ";
static char passed_str[]		= "passed ";
static char failed_str[]		= "failed ";
static char abort_str[]			= "ABORTING TEST: ";
static char values_passed[]		= "verify values passed";
static char values_failed[]		= "verify values failed";
static char storage_passed[]	= "storage type passed";
static char storage_failed[]	= "storage type failed";
static char empty_str[]			= "";

static const bool failed = false;
static const bool passed = true;

#pragma push_macro("cout_count")
#define cout_count() do { cout << endl; announceResults(passed_test_count, test_count); cout << endl; } while(false)


/* ********************************************************************	*/
/*						make a record of contents of set				*/
/* ********************************************************************	*/

class Node_Contents {
public:

	Node_Contents() { address = nullptr; data = 0; next = nullptr; }
	Node_Contents(const WrUIntNode *node) {
		address = node;
		data = node->data;
		next = node->next;
	}

	friend std::ostream& operator<<(std::ostream &out, const Node_Contents &obj) {

		out << "@" << obj.address << " .data " << obj.data << " .next " << obj.next;
		return out;
	}

	const WrUIntNode   *address;
	WrUInt				data;
	const WrUIntNode   *next;
};

class Set_Contents {
public:
	Set_Contents() {
		size = 0;
		set_address = nullptr;
		list_address = nullptr;
		head = nullptr ;
		list_of_nodes = nullptr;
	}
	Set_Contents(const Set_Contents &other) {
		size = other.size;
		set_address = other.set_address;
		list_address = other.list_address;
		head = other.head;
		list_of_nodes = nullptr;
		if (other.size != 0) {
			list_of_nodes = new Node_Contents*[size];
			Node_Contents **dst = list_of_nodes;
			Node_Contents *src = *other.list_of_nodes;
			// deep copy the other's list of nodes
			for (int i = 0; i != size; i++) {
				*dst++ = new Node_Contents(*src++);
			}
		}
	}
	Set_Contents(const WrUIntSet &set) {

		size = set.size();
		set_address = &set;
		list_address = &set.list;
		head = set.list.head;
		list_of_nodes = nullptr;
		if (size) {
			list_of_nodes = new Node_Contents*[size];
			Node_Contents **dst = list_of_nodes;
			*dst++ = new Node_Contents(set.list.head);
			WrUIntNode *src = set.list.head;
			while (src->next) {
				// deep copy the existing node because
				//	it may get recycled during the operation
				*dst = new Node_Contents(src->next);
				dst++;
				src = src->next;
			}
		}
	}
	~Set_Contents() {

		set_address = nullptr;
		list_address = nullptr;
		head = nullptr;
		if (list_of_nodes) {
			Node_Contents **pnode = list_of_nodes;
			for (int i = 0; i != size; i++) {
				if (*pnode) {
					delete *pnode;
				}
				pnode++;
			}
			delete list_of_nodes;
			size = 0;
		}
	}

	friend std::ostream& operator<<(std::ostream &out, const Set_Contents &obj) {

		out << "set was at " << obj.set_address << " and contained " << obj.size << " objects " << endl
			<< " list was at address " << obj.list_address << " head: " << obj.head << endl
			<< " an array of nodes is stored at " << obj.list_of_nodes << ": " << endl;
		for (int i = 0; i != obj.size; i++) {
			out << " [" << i << "] = " << obj.list_of_nodes[i]
				<< ": " << *obj.list_of_nodes[i] << endl; //obj.list_of_nodes[i])->data << " -> " << (obj.list_of_nodes[i])->next << endl;
		}

		return out;
	}

	int size;
	const WrUIntSet  *set_address;
	const WrUIntList *list_address;
	const WrUIntNode *head;
	Node_Contents **list_of_nodes;
};

const WrUIntNode *findNode(const WrUIntSet&, const WrUInt value_to_find);
bool setElementsAreStoredAtSameLocation(const Set_Contents* contents_before, const WrUIntSet& after_set, Message_Level);
bool setElementsAreNotStoredAtSameLocation(const Set_Contents* contents_before, const WrUIntSet& after_set, Message_Level);
bool setsStorageLocationsDontCare(const Set_Contents* contents_before, const WrUIntSet& after_Set, Message_Level);
bool setContentsAreEqual(const WrUIntSet&, const WrUIntSet&);

typedef bool (*Set_Relation_Function)(const Set_Contents* contents_before, const WrUIntSet& after_set, Message_Level);

class Test_Arguments {
public:
	Test_Arguments() {
		in_a = in_b = exp = nullptr;
		in_a_sz = in_b_sz = exp_sz = 0;
		op = SET_NOP;
		rel_tst = nullptr;
		msg_lvl = None;
	}
	Test_Arguments(	unsigned *_in_a, int _in_a_sz,
					unsigned *_in_b, int _in_b_sz,
					unsigned *_exp, int _exp_sz,
					Set_Operations _op,
					Set_Relation_Function _rel_tst,
					Message_Level _msg_lvl) :
						in_a(_in_a), in_a_sz(_in_a_sz),
						in_b(_in_b), in_b_sz(_in_b_sz),
						exp(_exp), exp_sz(_exp_sz),
						op(_op), rel_tst(_rel_tst),
						msg_lvl(_msg_lvl) { }

	unsigned 	*in_a; 	int in_a_sz;
	unsigned	*in_b;	int in_b_sz;
	unsigned 	*exp; 	int exp_sz;
	Set_Operations op;
	Set_Relation_Function rel_tst;
	Message_Level msg_lvl;
};

#pragma push_macro("NUM_TEST_RESULT_MESSAGES")

#define NUM_TEST_RESULT_MESSAGES 5

#undef TEST_RESULT_STRING_POSITION
#undef TEST_RESULT_STRING_POSITIONS

#define TEST_RESULT_STRING_POSITIONS\
	TEST_RESULT_STRING_POSITION(Result_Index = 0, 			"Test result:                  ")\
	TEST_RESULT_STRING_POSITION(Operator_Index, 			"Operator tested:              ")\
	TEST_RESULT_STRING_POSITION(Values_Verification_Index, 	"Values Verification Outcome:  ")\
	TEST_RESULT_STRING_POSITION(Storage_Verification_Index, "Storage Verification Outcome: ")\
	TEST_RESULT_STRING_POSITION(Info_Index, 				"Info:                         ")\
	TEST_RESULT_STRING_POSITION(nullptr_Index, 				"** do not use **nullptr_index:")

#define TEST_RESULT_STRING_POSITION(enm, str) enm,
enum Test_Result_String_Positions { TEST_RESULT_STRING_POSITIONS };
#undef TEST_RESULT_STRING_POSITION

#define TEST_RESULT_STRING_POSITION(enm, str) str,
const char *test_result_position_strings[] = { TEST_RESULT_STRING_POSITIONS "" };

#undef TEST_RESULT_STRING_POSITION
#undef TEST_RESULT_STRING_POSITIONS

class Test_Results {
public:
	void set_strings( const char* result_str ,
					  const char* op_str,
					  const char *verification_str,
					  const char *relation_str,
					  const char* info_str) {
		if (result_str != nullptr) 			messages[Result_Index] = result_str;
		if (op_str != nullptr)				messages[Operator_Index] = op_str;
		if (verification_str != nullptr)	messages[Values_Verification_Index] = verification_str;
		if (relation_str != nullptr)		messages[Storage_Verification_Index] = relation_str;
		if (info_str != nullptr) 			messages[Info_Index] = info_str;
		messages[nullptr_Index] = nullptr;
	}

	Test_Results(bool _outcome, char **_messages) {
		outcome = _outcome;
		for (int i = 0; i!= nullptr_Index; i++) {
			messages[i] = _messages[i];
		}
		messages[nullptr_Index] = nullptr;
	}
	Test_Results() {
		outcome = false;
		for (int i = 0; i < nullptr_Index; i++) {
			messages[i] = "";
		}
		messages[nullptr_Index] = nullptr;
	}

	friend std::ostream& operator<<(std::ostream& out, const Test_Results &results) {
		out << "test results: " << (results.outcome == passed ? "passed " : "failed ") << endl;
		out << "outcome: " << results.messages[Result_Index] << endl;
		for (int i = 0; i != nullptr_Index; i++) {
			if (results.messages != nullptr) {
				out << test_result_position_strings[i];
				if (i == Operator_Index) {
					cout << "\" " << results.messages[i] << " \"" << endl;
				} else {
					cout << results.messages[i] << endl;
				}
			}
		}
		return out;
	}

	bool outcome;
	const char *messages[NUM_TEST_RESULT_MESSAGES+1];		// last object has to be nullptr
};

#pragma push_macro("delete_object")
#pragma push_macro("delete_array")
#pragma push_macro("delete_args")
#pragma push_macro("delete_before")
#pragma push_macro("delete_results")

#define delete_object(ptr)\
	do {\
		if(ptr) {\
			delete (ptr);\
			(ptr) = nullptr;\
		}\
	} while(false)

#define delete_array(ptr)\
	do {\
		if (ptr) {\
			delete[] (ptr);\
			(ptr) = nullptr;\
		}\
	} while (false)

#define delete_args(ptr)	delete_object((ptr))
#define	delete_before(ptr)	delete_object((ptr))
#define delete_results(ptr)	delete_object((ptr))

bool announceResults(int passed_test_count, int test_count);
void buildSet(WrUIntSet& dst, unsigned *values, int num_values, Message_Level message_level);
bool isElement(unsigned *p, int sz, unsigned val);
void echoSet(const char* before, WrUIntSet& set, const char *after, Message_Level message_level);
void echoSetUnsignedOperationResult(WrUIntSet &set, Set_Operations op, unsigned operand, Message_Level message_level);
Test_Results* runTest(Test_Arguments*);
Test_Results* runUnsignedOperationTest(Test_Arguments *pa);
Test_Results* runSetArithmeticOperationTest(Test_Arguments *pa);
Test_Results* runSetAssignmentOperationTest(Test_Arguments *pa);
Test_Results* runSetRelationalOperatorsTest(Message_Level message_level, int& test_count, int& passed_test_count);

bool verifySetResults(	const char *before,
						WrUIntSet& set_to_verify,
						unsigned *expected, int expected_count,
						char *after, Message_Level level);


/* ********************************************************************	*/
/* ********************************************************************	*/
/*					test class WrappedUnsignedSet						*/
/* ********************************************************************	*/
/* ********************************************************************	*/

#define TEST_SET_BUILDING_SET
#define TEST_SET_CLEAR_RESET
#define TEST_SET_COPY_CONSTRUCTOR_AND_ASSIGNMENT
#define TEST_SET_OPERATOR_ADD_SUB_UNSIGNED
#define TEST_SET_IS_MEMBER_UNSIGNED
#define TEST_SET_RELATIONAL_OPERATORS
#define TEST_SET_OPERATOR_ARITHMETIC_SET
#define TEST_SET_OPERATOR_ARITHMETIC_ASSIGN_SET

bool testWrappedUnsignedSet() {

#if defined(TEST_SET_OPERATOR_ADD_SUB_UNSIGNED)\
 or	defined(TEST_SET_BUILDING_SET)\
 or defined(TEST_SET_IS_MEMBER_UNSIGNED)

	unsigned stressing_inputs[] = { 5, 3, 7, 4, 6, 8, 2, 1 };
	unsigned stressing_result[]	= { 1, 2, 3, 4, 5, 6, 7, 8 };
	int stressing_inputs_sz = sizeof(stressing_inputs) / sizeof(unsigned);
	int stressing_result_sz = sizeof(stressing_result) / sizeof(unsigned);

	unsigned repeating_inputs[]		= { 1, 2, 3, 4, 4, 4, 3, 1 };
	unsigned repeating_result[]		= { 1, 2, 3, 4 };
	int repeating_inputs_sz = sizeof(repeating_inputs) / sizeof(unsigned);
	int repeating_result_sz = sizeof(repeating_result) / sizeof(unsigned);
#endif

	unsigned* in_a = nullptr;
	int in_a_sz = 0;
	unsigned* in_b = nullptr;
	int in_b_sz = 0;
	unsigned* xpctd = nullptr;
	int xpctd_sz = 0;

	Message_Level message_level = Verbose;
	int test_count = 0;
	int passed_test_count = 0;
	Set_Operations op = SET_NOP;
	Test_Arguments *test_arguments = nullptr;
	Test_Results *test_results = nullptr;
	Set_Contents *contents_before = nullptr;

    cout << endl;

    echoTestName("Default Constructor");
    test_count++;
    WrUIntSet set;

    if (set.size() != 0 || set.list.head != nullptr) {
    	cout << abort_str << "default constructor did not produce empty set &" << &set << ": " << set << endl;
    	return announceResults(passed_test_count, test_count);
    } else {
    	cout << passed_str << "default constructor produced empty set &" << &set << ": " << set << endl;
    	passed_test_count++;
    }
    cout_count();

#ifdef	TEST_SET_BUILDING_SET

    // test the operator that 'buildSet' uses

	// this test has to be very verbose b/c no other test will work until this one passes
    echoTestName("operator+=(unsigned)");
    test_count++;
    contents_before = new Set_Contents(set);
    in_a = stressing_inputs;
    for (int i = 0; i != stressing_inputs_sz; i++) {
    	set += in_a[i];
    	if (isMsgLvlVerbose(message_level)) {
    		cout 	<< " .. added " << stressing_inputs[i] << " to set produces @"
    				<< &set << ": " << endl
					<< set << endl << endl;
    	}
    }

    if (!verifySetResults("Test adding stressing inputs\n", set, stressing_result, stressing_result_sz, newline, message_level)) {
    	cout << abort_str << " UNABLE TO CONTINUE TEST" << endl;
    	delete_before(contents_before);
    	return announceResults(passed_test_count, test_count);
    }
    cout << endl;

    // assignment operator should not produce a copy
    if (!setElementsAreStoredAtSameLocation(contents_before, set, message_level)) {
    	cout << "using += assignment operator to add elements to an existing set created a copy set" << endl;
    	cout << abort_str << " UNABLE TO CONTINUE TEST" << endl;
    	delete_before(contents_before);
    	return announceResults(passed_test_count, test_count);
    }

    delete_before(contents_before);
   	passed_test_count++;
    cout_count();

    //	test building the set

    echoTestName("buildSet()");
    test_count++;
    buildSet(set, repeating_inputs, repeating_inputs_sz, None);

    if (!verifySetResults("Test buildSet(stressing_inputs)\n", set, repeating_inputs, repeating_result_sz, newline, message_level)) {
    	cout << abort_str << " UNABLE TO CONTINUE TEST" << endl;
    	return announceResults(passed_test_count, test_count);
    }

   	passed_test_count++;
    cout_count();
#endif

    // at this point, buildSet can be called
#ifdef TEST_SET_CLEAR_RESET
    // this test has to pass, also, b/c each test needs to start witl .clear()
    echoTestName("set.clear()");
    test_count++;
    buildSet(set, stressing_inputs, stressing_inputs_sz, message_level);
    set.clear();
    if (set.size() != 0) {
    	cout << error_str << "set.clear() returns " << set.size() << " != 0 " << " set is @" << &set << ": " << endl << set << endl << endl;
    	cout << abort_str << " UNABLE TO CONTINUE TEST" << endl;
    	return announceResults(passed_test_count, test_count);
    } else {
    	passed_test_count++;
    	if (isMsgLvlVerbose(message_level)) {
    		cout << passed_str << "after set.clear()" << "set is @" << &set << ": " << endl << set << endl << endl;
    	}
    }
    cout_count();

    echoTestName("set.reset()"); test_count++;
    buildSet(set, stressing_inputs, stressing_inputs_sz, message_level);
    set.reset();
    if (set.size() != 0) {
    	if (isMsgLvlResults(message_level)) {
    		cout << error_str << "set.reset() returns " << set.size() << " != 0 " << " set is @" << &set << ": " << endl << set << endl << endl;
    	}
    } else {
    	passed_test_count++;
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << "after set.reset()," << endl << "set  @" << &set << ": " << endl << set << endl << endl;
    	}
    }
    cout_count();
#endif

#ifdef TEST_SET_COPY_CONSTRUCTOR_AND_ASSIGNMENT
    echoTestName("copy constructor");
    test_count++;
    buildSet(set, stressing_inputs, stressing_inputs_sz, None);
    contents_before = new Set_Contents(set);
    WrUIntSet copy_set(set);
    if (setElementsAreStoredAtSameLocation(contents_before, copy_set, None)) {
    	cout << error_str << " copy constructor did not copy set:" << endl
    		 << " original set was stored " << *contents_before << endl << endl
			 << "     copy set is stored &" << &copy_set << " " << copy_set << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout 	<< passed_str << ": " << endl;
    	}
    	if (isMsgLvlVerbose(message_level)) {
			cout 	<< "set      @" << &set << " :" << endl
					<< set << endl << endl;
			cout 	<< "copy_set @" << &copy_set << " :" << endl
					<< copy_set << endl << endl;
			}
    	passed_test_count++;
    }
    delete_before(contents_before);
    cout_count();


    echoTestName("operator=(WrUIntSet");
    test_count++;
    buildSet(set, stressing_inputs, stressing_inputs_sz, None);
    contents_before = new Set_Contents(set);
    WrUIntSet assigned_set;
    assigned_set = set;
    if (setElementsAreStoredAtSameLocation(contents_before, assigned_set, None)) {
    	cout << error_str << " assignment operator=(WrUIntSet&) did not copy set:" << endl
    		 << " original set was stored " << *contents_before << endl << endl
			 << " assigned set is stored &" << &assigned_set << " " << assigned_set << endl;
    } else {
    	if (isMsgLvlVerbose(message_level)) {
			cout 	<< "set          @" << &set << " :" << endl
					<< set << endl << endl;
			cout 	<< "assigned_set @" << &assigned_set << " :" << endl
					<< copy_set << endl << endl;
			}
    	if (isMsgLvlResults(message_level)) {
    		cout 	<< passed_str << endl;
    	}
    	passed_test_count++;
    }
    delete_before(contents_before);
    cout_count();
#endif

#ifdef TEST_SET_OPERATOR_ADD_SUB_UNSIGNED

    echoTestName("operator+(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { };
    in_a_sz = 0;
    in_b = stressing_inputs;
    in_b_sz = stressing_inputs_sz;
    xpctd = stressing_result;
    xpctd_sz = stressing_result_sz;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_UNSIGNED_ADD, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_UNSIGNED_ADD] << endl;
    	}
    	passed_test_count++;
    }
    delete_array(in_a);
    delete_args(test_arguments);
    delete_results(test_results);

    echoTestName("operator+(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { };
    in_a_sz = 0;
    in_b = repeating_inputs;
    in_b_sz = repeating_inputs_sz;
    xpctd = repeating_result;
    xpctd_sz = repeating_result_sz;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_UNSIGNED_ADD, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_UNSIGNED_ADD] << endl;
    	}
    	passed_test_count++;
    }
    delete_array(in_a);
    delete_args(test_arguments);
    delete_results(test_results);
    cout_count();

    echoTestName("operator-(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    in_a_sz = 8;
    in_b = new unsigned[] { 1, 2,    4, 5,    7, 8 };
    in_b_sz = 6;
    xpctd = new unsigned[] { 3, 6 };
    xpctd_sz = 2;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_UNSIGNED_SUB, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_UNSIGNED_SUB] << endl;
    	}
    	passed_test_count++;
    }
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    delete_args(test_arguments);
    delete_results(test_results);
    cout_count();

    echoTestName("operator+=(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { };
    in_a_sz = 0;
    in_b = stressing_inputs;
    in_b_sz = stressing_inputs_sz;
    xpctd = stressing_result;
    xpctd_sz = stressing_result_sz;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_ASSIGN_UNSIGNED_ADD, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_ASSIGN_UNSIGNED_ADD] << endl;
    	}
    	passed_test_count++;
    }
    delete_array(in_a);
    delete_args(test_arguments);
    delete_results(test_results);
    cout_count();

    echoTestName("operator+=(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { };
    in_a_sz = 0;
    in_b = repeating_inputs;
    in_b_sz = repeating_inputs_sz;
    xpctd = repeating_result;
    xpctd_sz = repeating_result_sz;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_ASSIGN_UNSIGNED_ADD, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_ASSIGN_UNSIGNED_ADD] << endl;
    	}
    	passed_test_count++;
    }
    delete_array(in_a);
    delete_args(test_arguments);
    delete_results(test_results);
    cout_count();

    echoTestName("operator-=(unsigned)");
    set.clear();
    test_count++;

    in_a = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    in_a_sz = 8;
    in_b = new unsigned[] { 1, 2,    4, 5,    7, 8 };
    in_b_sz = 6;
    xpctd = new unsigned[] { 3, 6 };
    xpctd_sz = 2;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										SET_ASSIGN_UNSIGNED_SUB, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[SET_ASSIGN_UNSIGNED_SUB] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();
#endif

#ifdef TEST_SET_IS_MEMBER_UNSIGNED
    echoTestName(".isMember(unsigned)");
    in_a = new unsigned[] { 1, 2, 3, 4 };
    in_a_sz = 4;
    in_b = new unsigned[] { 1, 2, 3, 4, 5, 6 };
    in_b_sz = 6;
    bool is_member_error = false;
    buildSet(set, in_a, in_a_sz, None);
    cout << endl;
    test_count++;
    for (int i = 0; i != in_b_sz; i++) {
    	if (isElement(in_a, in_a_sz, in_b[i])) {
    		if (set.isMember(in_b[i])) {
    			cout  << ".isMember(" << in_b[i] << ") returns true on " << set.valuesString() << endl;
    		} else {
    			cout  << error_str << ".isMember(" << in_b[i]
					  << ") returns true on " << set.valuesString() << endl;
    			is_member_error = true;
    		}
    	} else {
    		if (!set.isMember(in_b[i])) {
    			cout  << ".isMember(" << in_b[i] << ") returns false on " << set.valuesString() << endl;
    		} else {
    			cout  << error_str << ".isMember(" << in_b[i]
					  << ") returns true on " << set.valuesString() << endl;
    			is_member_error = true;
    		}
    	}
    }

    if (is_member_error) {
    	cout << error_str << " test .isMember() failed" << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " test .isMember() passed" << endl;
    	}
    	passed_test_count++;
    }

    delete_array(in_a);
    delete_array(in_b);
    cout_count();

#endif

#ifdef TEST_SET_RELATIONAL_OPERATORS

    test_results = runSetRelationalOperatorsTest(message_level, test_count, passed_test_count);

    if (test_results->outcome == failed) {
    	cout << error_str << test_results->messages[Result_Index]
			 << ": " << test_results->messages[Operator_Index]
			 << " " << test_results->messages[Values_Verification_Index]
    		 << " " << test_results->messages[Storage_Verification_Index]
    		 << endl;
    } else {
    	cout << passed_str << ": relational operators == !=" << endl;
    }

    delete_results(test_results);
    cout_count();
#endif

#ifdef TEST_SET_OPERATOR_ARITHMETIC_SET

     // these tests should create copies

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_UNION_PLUS						*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator+(WrUIntSet)");
    echoTestPhase("operator+(WrUIntSet) { 1-3, 5-8 } + { empty }");
    op = SET_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator+(WrUIntSet) { empty } + { 1-3, 5-8 }");
    op = SET_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();


    echoTestPhase("operator+(WrUIntSet) { 1-3, 5-8 } + { 3-5 }");
    op = SET_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    xpctd_sz = 8;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_UNION_OR						*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator|(WrUIntSet)");
    echoTestPhase("operator|(WrUIntSet) { 1-3, 5-8 } | { empty }");
    op = SET_UNION_OR;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator|(WrUIntSet) { empty } | { 1-3, 5-8 }");
    op = SET_UNION_OR;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator|(WrUIntSet) { 1-3, 5-8 } | { 3-5 }");
    op = SET_UNION_OR;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    xpctd_sz = 8;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_UNIQUE							*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator^(WrUIntSet)");
    echoTestPhase("operator^(WrUIntSet) { 1-3, 5-8 } ^ { empty }");
    op = SET_UNIQUE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator^(WrUIntSet) { empty } ^ { 1-3, 5-8 }");
    op = SET_UNIQUE;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator^(WrUIntSet) { 1-3, 5-8 } ^ { 3-5 }");
    op = SET_UNIQUE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 4, 6, 7, 8 };
    xpctd_sz = 6;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_INTERSECTION					*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator&(WrUIntSet)");
    echoTestPhase("operator&(WrUIntSet) { 1-3, 5-8 } & { empty }");
    op = SET_INTERSECTION;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator&(WrUIntSet) { empty } & { 1-3, 5-8 }");
    op = SET_INTERSECTION;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator&(WrUIntSet) { 1-3, 5-8 } & { 3-5 }");
    op = SET_INTERSECTION;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 3, 5 };
    xpctd_sz = 2;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_DIFFERENCE						*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator-(WrUIntSet)");
    echoTestPhase("operator-(WrUIntSet) { 1-3, 5-8 } - { empty }");
    op = SET_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,    5, 6, 7 , 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator-(WrUIntSet) { empty } - { 1-3, 5-8 }");
    op = SET_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator-(WrUIntSet) { 1-3, 5-8 } - { 3-5 }");
    op = SET_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 6, 7 , 8 };
    xpctd_sz = 5;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreNotStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();
#endif

#ifdef TEST_SET_OPERATOR_ARITHMETIC_ASSIGN_SET

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_ASSIGN_UNION_PLUS				*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator+=(WrUIntSet)");
    echoTestPhase("operator+=(WrUIntSet) { 1-3, 5-8 } + { empty }");
    op = SET_ASSIGN_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator+=(WrUIntSet) { empty } + { 1-3, 5-8 }");
    op = SET_ASSIGN_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();


    echoTestPhase("operator+=(WrUIntSet) { 1-3, 5-8 } + { 3-5 }");
    op = SET_ASSIGN_UNION_PLUS;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    xpctd_sz = 8;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_ASSIGN_UNION_OR						*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator|=(WrUIntSet)");
    echoTestPhase("operator|=(WrUIntSet) { 1-3, 5-8 } | { empty }");
    op = SET_ASSIGN_UNION_OR;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator|=(WrUIntSet) { empty } | { 1-3, 5-8 }");
    op = SET_ASSIGN_UNION_OR;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator|=(WrUIntSet) { 1-3, 5-8 } | { 3-5 }");
    op = SET_ASSIGN_UNION_OR;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    xpctd_sz = 8;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    return announceResults(passed_test_count, test_count);
    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_ASSIGN_UNIQUE							*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator^=(WrUIntSet)");
    echoTestPhase("operator^=(WrUIntSet) { 1-3, 5-8 } ^ { empty }");
    op = SET_ASSIGN_UNIQUE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator^=(WrUIntSet) { empty } ^ { 1-3, 5-8 }");
    op = SET_ASSIGN_UNIQUE;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] { 1, 2, 3,   5, 6, 7, 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator^=(WrUIntSet) { 1-3, 5-8 } ^ { 3-5 }");
    op = SET_ASSIGN_UNIQUE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 4, 6, 7, 8 };
    xpctd_sz = 6;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_ASSIGN_INTERSECTION					*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator&=(WrUIntSet)");
    echoTestPhase("operator&=(WrUIntSet) { 1-3, 5-8 } & { empty }");
    op = SET_ASSIGN_INTERSECTION;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator&=(WrUIntSet) { empty } & { 1-3, 5-8 }");
    op = SET_ASSIGN_INTERSECTION;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator&=(WrUIntSet) { 1-3, 5-8 } & { 3-5 }");
    op = SET_ASSIGN_INTERSECTION;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 3, 5 };
    xpctd_sz = 2;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    /* ****************************************************	*/
    /* ****************************************************	*/
    /*					SET_ASSIGN_DIFFERENCE						*/
    /* ****************************************************	*/
    /* ****************************************************	*/

    echoTestName("operator-=(WrUIntSet)");
    echoTestPhase("operator-=(WrUIntSet) { 1-3, 5-8 } - { empty }");
    op = SET_ASSIGN_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {};
    in_b_sz = 0;
    xpctd = new unsigned[] { 1, 2, 3,    5, 6, 7 , 8 };
    xpctd_sz = 7;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator-=(WrUIntSet) { empty } - { 1-3, 5-8 }");
    op = SET_ASSIGN_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] {};
    in_a_sz = 0;
    in_b = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_b_sz = 7;
    xpctd = new unsigned[] {};
    xpctd_sz = 0;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

    echoTestPhase("operator-=(WrUIntSet) { 1-3, 5-8 } - { 3-5 }");
    op = SET_ASSIGN_DIFFERENCE;
    test_count++;

    in_a = new unsigned[] { 1, 2, 3,    5, 6, 7, 8 };
    in_a_sz = 7;
    in_b = new unsigned[] {       3, 4, 5  };
    in_b_sz = 3;
    xpctd = new unsigned[] { 1, 2, 6, 7 , 8 };
    xpctd_sz = 5;

    test_arguments = new Test_Arguments(in_a, in_a_sz, in_b, in_b_sz, xpctd, xpctd_sz,
										op, setElementsAreStoredAtSameLocation, Verbose);

    test_results = runTest(test_arguments);
    if (!test_results->outcome) {
    	cout << *test_results << endl;
    } else {
    	if (isMsgLvlResults(message_level)) {
    		cout << passed_str << " " << " operation " << s_op_str[op] << endl;
    	}
    	passed_test_count++;
    }
    delete_args(test_arguments);
    delete_results(test_results);
    delete_array(in_a);
    delete_array(in_b);
    delete_array(xpctd);
    cout_count();

#endif

    //#define TEST_SET_OPERATOR_ARITHMETIC_ASSIGN_SET
    cout << "testWrappedUnsignedSet(): ";
    return announceResults(passed_test_count, test_count);
}


    /* ****************************************************************	*/
    /* ****************************************************************	*/
    /*							utilities								*/
    /* ****************************************************************	*/
    /* ****************************************************************	*/

bool announceResults(int passed_test_count, int test_count) {

	cout << "Passed " << passed_test_count << " of " << test_count << " tests" << endl;
	return passed_test_count == test_count;
}

void buildSet(WrUIntSet& dst, unsigned *values, int num_values, Message_Level message_level) {

	dst.clear();

	for (int i = 0; i != num_values; i++) {
		dst += values[i];
	}

	if(isMsgLvlVerbose(message_level))
		cout << "buildSet @" << &dst << endl << dst << endl;
}

bool isElement(unsigned *p, int sz, unsigned val) {
	for (int i = 0; i != sz; i++) {
		if (p[i] == val) return true;
	}
	return false;
}


	/* ****************************************************************	*/
	/*	determine if one set is the result of an assignment operator	*/
	/* ****************************************************************	*/

const WrUIntNode *findNode(const WrUIntSet& set, const WrUInt value_to_find) {

	WrUIntNode* src = set.list.head;
	while (src) {
		if (src->data == value_to_find) {
			return src;
		}
		src = src->next;
	}

	return nullptr;

}

//	determine if the storage location of the two sets are the same:
//	  set address is identical before & after
//	  set list is stored at the same address
//	  any values that are in both sets are stored in the same nodes
bool setElementsAreStoredAtSameLocation(const Set_Contents* before_set, const WrUIntSet& after_set, Message_Level message_level) {

	if (isMsgLvlVerbose(message_level)) {
		cout << __FUNCTION__ << endl;
	}
	// trap an error by the caller
	if (before_set == nullptr) {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns false b/c before_set == nullptr" << endl << endl;
		}
		return false;
	}

	if (isMsgLvlVerbose(message_level)) {
		cout << "before_set: " << endl << *before_set << endl;
		cout << "after_set:  " << endl << after_set << endl << endl;
	}

	// the sets are both stored at the same address
	//   and the list is stored at the same location
	// the lists may not have the same contents,
	//	for example in the operation seta &= setb
	// however, any values that are in the after_set that were also
	//	in the before_set, must be stored at the same address

	// if the before set was empty, the list of nodes will be nullptr
	if (before_set->size == 0 || before_set->list_of_nodes == nullptr)  {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns true b/c before_set was empty" << endl << endl;
		}
		return true;
	}


	for (int i = 0; i != before_set->size; i++) {
		// attempt to find values the after set that were in before set
		const WrUIntNode *after_node = findNode(after_set, before_set->list_of_nodes[i]->data);
		// if the value that was in the before set is in the after set
		if (after_node != nullptr) {
			// if the node containing the value is different that it was before
			if (after_node != before_set->list_of_nodes[i]->address) {
				if (isMsgLvlVerbose(message_level)) {
					cout << " returns false b/c value in before_set is not in same node in after_set" << endl << endl;
				}
				return false;
			}
		}
	}

	if (isMsgLvlVerbose(message_level)) {
		cout << " returns true " << endl << endl;
	}
	return true;
}


/* ************************************************************	*/
/*	determine if the list nodes are identical in each set		*/
/* ************************************************************	*/

bool setElementsAreNotStoredAtSameLocation(const Set_Contents* before_set,
									const WrUIntSet& after_set,
									Message_Level message_level) {

	if (isMsgLvlVerbose(message_level)) {
		cout << __FUNCTION__ << endl;
	}
	// trap an error by the caller
	if (before_set == nullptr) {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns false b/c before_set == nullptr" << endl << endl;
		}
		return false;
	}

	if (isMsgLvlVerbose(message_level)) {
		cout << "before_set: " << endl << *before_set << endl;
		cout << "after_set:  @" << &after_set << endl << after_set << endl << endl;
	}

	// if the before set was empty, the list of nodes will be nullptr
	if (before_set->size == 0 || before_set->list_of_nodes == nullptr)  {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns true b/c before_set was empty" << endl << endl;
		}
		return true;
	}

	// if after->head is now null, nothing to check
	if (after_set.list.head == nullptr) {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns true b/c after_set.list.head == nullptr" << endl << endl;
		}
		return true;
	}

	// before_set->size != 0, so there is at least one node stored in 'list_of_nodes[]'
	bool sets_have_at_least_one_value_in_common = false;
	// if every value that the two sets have in common is at the same node address, not a copy
	for (int i = 0; i != before_set->size; i++)
	{
		// determine if a value in the before_set is in the after_set
		const WrUIntNode *after_node = findNode(after_set, before_set->list_of_nodes[i]->data);
		// if a value in the before set was found in the after set
		if (after_node != nullptr)
		{
			sets_have_at_least_one_value_in_common = true;
			if (after_node != before_set->list_of_nodes[i]->address) {
				// the sets share at least one value, and that value
				// 	is not stored at the same memory address in each set
				//	(they have at least one different node address)
				if (isMsgLvlVerbose(message_level)) {
					cout << " returns true b/c a value in before_set "
						 << "was found in after_set "
						 << "but at different node address"
						 << endl << endl;
				}
				return true;
			}
			// it is possible that a value in the before set was copied
			//  into the after set, but the allocator re-used (recycled)
			//	the same memory location when building the after_set
			// thus, at this point the for loop needs to check the next node
			//	until all nodes in the before_set have been checked
		}
	}

	// if there was at least one value in common, but there was
	//	no exit from the function during the for() loop due to
	//	a common value being stored at different node address before & after
	//  then all values (of which there was at least one)
	//	that are common to both sets are stored in nodes at the same location
	if (sets_have_at_least_one_value_in_common) {
		if (isMsgLvlVerbose(message_level)) {
			cout << " returns false because "
				 << " all of the values that are in both sets "
				 << " are stored at the same nodes" << endl << endl;
		}
		return false;
	}

	if (isMsgLvlVerbose(message_level)) {
		cout << " returns true b/c there were no values "
			 << "that were common to each set" << endl << endl;
	}

	return true;
}

bool setsStorageLocationsDontCare(const Set_Contents* contents_before __attribute__((unused)), const WrUIntSet& after_Set __attribute__((unused)), Message_Level message_level) {
	if (isMsgLvlVerbose(message_level)) {
		cout << __FUNCTION__ << " returns true " << endl;
	}
	return true;
}

bool setContentsAreEqual(const WrUIntSet &seta, const WrUIntSet &setb) {

	if (seta.size() != setb.size())
		return false;

	WrUIntNode *pa = seta.list.head;
	WrUIntNode *pb = setb.list.head;

	while (pa != nullptr && pb != nullptr) {

		if (pa->data != pb->data) {
			return false;
		}
		pa = pa->next;
		pb = pb->next;
	}

	// if one list terminated before the other,
	// which should never happen
	if (pa != nullptr || pb != nullptr)
		return false;

	return true;
}


	/* ****************************************************************	*/
	/*							running tests							*/
	/* ****************************************************************	*/


Test_Results* runSetRelationalOperatorsTest(Message_Level message_level, int& test_count, int& passed_test_count)
{
    echoTestName("relational operators");

    Test_Results *retval = new Test_Results;
    retval->outcome = passed;
    retval->set_strings("relational operators", passed_str, empty_str, empty_str, empty_str);

    const char* relational_operator_test_failed_str = "relational operator test failed";
    const char* empty_set_str = "{ empty }";
    const char* seta_str = "{ 1, 2, 3, 4 }";
    const char* setb_str = "{ 5, 6, 7, 8 }";
    const char* returns_false_str = " returns false";
    const char* returns_true_str = " returns false";

    WrUIntSet seta;
    WrUIntSet setb;

    unsigned unsigneds_1_4[] = { 1, 2, 3, 4 };
    unsigned unsigneds_5_8[] = { 5, 6, 7, 8 };
    int num_unsigneds_1_4 = sizeof(unsigneds_1_4)/sizeof(unsigned);
    int num_unsigneds_5_8 = sizeof(unsigneds_5_8)/sizeof(unsigned);

    echoTestPhase("{ empty }.operator==({ empty })");
    test_count++;
    if (!(seta == setb)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " == " << setb.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						empty_set_str,
							set_operation_strings[SET_EQUAL],
							empty_set_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " == " << setb.valuesString() << " returns true " << endl;
    }
    cout << endl;

    echoTestPhase("{ empty }.operator!=({ empty })");
    test_count++;
    if (seta != setb) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " != " << setb.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						empty_set_str,
							set_operation_strings[SET_EQUAL],
							empty_set_str,
							returns_true_str);
    } else {
    	cout << "      " << seta.valuesString() << " != " << setb.valuesString() << " returns false" << endl;
    	passed_test_count++;
    }

    echoTestPhase("{ 1-4 }.operator==({ empty })");
    buildSet(seta, unsigneds_1_4, num_unsigneds_1_4, None);
    test_count++;
    if (seta == setb) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " == " << setb.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							empty_set_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " == " << setb.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ 1-4 }.operator!=({ empty })");
    test_count++;
    if (!(seta != setb)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " != " << setb.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							empty_set_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " != " << setb.valuesString() << " returns true " << endl;
    }

    echoTestPhase("{ empty }.operator==({ 1-4 })");
    test_count++;
    if (setb == seta) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " == " << seta.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						empty_set_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " == " << seta.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ empty }.operator!=({ 1-4 })");
    test_count++;
    if (!(setb != seta)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " != " << seta.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						empty_set_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " != " << seta.valuesString() << " returns true " << endl;
    }

    echoTestPhase("{ 1-4 }.operator==({ 1-4 })");
    buildSet(setb, unsigneds_1_4, num_unsigneds_1_4, None);
    test_count++;
    if (!(seta == setb)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " == " << setb.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " == " << setb.valuesString() << " returns true " << endl;
    }

    echoTestPhase("{ 1-4 }.operator!=({ 1-4 })");
    test_count++;
    if (seta != setb) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " != " << setb.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " != " << setb.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ 1-4 }.operator==({ 1-4 })");
    test_count++;
    if (!(setb == seta)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " == " << seta.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " == " << seta.valuesString() << " returns true " << endl;
    }

    echoTestPhase("{ 1-4 }.operator!=({ 1-4 })");
    test_count++;
    if (setb != seta) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " != " << seta.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " != " << seta.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ 1-4 }.operator==({ 5-8 })");
    buildSet(setb, unsigneds_5_8, num_unsigneds_5_8, None);
    test_count++;
    if (seta == setb) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " == " << setb.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							setb_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " == " << setb.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ 1-4 }.operator!=({ 5-8 })");
    test_count++;
    if (!(seta != setb)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << seta.valuesString() << " != " << setb.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						seta_str,
							set_operation_strings[SET_EQUAL],
							setb_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << seta.valuesString() << " != " << setb.valuesString() << " returns true " << endl;
    }

    echoTestPhase("{ 5-8 }.operator==({ 1-4 })");
    test_count++;
    if (setb == seta) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " == " << seta.valuesString() << " returns true " << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						setb_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_true_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " == " << seta.valuesString() << " returns false" << endl;
    }

    echoTestPhase("{ 5-8 }.operator!=({ 1-4 })");
    test_count++;
    if (!(setb != seta)) {
    	if (isMsgLvlVerbose(message_level)) {
    		cout << "ERROR " << setb.valuesString() << " != " << seta.valuesString() << " returns false" << endl;
    	}
    	retval->outcome = failed;
    	retval->set_strings(relational_operator_test_failed_str,
    						setb_str,
							set_operation_strings[SET_EQUAL],
							seta_str,
							returns_false_str);
    } else {
    	passed_test_count++;
    	cout << "      " << setb.valuesString() << " != " << seta.valuesString() << " returns true " << endl;
    }

    cout << "Passed " << passed_test_count << " of " << test_count << " tests" << endl;
    cout << endl;

    return retval;
}


Test_Results* runUnsignedOperationTest(struct Test_Arguments *pa) {

	const char *op_str = set_operation_strings[pa->op]; // improves readability
	Set_Contents *contents_before = nullptr;
	Test_Results *test_results = new Test_Results;
	test_results->outcome = passed;
	test_results->set_strings(passed_str, op_str, values_passed, storage_passed, __FUNCTION__);

	cout << __PRETTY_FUNCTION__ << " operator " << op_str << endl;

	WrUIntSet set_result;
	buildSet(set_result, pa->in_a, pa->in_a_sz, pa->msg_lvl);

	for (int i = 0; i !=pa->in_b_sz; i++) {
		contents_before = new Set_Contents(set_result);
		switch (pa->op) {
		case SET_ASSIGN_UNSIGNED_ADD:
			set_result += pa->in_b[i];
			break;
		case SET_ASSIGN_UNSIGNED_SUB:
			set_result -= pa->in_b[i];
			break;
		case SET_UNSIGNED_ADD:
			set_result = set_result + pa->in_b[i];
			break;
		case SET_UNSIGNED_SUB:
			set_result = set_result - pa->in_b[i];
			break;
		default:
			test_results->outcome = failed;
			test_results->set_strings(error_str, op_str, " ... operation invalid for this test function", __FUNCTION__, "");
			return test_results;
		}	// switch (op)
		echoSetUnsignedOperationResult(set_result, pa->op, pa->in_b[i], pa->msg_lvl);
	}	// for (each unsigned

	if (!verifySetResults(empty_str, set_result, pa->exp, pa->exp_sz, newline, pa->msg_lvl)) {
		test_results->outcome = failed;
		test_results->set_strings(failed_str, op_str,  values_failed,  "", "");
		if (isMsgLvlVerbose(pa->msg_lvl)) {
			cout << __FUNCTION__ << " verifySetResults failed" << endl;
		}
	} else {
		// this test is not run unless their are a sufficient number of elements
		//  in the before set to lower to probability of recycling a node address
		//	i.e. - if there was only one element in the before set, given the
		//	number of copy constructors/destructors that occur, it is possible
		//	that a deleted node early in the chain would get new'd in a later set
		if (set_result.size() > 3) {
			if (!pa->rel_tst(contents_before, set_result, pa->msg_lvl)) {
				test_results->outcome = failed;
				test_results->set_strings(passed_str, op_str, storage_failed, "", "");
				if (isMsgLvlVerbose(pa->msg_lvl))
					cout << __FUNCTION__ << " set location test failed";
				cout << endl;
			}
		}
	}

	return test_results;
}


/* ************************************************ */
/*	arithmetic operators:							*/
/*	  result = operand1 + operaand2,				*/
/*	  verify that result is not the same set as		*/
/*		operand1 or operand2						*/
/* ************************************************	*/

Test_Results* runSetArithmeticOperationTest(struct Test_Arguments *pa) {

	cout << __PRETTY_FUNCTION__ << " operator " << set_operation_strings[pa->op] << endl;

	const char *op_str = set_operation_strings[pa->op]; // improves readability

	Test_Results *test_results = new Test_Results;
	test_results->outcome = passed;
	test_results->set_strings(passed_str, op_str, values_passed, storage_passed, __FUNCTION__);

	WrUIntSet operand_1;
	buildSet(operand_1, pa->in_a, pa->in_a_sz, pa->msg_lvl);
	WrUIntSet operand_2;
	buildSet(operand_2, pa->in_b, pa->in_b_sz, pa->msg_lvl);
	//	place a value in result{} that will get overwritten
	WrUIntSet result;

	Set_Contents *operand_1_before = new Set_Contents(operand_1);
	Set_Contents *operand_2_before = new Set_Contents(operand_2);

	cout << endl << endl;

	switch (pa->op) {
	case SET_DIFFERENCE:
		result = operand_1 - operand_2;
		break;
	case SET_INTERSECTION:
		result = operand_1 & operand_2;
		break;
	case SET_UNIQUE:
		result = operand_1 ^ operand_2;
		break;
	case SET_UNION_PLUS:
		result = operand_1 + operand_2;
		break;
	case SET_UNION_OR:
		result = operand_1 | operand_2;
		break;
	// unsuported operator
	default:
		test_results->outcome = failed;
		test_results->set_strings(error_str, op_str, " ... operation invalid for this test function", __FUNCTION__, "");
		return test_results;
	}	// switch (op)

	if (isMsgLvlVerbose(pa->msg_lvl)) {
		cout 	<< " operand_1: " << operand_1 << endl << endl
				<< "\" " << op_str << " \"" << endl << endl
				<< " operand_2: " << operand_2 << endl << endl;
	}

	if (!verifySetResults(empty_str, result, pa->exp, pa->exp_sz, newline, pa->msg_lvl)) {
		test_results->outcome = failed;
		test_results->set_strings(failed_str, op_str,  values_failed,  "", "");
		if (isMsgLvlVerbose(pa->msg_lvl)) {
			cout << __FUNCTION__ << " verifySetResults failed" << endl;
		}
		delete_before(operand_1_before);
		delete_before(operand_2_before);
		return test_results;
	}

	static const char *storage_1_failed = " result has elements in common with operand_1";
	static const char *storage_2_failed = " result has elements in common with operand_2";

	// this test is not run unless their are a sufficient number of elements
	//  in the before set to lower to probability of recycling a node address
	//	i.e. - if there was only one element in the before set, given the
	//	number of copy constructors/destructors that occur, it is possible
	//	that a deleted node early in the chain would get new'd in a later set
	if (result.size() > 3) {
		if (!pa->rel_tst(operand_1_before, result, pa->msg_lvl)) {
			test_results->outcome = failed;
			test_results->set_strings(passed_str, op_str, storage_1_failed, "", "");
			if (isMsgLvlVerbose(pa->msg_lvl))
				cout << __FUNCTION__ << " set location test failed";
			cout << endl;
			delete_before(operand_1_before);
			delete_before(operand_2_before);
			return test_results;
		}
		if (!pa->rel_tst(operand_2_before, result, pa->msg_lvl)) {
			test_results->outcome = failed;
			test_results->set_strings(passed_str, op_str, storage_2_failed, "", "");
			if (isMsgLvlVerbose(pa->msg_lvl))
				cout << __FUNCTION__ << " set location test failed";
			cout << endl;
			delete_before(operand_1_before);
			delete_before(operand_2_before);
			return test_results;
		}
	} else {
		if (isMsgLvlVerbose(pa->msg_lvl)) {
			cout << "INFO - Element storage location test not run," << endl
				 << " set size: " << result.size() << endl;
		}
	}


	delete_before(operand_1_before);
	delete_before(operand_2_before);
	return test_results;
}

/* ************************************************ */
/*	arithmetic operators:							*/
/*	  result = operand1 + operaand2,				*/
/*	  verify that result is not the same set as		*/
/*		operand1 or operand2						*/
/*													*/
/*	assignment operators							*/
/*	  result = operand1								*/
/*	  result += operand2							*/
/*	  verify that result is the same as operaned1	*/
/* ************************************************	*/

Test_Results* runSetAssignmentOperationTest(struct Test_Arguments *pa) {

	cout << __PRETTY_FUNCTION__ << " operator " << set_operation_strings[pa->op] << endl;

	const char *op_str = set_operation_strings[pa->op]; // improves readability

	Test_Results *test_results = new Test_Results;
	test_results->outcome = passed;
	test_results->set_strings(passed_str, op_str, values_passed, storage_passed, __FUNCTION__);

	WrUIntSet operand_1;
	buildSet(operand_1, pa->in_a, pa->in_a_sz, pa->msg_lvl);
	WrUIntSet operand_2;
	buildSet(operand_2, pa->in_b, pa->in_b_sz, pa->msg_lvl);
	//	place a value in result{} that will get overwritten
	WrUIntSet result(operand_1);

	Set_Contents *contents_before = new Set_Contents(result);

	cout << endl << endl;

	switch (pa->op) {
	// assignment operators
	case SET_ASSIGN:
		result = operand_2;
		break;
	case SET_ASSIGN_DIFFERENCE:
		result -= operand_2;
		break;
	case SET_ASSIGN_INTERSECTION:
		result &= operand_2;
		break;
	case SET_ASSIGN_UNIQUE:
		result ^= operand_2;
		break;
	case SET_ASSIGN_UNION_PLUS:
		result += operand_2;
		break;
	case SET_ASSIGN_UNION_OR:
		result |= operand_2;
		break;

	// unsuported operator
	default:
		test_results->outcome = failed;
		test_results->set_strings(error_str, op_str, " ... operation invalid for this test function", __FUNCTION__, "");
		return test_results;
	}	// switch (op)

	if (isMsgLvlVerbose(pa->msg_lvl)) {
		cout << endl;
		cout << operand_1 << endl << endl;
		cout << " " <<  set_operation_strings[pa->op] << " " << endl << endl;
		cout << operand_2 << endl << endl;
		cout << "yields" << endl << endl;
		cout << result << endl << endl;
	}

	if (!verifySetResults(empty_str, result, pa->exp, pa->exp_sz, newline, pa->msg_lvl)) {
		test_results->outcome = failed;
		test_results->set_strings(failed_str, op_str,  values_failed,  "", "");
		if (isMsgLvlVerbose(pa->msg_lvl)) {
			cout << __FUNCTION__ << " verifySetResults failed" << endl;
		}
	} else {
		// this test is not run unless their are a sufficient number of elements
		//  in the before set to lower to probability of recycling a node address
		//	i.e. - if there was only one element in the before set, given the
		//	number of copy constructors/destructors that occur, it is possible
		//	that a deleted node early in the chain would get new'd in a later set
		if (result.size() > 3) {
			if (!pa->rel_tst(contents_before, result, pa->msg_lvl)) {
				test_results->outcome = failed;
				test_results->set_strings(passed_str, op_str, storage_failed, "", "");
				if (isMsgLvlVerbose(pa->msg_lvl))
					cout << __FUNCTION__ << " set location test failed";
				cout << endl;
			}
		} else {
			if (isMsgLvlVerbose(pa->msg_lvl)) {
				cout << "set location test not run, result is too small: " << result.size() << endl;
			}
		}
	}

	cout << endl;

	return test_results;
}

Test_Results* runTest(struct Test_Arguments *args)
{
	Test_Results *retval;

	switch(args->op) {
	case SET_ASSIGN_UNSIGNED_ADD:
	case SET_ASSIGN_UNSIGNED_SUB:
	case SET_UNSIGNED_ADD:
	case SET_UNSIGNED_SUB:
		retval = runUnsignedOperationTest(args);
		break;
	case SET_ASSIGN_DIFFERENCE:
	case SET_ASSIGN_INTERSECTION:
	case SET_ASSIGN_UNION_OR:
	case SET_ASSIGN_UNION_PLUS:
	case SET_ASSIGN_UNIQUE:
		retval = runSetAssignmentOperationTest(args);
		break;
	case SET_DIFFERENCE:
	case SET_INTERSECTION:
	case SET_UNION_OR:
	case SET_UNION_PLUS:
	case SET_UNIQUE:
		retval = runSetArithmeticOperationTest(args);
		break;
	default:
		if (isMsgLvlVerbose(args->msg_lvl)) {
		cout << __PRETTY_FUNCTION__ << endl
			 << "Test  not implemented: "
			 << set_operation_strings[args->op] << endl;
		}
		retval = new Test_Results;
		retval->set_strings(error_str, set_operation_strings[args->op], "Test not implemented", "", "");
		break;
	}

	return retval;
}

bool verifySetResults(	const char *before,
						WrUIntSet& set_to_verify,
						unsigned *expected, int expected_count,
						char *after, Message_Level level) {

	unsigned *outputs;
	int outputs_size = set_to_verify.getValues(&outputs);
	bool retval = verifyResults(before, expected, expected_count, outputs, outputs_size, after, level);
	delete[] outputs;
	return retval;
}

/* ********************************************************************	*/
/* 						echo objects									*/
/* ********************************************************************	*/

void echoSetUnsignedOperationResult(WrUIntSet &set, Set_Operations op, unsigned operand, Message_Level message_level)
{
	if (isMsgLvlVerbose(message_level)) {
		cout 	<< "set " << set_operation_strings[op] << " " << operand
				<< " results in set @" << &set << endl
				<< set << endl;
	}
}

void echoSet(const char* before, WrUIntSet& set, const char *after, Message_Level message_level) {

	if (isMsgLvlNone(message_level))
		return;

	if (!isMsgLvlValues(message_level)) {
		// echo the addresses of each variable in the set
		cout 	<< before << "set:  @" << &set << ": " <<set.toString();
		if (isMsgLvlValues(message_level)) {
			cout << "\nlist: " << set.list << endl;
		}
		cout 	<< setw(0) << after;
	} else {
		// create (eg): { 0, 1, 2 }
		WrUIntNode *p = set.list.head;
		cout << before << "{ ";
		if (p == nullptr) {
			cout << "empty }" << after;
		}
		else {
			WrUIntNode *p = set.list.head;
			while (p->next != nullptr) {
				cout << setw(0) << p->data.get_x() << ", ";
				p = p->next;
			}
			cout << setw(0) << p->data.get_x() << " }" << after;
		}
	}
}

#pragma pop_macro("NUM_TEST_RESULT_MESSAGES")
#pragma pop_macro("cout_count")
#pragma pop_macro("delete_object")
#pragma pop_macro("delete_array")
#pragma pop_macro("delete_args")
#pragma pop_macro("delete_before")
#pragma pop_macro("delete_results")
