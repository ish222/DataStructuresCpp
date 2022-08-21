#include "../Vector.h"
#include "gtest/gtest.h"

TEST (VectorTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Vector<int> vector;
	ASSERT_EQ (vector.size(), 0);
	vector.push_back(10);
	ASSERT_EQ (vector.size(), 1);
	vector.push_back({20, 30, 40});
	ASSERT_EQ (vector.size(), 4);
	
	// Capacity initialization
	custom::Vector<int> vector_val(10);
	ASSERT_EQ (vector_val.size(), 0);
	
	// Initializer vector initialization
	custom::Vector<int> vector2 = {1,2,3,4,5};
	ASSERT_EQ (vector2.size(), 5);
	
	// Copy initialization
	custom::Vector<int> vector3(vector);
	ASSERT_EQ (vector3.size(), vector.size());

	// Move initialization
	custom::Vector<int> vector_move(std::move(vector3));
	ASSERT_EQ (vector_move.size(), vector.size());
	ASSERT_EQ (vector3.size(), 0);
	ASSERT_TRUE (vector3.empty());
}

TEST (VectorTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::Vector<int> vector ({1,2,3,4,5,6,7});
	custom::Vector<int> vector2;
	vector2 = vector;
	ASSERT_EQ (vector2.size(), vector.size());
	ASSERT_EQ (vector2, vector);
	
	// Move assignment
	custom::Vector<int> vector3({10});
	ASSERT_EQ (vector3.front(), 10);
	custom::Vector<int> vector4;
	vector4 = std::move(vector3);
	ASSERT_EQ (vector4.front(), 10);
	ASSERT_TRUE (vector3.empty());
}

TEST (VectorTests /*test suite name*/, Methods /*test name*/) {
	custom::Vector<int> vector ({1,2,3,4,5,6,7});
	ASSERT_EQ (vector.back(), 7);
	vector.pop_back();
	ASSERT_EQ (vector.back(), 6);
	ASSERT_EQ (vector.front(), 1);
	const custom::Vector<int> const_vector(vector);
	ASSERT_EQ (vector.back(), 6);
	ASSERT_TRUE (vector);
	ASSERT_EQ (vector, const_vector);
	custom::Vector<int> vector2 = {8,9,10};
	custom::Vector<int> vector3 = vector + vector2;
	ASSERT_EQ (vector3.back(), 10);
	ASSERT_EQ (vector3.size(), 9);
	vector3.emplace_back(11);
	ASSERT_EQ (vector3[9], 11);
	vector3.clear();
	ASSERT_FALSE (vector3);
}

TEST (VectorTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty vector exception test
	custom::Vector<int> vector2;
	ASSERT_TRUE (vector2.empty());
	ASSERT_THROW (static_cast<void>(vector2.pop_back()), std::runtime_error);
	ASSERT_THROW (static_cast<void>(vector2.back()), std::runtime_error);
	ASSERT_THROW (static_cast<void>(vector2.front()), std::runtime_error);
	ASSERT_THROW (static_cast<void>(vector2[0]), std::invalid_argument);
}

TEST (VectorTests /*test suite name*/, IteratorTest /*test name*/) {
	custom::Vector<int> vector = {1,2,3,4,5,6,7,8,9};
	
	// Range-based for loop
	int j = 1;
	for (const int& i : vector) {
		ASSERT_EQ (i, j++);
	}

	// Iterator tests, with methods
	auto it = vector.begin();
	ASSERT_EQ (*it, 1);
	it++;
	ASSERT_EQ (*it, 2);
	it--;
	ASSERT_EQ (*it, 1);
	++it;
	ASSERT_EQ (*it, 2);
	--it;
	ASSERT_EQ (*it, 1);
	it = vector.end();
	ASSERT_THROW(it.advance(100), std::runtime_error);
	ASSERT_THROW(it++, std::out_of_range);
	ASSERT_THROW(++it, std::out_of_range);
	it = vector.begin();
	it.advance(3);
	ASSERT_EQ (*it, 4);
	it.advance(-3);
	ASSERT_EQ (*it, 1);
	it  = it + 1;
	ASSERT_EQ (*it, 2);
	it += 2;
	ASSERT_EQ (*it, 4);
	it = it - 1;
	ASSERT_EQ (*it, 3);
	it -= 2;
	ASSERT_EQ (*it, 1);
	ASSERT_THROW (it.advance(100), std::invalid_argument);
	it = vector.end();
	ASSERT_THROW (++it, std::out_of_range);
	it = vector.end();
	ASSERT_THROW (it++, std::out_of_range);
	it = vector.begin();
	ASSERT_THROW (--it, std::out_of_range);
	it = vector.begin();
	ASSERT_THROW (it--, std::out_of_range);
	it = vector.begin();
	ASSERT_THROW (it.advance(-100), std::invalid_argument);
	it = vector.begin();
	ASSERT_THROW (it=it+100, std::out_of_range);
	it = vector.begin();
	ASSERT_THROW (it+=100, std::out_of_range);
	it = vector.end();
	ASSERT_THROW (it=it-100, std::out_of_range);
	it = vector.end();
	ASSERT_THROW (it-=100, std::out_of_range);
	auto it2 = vector.begin();
	it = vector.begin();
	ASSERT_TRUE (it == it2);
	++it;
	ASSERT_FALSE (it == it2);
	ASSERT_TRUE (it != it2);
	--it;
	ASSERT_TRUE (it == it2);

	custom::Vector<std::vector<int>> vec3 = {{1,2,3,4}, {1,2,3}};
	auto it3 = vec3.begin();
	ASSERT_EQ (it3->size(), 4);
	++it3;
	ASSERT_EQ (it3->size(), 3);
}