#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

template<typename T>
class Stack
{
public:
	Stack() {
		head = new Node<T>();
	}

	Stack(const T& data) {
		head = new Node<T>();
		push(data);
	}

	void push(const T& data) {
		Node<T>* new_node = new Node<T>();
		new_node->data = data;
		new_node->next = head;
		head = new_node;
	}

	bool is_empty() const {
		return length() == 0;
	}

	T pop() {
		if (is_empty()) {
			throw std::invalid_argument("Stack is empty, there is nothing to pop.");
			return T();
		}
		T result = head->data;
		head = head->next;
		return result;
	}

	T peek() const {
		if (is_empty()) {
			throw std::invalid_argument("Stack is empty, there is nothing to peek.");
			return T();
		}
		return head->data;
	}

	int length() const {
		Node<T> cur_node = *head;
		int total = 0;
		while (cur_node.next != NULL) {
			total++;
			cur_node = *(cur_node.next);
		}
		return total;
	}

	std::vector<T> contents() const {
		if (length() == 0) {
			throw std::runtime_error("Error: stack is empty, there is no content");
			return std::vector<T>();
		}
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != NULL) {
			elems.push_back(cur_node.data);
			cur_node = *(cur_node.next);
		}
		return elems;
	}

	void display() const {
		if (length() == 0) {
			throw std::runtime_error("Error: stack is empty, there is nothing to display");
			return;
		}
		std::vector<T> data = contents();
		for (T& i : data) {
			std::cout << i << "\n";
		}
	}

	void clear(bool destroy = false) {
		if (length() == 0) {
			throw std::runtime_error("Error: stack is empty and so cannot be cleared");
			return;
		}
		Node<T>* cur_node = head;
		Node<T>* mext = NULL;
		while (cur_node->next != NULL) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}

		if (destroy == false)
			this->head = new Node<T>();
	}

	~Stack() {
		if (length() != 0)
			clear(true);
		else delete head;

	}

private:
	Node<T>* head;
};

