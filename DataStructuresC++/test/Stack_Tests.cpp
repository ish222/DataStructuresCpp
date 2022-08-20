#include "../Stack.h"
#include "gtest/gtest.h"

TEST (StackTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Stack<int> stack;
	ASSERT_EQ (stack.length(), 0);
	stack.push(10);
	ASSERT_EQ (stack.length(), 1);
	stack.push({20, 30, 40});
	ASSERT_EQ (stack.length(), 4);
	
	// Value initialization
	custom::Stack<int> stack_val(10);
	ASSERT_EQ (stack_val.length(), 1);
	ASSERT_EQ (stack_val.peek(), 10);
	
	// Initializer stack initialization
	custom::Stack<int> stack2 = {1,2,3,4,5};
	ASSERT_EQ (stack2.length(), 5);

	// Copy initialization
	custom::Stack<int> stack3(stack);
	ASSERT_EQ (stack3.length(), stack.length());

	// Move initialization
	custom::Stack<int> stack_move(std::move(stack3));
	ASSERT_EQ (stack_move.length(), stack.length());
	ASSERT_TRUE (stack3.empty());
}

TEST (StackTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::Stack<int> stack ({1,2,3,4,5,6,7});
	custom::Stack<int> stack2;
	stack2 = stack;
	ASSERT_EQ (stack2.length(), stack.length());
	ASSERT_EQ (stack2, stack);
	
	// Move assignment
	custom::Stack<int> stack3(10);
	ASSERT_EQ (stack3.peek(), 10);
	custom::Stack<int> stack4;
	stack4 = std::move(stack3);
	ASSERT_EQ (stack4.peek(), 10);
	ASSERT_TRUE (stack3.empty());
}

TEST (StackTests /*test suite name*/, Methods /*test name*/) {
	custom::Stack<int> stack ({1,2,3,4,5,6,7});
	int top = stack.pop();
	ASSERT_EQ (top, 7);
	ASSERT_EQ (stack.peek(), 6);
	const custom::Stack<int> const_stack(stack);
	ASSERT_EQ (stack.peek(), 6);
	ASSERT_TRUE (stack);
	ASSERT_EQ (stack, const_stack);
	ASSERT_TRUE (stack.contains(5));
	ASSERT_FALSE (stack.contains(100));
	custom::Stack<int> stack2 = {8,9,10};
	custom::Stack<int> stack3 = stack + stack2;
	ASSERT_EQ (stack3.peek(), 10);
	ASSERT_EQ (stack3.length(), 9);
	stack3.clear();
	ASSERT_FALSE (stack3);
}

TEST (StackTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty stack exception test
	custom::Stack<int> stack2;
	ASSERT_TRUE (stack2.empty());
	ASSERT_THROW (static_cast<void>(stack2.pop()), std::runtime_error);
	ASSERT_TRUE (stack2.contents().empty());
	ASSERT_THROW (static_cast<void>(stack2.peek()), std::runtime_error);
	ASSERT_THROW (stack2.contains(3), std::runtime_error);
	ASSERT_THROW (stack2.display(), std::runtime_error);
}