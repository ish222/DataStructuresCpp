#include "../Queue.h"
#include "gtest/gtest.h"

TEST (QueueTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::Queue<int> queue;
	EXPECT_EQ (queue.length(), 0);
	queue.enqueue(10);
	EXPECT_EQ (queue.length(), 1);
	queue.enqueue({20, 30, 40});
	EXPECT_EQ (queue.length(), 4);
	
	// Value initialization
	custom::Queue<int> queue_val(10);
	EXPECT_EQ (queue_val.length(), 1);

	// Initializer queue initialization
	custom::Queue<int> queue2 = {1,2,3,4,5};
	EXPECT_EQ (queue2.length(), 5);

	// Copy initialization
	custom::Queue<int> queue3(queue);
	EXPECT_EQ (queue3.length(), queue.length());

	// Move initialization
	custom::Queue<int> queue_move(std::move(queue3));
	EXPECT_EQ (queue_move.length(), queue.length());
	EXPECT_TRUE (queue3.empty());
}

TEST (QueueTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::Queue<int> queue ({1,2,3,4,5,6,7});
	custom::Queue<int> queue2;
	queue2 = queue;
	EXPECT_EQ (queue2.length(), queue.length());
	EXPECT_EQ (queue2, queue);

	// Move assignment
	custom::Queue<int> queue3(10);
	EXPECT_EQ (queue3.peek(), 10);
	custom::Queue<int> queue4;
	queue4 = std::move(queue3);
	EXPECT_EQ (queue4.peek(), 10);
	EXPECT_TRUE (queue3.empty());
}

TEST (QueueTests /*test suite name*/, Methods /*test name*/) {
	custom::Queue<int> queue ({1,2,3,4,5,6,7});
	int front = queue.dequeue();
	EXPECT_EQ (front, 1);
	EXPECT_EQ (queue.peek(), 2);
	const custom::Queue<int> const_queue(queue);
	EXPECT_EQ (queue.peek(), 2);
	EXPECT_TRUE (queue);
	EXPECT_EQ (queue, const_queue);
	EXPECT_TRUE (queue.contains(7));
	EXPECT_FALSE (queue.contains(100));
	custom::Queue<int> queue2 = {8,9,10};
	custom::Queue<int> queue3 = queue + queue2;
	EXPECT_EQ (queue3.peek(), 2);
	EXPECT_EQ (queue3.length(), 9);
	queue3.clear();
	EXPECT_FALSE (queue3);
}

TEST (QueueTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty queue exception test
	custom::Queue<int> queue2;
	EXPECT_TRUE (queue2.empty());
	EXPECT_THROW (static_cast<void>(queue2.dequeue()), std::runtime_error);
	EXPECT_TRUE (queue2.contents().empty());
	EXPECT_THROW (static_cast<void>(queue2.peek()), std::runtime_error);
	EXPECT_THROW (queue2.contains(3), std::runtime_error);
	EXPECT_THROW (queue2.display(), std::runtime_error);
}

TEST (PriorityQueueTests /*test suite name*/, Initialisation /*test name*/) {
	// Default initialization
	custom::PriorityQueue<int> queue;
	EXPECT_EQ (queue.length(), 0);
	queue.enqueue(10);
	EXPECT_EQ (queue.length(), 1);
	queue.enqueue({20, 30, 40});
	EXPECT_EQ (queue.length(), 4);

	// Value initialization
	custom::PriorityQueue<int> queue_val(10);
	EXPECT_EQ (queue_val.length(), 1);

	// Initializer queue initialization
	custom::PriorityQueue<int> queue2;
	queue2.enqueue({1,2,3,4,5});
	EXPECT_EQ (queue2.length(), 5);

	// Copy initialization
	custom::PriorityQueue<int> queue3(queue);
	EXPECT_EQ (queue3.length(), queue.length());
	custom::Queue<int> queue4(queue);
	custom::PriorityQueue<int> queue5(queue4);
	EXPECT_EQ (queue4, queue);
	EXPECT_EQ (queue5, queue4);

	// Move initialization
	custom::PriorityQueue<int> queue_move(std::move(queue3));
	EXPECT_EQ (queue_move.length(), queue.length());
	EXPECT_TRUE (queue3.empty());
	custom::Queue<int> queue6(std::move(queue));
	EXPECT_EQ (queue6, queue_move);
	EXPECT_TRUE (queue.empty());
	custom::PriorityQueue<int> queue7(std::move(queue6));
	EXPECT_EQ (queue7, queue_move);
	EXPECT_TRUE (queue6.empty());
}

TEST (PriorityQueueTests /*test suite name*/, Assignment /*test name*/) {
	// Copy assignment
	custom::PriorityQueue<int> queue;
	queue.enqueue({1,2,3,4,5,6,7});
	custom::PriorityQueue<int> queue2;
	queue2 = queue;
	EXPECT_EQ (queue2.length(), queue.length());
	EXPECT_EQ (queue2, queue);
	
	// Move assignment
	custom::PriorityQueue<int> queue3(10);
	EXPECT_EQ (queue3.peek(), 10);
	custom::PriorityQueue<int> queue4;
	queue4 = std::move(queue3);
	EXPECT_EQ (queue4.peek(), 10);
	EXPECT_TRUE (queue3.empty());
}

TEST (PriorityQueueTests /*test suite name*/, Methods /*test name*/) {
	custom::PriorityQueue<int> queue(7, 1);
	queue.enqueue({3,1,2,5,6,4});
	int front = queue.dequeue();
	EXPECT_EQ (front, 1);
	EXPECT_EQ (queue.peek(), 2);
	const custom::PriorityQueue<int> const_queue(queue);
	EXPECT_EQ (queue.peek(), 2);
	EXPECT_TRUE (queue);
	EXPECT_EQ (queue, const_queue);
	EXPECT_TRUE (queue.contains(7));
	EXPECT_FALSE (queue.contains(100));
	custom::PriorityQueue<int> queue2;
	queue2.enqueue({8,9,10});
	custom::PriorityQueue<int> queue3 = queue + queue2;
	EXPECT_EQ (queue3.peek(), 2);
	EXPECT_EQ (queue3.length(), 9);
	queue3.clear();
	EXPECT_FALSE (queue3);
}

TEST (PriorityQueueTests /*test suite name*/, EmptyListExceptions /*test name*/) {
	// Empty priority queue exception test
	custom::PriorityQueue<int> queue2;
	EXPECT_TRUE (queue2.empty());
	EXPECT_THROW (static_cast<void>(queue2.dequeue()), std::runtime_error);
	EXPECT_TRUE (queue2.contents().empty());
	EXPECT_THROW (static_cast<void>(queue2.peek()), std::runtime_error);
	EXPECT_THROW (queue2.contains(3), std::runtime_error);
	EXPECT_THROW (queue2.display(), std::runtime_error);
}