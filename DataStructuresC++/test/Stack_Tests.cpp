#include "../Stack.h"
#include "gtest/gtest.h"

TEST (StackTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Stack<int> stack;
	EXPECT_EQ (stack.length(), 0);
	stack.push(10);
	EXPECT_EQ (stack.length(), 1);
	stack.push({20, 30, 40});
	EXPECT_EQ (stack.length(), 4);
	
	// Value initialization
	custom::Stack<int> stack_val(10);
	EXPECT_EQ (stack_val.length(), 1);
	EXPECT_EQ (stack_val.peek(), 10);
	
	// Initializer stack initialization
	custom::Stack<int> stack2 = {1,2,3,4,5};
	EXPECT_EQ (stack2.length(), 5);

	// Copy initialization
	custom::Stack<int> stack3(stack);
	EXPECT_EQ (stack3.length(), stack.length());

	// Move initialization
	custom::Stack<int> stack_move(std::move(stack3));
	EXPECT_EQ (stack_move.length(), stack.length());
	EXPECT_TRUE (stack3.empty());
}

TEST (StackTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::Stack<int> stack ({1,2,3,4,5,6,7});
	custom::Stack<int> stack2;
	stack2 = stack;
	EXPECT_EQ (stack2.length(), stack.length());
	EXPECT_EQ (stack2, stack);
	
	// Move assignment
	custom::Stack<int> stack3(10);
	EXPECT_EQ (stack3.peek(), 10);
	custom::Stack<int> stack4;
	stack4 = std::move(stack3);
	EXPECT_EQ (stack4.peek(), 10);
	EXPECT_TRUE (stack3.empty());
}

TEST (StackTests /*test suite name*/, Methods /*test name*/) {
	custom::Stack<int> stack ({1,2,3,4,5,6,7});
	int top = stack.pop();
	EXPECT_EQ (top, 7);
	EXPECT_EQ (stack.peek(), 6);
	const custom::Stack<int> const_stack(stack);
	EXPECT_EQ (stack.peek(), 6);
	EXPECT_TRUE (stack);
	EXPECT_EQ (stack, const_stack);
	EXPECT_TRUE (stack.contains(5));
	EXPECT_FALSE (stack.contains(100));
	custom::Stack<int> stack2 = {8,9,10};
	custom::Stack<int> stack3 = stack + stack2;
	EXPECT_EQ (stack3.peek(), 10);
	EXPECT_EQ (stack3.length(), 9);
	stack3.clear();
	EXPECT_FALSE (stack3);
}

TEST (StackTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty stack exception test
	custom::Stack<int> stack2;
	EXPECT_TRUE (stack2.empty());
	EXPECT_THROW (static_cast<void>(stack2.pop()), std::runtime_error);
	EXPECT_TRUE (stack2.contents().empty());
	EXPECT_THROW (static_cast<void>(stack2.peek()), std::runtime_error);
	EXPECT_THROW (stack2.contains(3), std::runtime_error);
	EXPECT_THROW (stack2.display(), std::runtime_error);
}