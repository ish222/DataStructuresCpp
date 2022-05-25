#include "Stack.h"

Stack::Stack() {
	head = new Node();
	size = 0;
}

Stack::Stack(const int& data) {
	head = new Node();
	push(data);
	size = 1;
}

void Stack::push(const int& data) {
	Node* new_node = new Node();
	new_node->data = data;
	new_node->next = head;
	head = new_node;
	size++;
}

bool Stack::is_empty() {
	return size == 0;
}

int Stack::pop() {
	if (is_empty()) {
		throw std::invalid_argument("Stack is empty, there is nothing to pop.");
	}
	int result = head->data;
	head = head->next;
	size--;
	return result;
}

int Stack::peek() {
	if (is_empty()) {
		throw std::invalid_argument("Stack is empty, there is nothing to peek.");
	}
	return head->data;
}

int Stack::length() {
	return size;
}

std::vector<int> Stack::contents() {
	std::vector<int> elems;
	Node cur_node = *head;
	while (cur_node.next != NULL) {
		elems.push_back(cur_node.data);
		cur_node = *(cur_node.next);
	}
	return elems;
}

void Stack::display() {
	std::vector<int> data = contents();
	for (int& i : data) {
		std::cout << i << "\n";
	}
}
