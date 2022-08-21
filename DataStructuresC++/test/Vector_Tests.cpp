#include "../Vector.h"
#include "gtest/gtest.h"

TEST (VectorTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Vector<int> vector;
	EXPECT_EQ (vector.size(), 0);
	vector.push_back(10);
	EXPECT_EQ (vector.size(), 1);
	vector.push_back({20, 30, 40});
	EXPECT_EQ (vector.size(), 4);
	
	// Capacity initialization
	custom::Vector<int> vector_val(10);
	EXPECT_EQ (vector_val.size(), 0);
	
	// Initializer vector initialization
	custom::Vector<int> vector2 = {1,2,3,4,5};
	EXPECT_EQ (vector2.size(), 5);
	
	// Copy initialization
	custom::Vector<int> vector3(vector);
	EXPECT_EQ (vector3.size(), vector.size());

	// Move initialization
	custom::Vector<int> vector_move(std::move(vector3));
	EXPECT_EQ (vector_move.size(), vector.size());
	EXPECT_EQ (vector3.size(), 0);
	EXPECT_TRUE (vector3.empty());
}

TEST (VectorTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::Vector<int> vector ({1,2,3,4,5,6,7});
	custom::Vector<int> vector2;
	vector2 = vector;
	EXPECT_EQ (vector2.size(), vector.size());
	EXPECT_EQ (vector2, vector);
	
	// Move assignment
	custom::Vector<int> vector3({10});
	EXPECT_EQ (vector3.front(), 10);
	custom::Vector<int> vector4;
	vector4 = std::move(vector3);
	EXPECT_EQ (vector4.front(), 10);
	EXPECT_TRUE (vector3.empty());
}

TEST (VectorTests /*test suite name*/, Methods /*test name*/) {
	custom::Vector<int> vector ({1,2,3,4,5,6,7});
	EXPECT_EQ (vector.back(), 7);
	vector.pop_back();
	EXPECT_EQ (vector.back(), 6);
	EXPECT_EQ (vector.front(), 1);
	const custom::Vector<int> const_vector(vector);
	EXPECT_EQ (vector.back(), 6);
	EXPECT_TRUE (vector);
	EXPECT_EQ (vector, const_vector);
	custom::Vector<int> vector2 = {8,9,10};
	custom::Vector<int> vector3 = vector + vector2;
	EXPECT_EQ (vector3.back(), 10);
	EXPECT_EQ (vector3.size(), 9);
	vector3.emplace_back(11);
	EXPECT_EQ (vector3[9], 11);
	vector3.clear();
	EXPECT_FALSE (vector3);
}

TEST (VectorTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty vector exception test
	custom::Vector<int> vector2;
	EXPECT_TRUE (vector2.empty());
	EXPECT_THROW (static_cast<void>(vector2.pop_back()), std::runtime_error);
	EXPECT_THROW (static_cast<void>(vector2.back()), std::runtime_error);
	EXPECT_THROW (static_cast<void>(vector2.front()), std::runtime_error);
	EXPECT_THROW (static_cast<void>(vector2[0]), std::invalid_argument);
}

TEST (VectorTests /*test suite name*/, IteratorTest /*test name*/) {
	custom::Vector<int> vector = {1,2,3,4,5,6,7,8,9};
	
	// Range-based for loop
	int j = 1;
	for (const int& i : vector) {
		EXPECT_EQ (i, j++);
	}

	// Iterator tests, with methods
	auto it = vector.begin();
	EXPECT_EQ (*it, 1);
	it++;
	EXPECT_EQ (*it, 2);
	it--;
	EXPECT_EQ (*it, 1);
	++it;
	EXPECT_EQ (*it, 2);
	--it;
	EXPECT_EQ (*it, 1);
	it = vector.end();
	EXPECT_THROW(it.advance(100), std::runtime_error);
	EXPECT_THROW(it++, std::out_of_range);
	EXPECT_THROW(++it, std::out_of_range);
	it = vector.begin();
	it.advance(3);
	EXPECT_EQ (*it, 4);
	it.advance(-3);
	EXPECT_EQ (*it, 1);
	it  = it + 1;
	EXPECT_EQ (*it, 2);
	it += 2;
	EXPECT_EQ (*it, 4);
	it = it - 1;
	EXPECT_EQ (*it, 3);
	it -= 2;
	EXPECT_EQ (*it, 1);
	EXPECT_THROW (it.advance(100), std::invalid_argument);
	it = vector.end();
	EXPECT_THROW (++it, std::out_of_range);
	it = vector.end();
	EXPECT_THROW (it++, std::out_of_range);
	it = vector.begin();
	EXPECT_THROW (--it, std::out_of_range);
	it = vector.begin();
	EXPECT_THROW (it--, std::out_of_range);
	it = vector.begin();
	EXPECT_THROW (it.advance(-100), std::invalid_argument);
	it = vector.begin();
	EXPECT_THROW (it=it+100, std::out_of_range);
	it = vector.begin();
	EXPECT_THROW (it+=100, std::out_of_range);
	it = vector.end();
	EXPECT_THROW (it=it-100, std::out_of_range);
	it = vector.end();
	EXPECT_THROW (it-=100, std::out_of_range);
	auto it2 = vector.begin();
	it = vector.begin();
	EXPECT_TRUE (it == it2);
	++it;
	EXPECT_FALSE (it == it2);
	EXPECT_TRUE (it != it2);
	--it;
	EXPECT_TRUE (it == it2);

	custom::Vector<std::vector<int>> vec3 = {{1,2,3,4}, {1,2,3}};
	auto it3 = vec3.begin();
	EXPECT_EQ (it3->size(), 4);
	++it3;
	EXPECT_EQ (it3->size(), 3);
}