#include "../LinkedList.h"
#include "gtest/gtest.h"

TEST (LinkedListTest /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::LinkedList<int> list;
	ASSERT_EQ (list.length(), 0);
	list.append(10);
	ASSERT_EQ (list.length(), 1);
	list.append({20, 30, 40});
	ASSERT_EQ (list.length(), 4);

	// Value initialization
	custom::LinkedList<int> list_val(10);
	ASSERT_EQ (list_val.length(), 1);

	// Initializer list initialization
	custom::LinkedList<int> list2 = {1,2,3,4,5};
	ASSERT_EQ (list2.length(), 5);

	// Copy initialization
	custom::LinkedList<int> list3(list);
	ASSERT_EQ (list3.length(), list.length());

	// Move initialization
	custom::LinkedList<int> list_move(std::move(list3));
	ASSERT_EQ (list_move.length(), list.length());
	ASSERT_TRUE (list3.empty());
}

TEST (LinkedListTest /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::LinkedList<int> list = {1,2,3,4,5,6,7};
	custom::LinkedList<int> list2;
	list2 = list;
	ASSERT_EQ (list2.length(), list.length());
	ASSERT_EQ (list2.contents(), list.contents());

	// Move assignment
	custom::LinkedList<int> list3(10);
	ASSERT_EQ (list3.back(), 10);
	custom::LinkedList<int> list4;
	list4 = std::move(list3);
	ASSERT_EQ (list4.back(), 10);
	ASSERT_TRUE (list3.empty());
}

TEST (LinkedListTest /*test suite name*/, Methods /*test name*/) {
	// Access members
	custom::LinkedList<int> list = {1,2,3,4,5,6,7};
	ASSERT_EQ (list[0], 1);
	ASSERT_EQ (list[6], 7);
	ASSERT_THROW (list[-1], std::invalid_argument);
	ASSERT_THROW (list[10], std::invalid_argument);

	ASSERT_EQ (list.front(), 1);
	ASSERT_EQ (list.back(), 7);
	list.push_back(8);
	list.push_front(0);
	ASSERT_EQ (list.front(), 0);
	ASSERT_EQ (list.back(), 8);

	ASSERT_EQ (list.find(2), 2);
	ASSERT_EQ (list.find(100), -1);

	ASSERT_FALSE (list.empty());
	ASSERT_TRUE (list);

	custom::LinkedList<int> list2(list);
	ASSERT_TRUE (list == list2);
	list.append(9);
	ASSERT_FALSE (list == list2);
	ASSERT_TRUE (list != list2);

	list.erase(0);
	ASSERT_EQ (list.front(), 1);
	ASSERT_THROW(list.erase(100), std::invalid_argument);
	ASSERT_THROW(list.insert(10, 100), std::invalid_argument);
}

TEST (LinkedListTest /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty list exception test
	custom::LinkedList<int> list2;
	ASSERT_TRUE(list2.empty());
	ASSERT_THROW(list2.erase(0), std::runtime_error);
	ASSERT_THROW(list2.insert(0, 0), std::runtime_error);
	ASSERT_TRUE (list2.contents().empty());
	ASSERT_THROW(list2.find(10), std::runtime_error);
	ASSERT_THROW(list2.get(0), std::runtime_error);
	ASSERT_THROW(list2.front(), std::runtime_error);
	ASSERT_THROW(list2.back(), std::runtime_error);
	ASSERT_THROW(list2.pop_front(), std::runtime_error);
	ASSERT_THROW(list2.pop_back(), std::runtime_error);
	ASSERT_THROW(list2[0], std::runtime_error);
}