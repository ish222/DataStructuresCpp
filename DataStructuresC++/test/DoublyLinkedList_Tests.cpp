#include "../DoublyLinkedList.h"
#include "gtest/gtest.h"

TEST (DoublyLinkedListTest /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::DoublyLinkedList<int> list;
	ASSERT_EQ (list.length(), 0);
	list.append(10);
	ASSERT_EQ (list.length(), 1);
	list.append({20, 30, 40});
	ASSERT_EQ (list.length(), 4);

	// Value initialization
	custom::DoublyLinkedList<int> list_val(10);
	ASSERT_EQ (list_val.length(), 1);

	// Initializer list initialization
	custom::DoublyLinkedList<int> list2 = {1,2,3,4,5};
	ASSERT_EQ (list2.length(), 5);

	// Copy initialization
	custom::DoublyLinkedList<int> list3(list);
	ASSERT_EQ (list3.length(), list.length());

	// Move initialization
	custom::DoublyLinkedList<int> list_move(std::move(list3));
	ASSERT_EQ (list_move.length(), list.length());
	ASSERT_TRUE (list3.empty());
}

TEST (DoublyLinkedListTest /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::DoublyLinkedList<int> list = {1,2,3,4,5,6,7};
	custom::DoublyLinkedList<int> list2;
	list2 = list;
	ASSERT_EQ (list2.length(), list.length());
	ASSERT_EQ (list2.contents(), list.contents());

	// Move assignment
	custom::DoublyLinkedList<int> list3(10);
	ASSERT_EQ (list3.back(), 10);
	custom::DoublyLinkedList<int> list4;
	list4 = std::move(list3);
	ASSERT_EQ (list4.back(), 10);
	ASSERT_TRUE (list3.empty());
}

TEST (DoublyLinkedListTest /*test suite name*/, Methods /*test name*/) {
	// Access members
	custom::DoublyLinkedList<int> list = {1,2,3,4,5,6,7};
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

	custom::DoublyLinkedList<int> list2(list);
	ASSERT_TRUE (list == list2);
	list.append(9);
	ASSERT_FALSE (list == list2);
	ASSERT_TRUE (list != list2);

	list.erase(0);
	ASSERT_EQ (list.front(), 1);
	ASSERT_THROW (list.erase(100), std::invalid_argument);
	ASSERT_THROW (list.insert(10, 100), std::invalid_argument);

	custom::DoublyLinkedList<int> list3 = {10, 11, 12, 13};
	custom::DoublyLinkedList<int> list4 = list + list3;
	ASSERT_EQ (list4.front(), 1);
	ASSERT_EQ (list4.back(), 13);
	ASSERT_EQ (list4.length(), 13);

	custom::DoublyLinkedList<int> list_r = {1,2,3,4,5};
	list_r.reverse_order();
	custom::DoublyLinkedList<int> list_rev = {5,4,3,2,1};
	ASSERT_EQ (list_r, list_rev);
}

TEST (DoublyLinkedListTest /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty list exception test
	custom::DoublyLinkedList<int> list2;
	ASSERT_TRUE (list2.empty());
	ASSERT_THROW (list2.erase(0), std::runtime_error);
	ASSERT_THROW (list2.insert(0, 0), std::runtime_error);
	ASSERT_TRUE (list2.contents().empty());
	ASSERT_THROW (static_cast<void>(list2.find(10)), std::runtime_error);
	ASSERT_THROW (static_cast<void>(list2.get(0)), std::runtime_error);
	ASSERT_THROW (list2.front(), std::runtime_error);
	ASSERT_THROW (list2.back(), std::runtime_error);
	ASSERT_THROW (list2.pop_front(), std::runtime_error);
	ASSERT_THROW (list2.pop_back(), std::runtime_error);
	ASSERT_THROW (static_cast<void>(list2[0]), std::runtime_error);
}

TEST (DoublyLinkedListTest /*test suite name*/, IteratorTest /*test name*/) {
	custom::DoublyLinkedList<int> list = {1,2,3,4,5,6,7,8,9};

	// Range-based for loop
	int j = 1;
	for (const int& i : list) {
		ASSERT_EQ (i, j++);
	}

	// Iterator tests, with methods
	auto it = list.begin();
	ASSERT_EQ (*it, 1);
	it++;
	ASSERT_EQ (*it, 2);
	it--;
	ASSERT_EQ (*it, 1);
	++it;
	ASSERT_EQ (*it, 2);
	--it;
	ASSERT_EQ (*it, 1);
	it = list.end();
	ASSERT_EQ (it, nullptr);
	ASSERT_THROW(it.advance(100), std::runtime_error);
	it = list.begin();
	it.advance(3);
	ASSERT_EQ (*it, 4);
	it.advance(-3);
	ASSERT_EQ (*it, 1);
	auto it3 = it.next();
	ASSERT_EQ (*it3, 2);
	auto it4 = it3.prev();
	ASSERT_EQ (*it4, 1);
	it  = it + 1;
	ASSERT_EQ (*it, 2);
	it += 2;
	ASSERT_EQ (*it, 4);
	it = it - 1;
	ASSERT_EQ (*it, 3);
	it -= 2;
	ASSERT_EQ (*it, 1);
	ASSERT_THROW (it.advance(100), std::invalid_argument);
	it = list.end();
	ASSERT_THROW (++it, std::out_of_range);
	it = list.end();
	ASSERT_THROW (it++, std::out_of_range);
	it = list.begin();
	--it;
	ASSERT_THROW (--it, std::out_of_range);
	it = list.begin();
	--it;
	ASSERT_THROW (it--, std::out_of_range);
	it = list.begin();
	ASSERT_THROW (it.advance(-100), std::invalid_argument);
	it = list.begin();
	ASSERT_THROW (it=it+100, std::out_of_range);
	it = list.begin();
	ASSERT_THROW (it+=100, std::out_of_range);
	it = list.end();
	ASSERT_THROW (it=it-100, std::out_of_range);
	it = list.end();
	ASSERT_THROW (it-=100, std::out_of_range);
	auto it2 = list.begin();
	it = list.begin();
	ASSERT_TRUE (it == it2);
	++it;
	ASSERT_FALSE (it == it2);
	ASSERT_TRUE (it != it2);
	--it;
	ASSERT_TRUE (it == it2);
}