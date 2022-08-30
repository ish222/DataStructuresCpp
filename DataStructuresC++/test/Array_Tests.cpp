#include "../Array.h"
#include "gtest/gtest.h"

TEST (ArrayTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Array<int, 10> arr;
	EXPECT_EQ (arr.size(), 10);

	// Initializer vector initialization
	custom::Array<int, 10> arr_init = {1, 2, 3, 4, 5};
	EXPECT_EQ (arr_init.size(), 10);
	EXPECT_EQ (arr_init[1], 2);

	// Copy initialization
	custom::Array<int, 10> arr_cpy(arr_init);
	EXPECT_EQ (arr_cpy.size(), arr_init.size());
	EXPECT_EQ (arr_cpy[2], arr_init[2]);

	// Move initialization
	custom::Array<int, 10> arr_move(std::move(arr_init));
	EXPECT_EQ (arr_cpy.size(), arr_move.size());
	EXPECT_EQ (arr_init.size(), 0);
}

TEST (ArrayTests /*test suite name*/, Assignment /*test name*/) {
	custom::Array<int, 10> arr_init = {1, 2, 3, 4, 5};

	// Copy assignment
	custom::Array<int, 10> arr_cpy = arr_init;
	EXPECT_EQ (arr_cpy.size(), arr_init.size());
	EXPECT_EQ (arr_cpy[2], arr_init[2]);

	// Move assignment
	custom::Array<int, 10> arr_move = std::move(arr_init);
	EXPECT_EQ (arr_cpy.size(), arr_move.size());
	EXPECT_EQ (arr_init.size(), 0);
}

TEST (ArrayTests /*test suite name*/, Methods /*test name*/) {
	custom::Array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	EXPECT_EQ (arr[0], 0);
	EXPECT_EQ (arr[9], 9);
	EXPECT_TRUE (arr);
	EXPECT_EQ (arr.size(), 10);
}

TEST (ArrayTests /*test suite name*/, IteratorTest /*test name*/) {
	custom::Array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	size_t j = 0;
	for (const auto& i : arr) {
		EXPECT_EQ (i, j++);
	}
	EXPECT_EQ (*(arr.begin()), 0);
	EXPECT_EQ (*(arr.end()-1), 9);
}