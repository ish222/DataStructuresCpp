#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "BinaryTree.h"
#include "Graph.h"
#include "Map.h"
#include "BinarySearchTree.h"

template<typename T>
void printvec(std::vector<T> data) {
	for (T& i : data) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main() {
	try {
		LinkedList<float> list(2.31f);
		list.append(3.13f);
		list.append(5.87f);
		list.append(15.55f);
		list.append(45.90f);
		list.display();
		std::cout << list.length() << std::endl;
		list.erase(2);
		list.display();
		std::cout << list[0] <<"\n\n";

		list.reverse_order();
		list.display();
		std::cout << "\n";

		LinkedList<float> list2;
		list2.append(1.90f);
		list2.append(2.90f);
		list2.append(3.90f);
		list2.display();
		std::cout << "\n";

		LinkedList list3 = list + list2;
		list3.display();
		std::cout << "\n\n";

		Queue<std::string> queue;
		queue.enqueue("Hello ");
		queue.enqueue("my ");
		queue.enqueue("name ");
		queue.enqueue("is ");
		std::cout << "Peeking Queue: " << queue.peek() << "\n";
		queue.enqueue("Bart");
		queue.dequeue();
		std::cout << "length = " << queue.length() << "\n";
		std::cout << "Peeking Queue: " << queue.peek() << "\n";
		queue.display();
		std::cout << queue.contains("is ") << "\n";
		std::cout << queue.length() << "\n\n";

		Stack<char> stack;
		stack.push('a');
		stack.push('n');
		stack.push('d');
		stack.push('k');
		stack.display();
		stack.pop();
		std::cout << stack.peek() << "\n";
		std::cout << stack.length() << "\n\n";

		BT::BinaryTree<int> tree = BT::BinaryTree(5);
		tree.new_left(10);
		tree.new_right(15);
		tree.advance_left();
		tree.new_left(20);
		tree.new_right(25);
		tree.goto_root();
		tree.advance_right();
		tree.new_left(30);
		tree.new_right(35);
		std::vector<int> data = tree.contents_InOrder();
		printvec(data);
		data = tree.contents_PostOrder();
		printvec(data);
		data = tree.contents_PreOrder();
		printvec(data);
		std::cout << tree.max_height() << std::endl;

		tree.clear();
		tree.change_data(10);
		std::cout << "\nTree height after clearing: " << tree.max_height();
		std::cout << "\n\n";

		GP::Graph<int> graph(5);
		graph.add_node(10);
		graph.add_node(15);
		graph.add_node(20);
		graph.add_node(30);
		graph.add_edge('A', 'B');
		graph.add_edge('B', 'C');
		graph.add_edge('A', 'D');
		graph.add_edge('A', 'C');
		graph.add_edge('C', 'D');
		graph.add_edge('B', 'D');
		graph.add_edge('A', 'E');
		graph.add_edge('E', 'B');
		graph.print();
		std::cout << "\n";
		graph.remove('C');
		graph.print();
		std::cout << "\n\n";

		Queue<int> PQueue(3, 1);
		PQueue.enqueue(15);
		PQueue.enqueue(12);
		PQueue.enqueue(20);
		PQueue.enqueue(7);
		PQueue.display();
		std::cout << "\n\n";

		Map::Map<int> map;
		map.add("A", 15);
		map.add("B", 25);
		map.add("C", 6);
		map.add("D", 53);
		map.add("E", 152);
		map.print();
		std::cout << "\n\n";

		BST::BinarySearchTree<double> bst(2.5);
		bst.add(1.9);
		bst.add(5.6);
		bst.add(8.3);
		bst.add(1.2);
		bst.add(12.9);
		bst.remove(5.6);
		std::vector<double> res = bst.contents_InOrder();
		printvec(res);
	}

	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}