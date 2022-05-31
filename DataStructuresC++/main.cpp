#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "BinaryTree.h"


int main() {
	//LinkedList<float> list;
	//list.append(2.31f);
	//list.append(3.13f);
	//list.append(5.87f);
	//list.append(15.55f);
	//list.append(45.90f);
	//list.display();
	//std::cout << list.length() << std::endl;
	//list.erase(2);
	//list.display();
	//std::cout << list[0] <<"\n";

	//LinkedList<float> list2;
	//list2.append(1.90f);
	//list2.append(2.90f);
	//list2.append(3.90f);
	//list2.display();
	//std::cout << "\n";

	//LinkedList list3 = list + list2;
	//list3.display();
	//std::cout << "\n\n";

	//Queue<std::string> queue;
	//queue.enqueue("Hello ");
	//queue.enqueue("my ");
	//queue.enqueue("name ");
	//queue.enqueue("is ");
	//std::cout << queue.peek() << "\n";
	//queue.enqueue("Bart");
	//queue.dequeue();
	//std::cout << queue.peek() << "\n";
	//queue.display();
	//std::cout << queue.contains("is ") << "\n";
	//std::cout << queue.length() << "\n\n";

	//Stack<char> stack;
	//stack.push('a');
	//stack.push('n');
	//stack.push('d');
	//stack.push('k');
	//stack.display();
	//stack.pop();
	//std::cout << stack.peek() << "\n";
	//std::cout << stack.length() << "\n";

	BT::BinaryTree tree = BT::BinaryTree(5);
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
	for (int& i : data) {
		std::cout << i << " ";
	}
	
}