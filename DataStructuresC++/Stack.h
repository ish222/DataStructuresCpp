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
		Node<T>* cur = head;
		head = head->next;
		delete cur;
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
		while (cur_node.next != nullptr) {
			total++;
			cur_node = *(cur_node.next);
		}
		return total;
	}

	operator bool() const {
		return (length() != 0);
	}

	std::vector<T> contents() const {
		if (length() == 0) {
			throw std::runtime_error("Error: stack is empty, there is no content");
			return std::vector<T>();
		}
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != nullptr) {
			elems.push_back(cur_node.data);
			cur_node = *(cur_node.next);
		}
		return elems;
	}

	int find(const T& data) const {
		if (length() == 0) {
			throw std::runtime_error("Error: stack is empty, there is no content to search");
			return -1;
		}
		int index = 0;
		bool found = false;
		Node<T> cur_node = *head;
		while (cur_node.next != nullptr) {
			if (cur_node.data == data) {
				found = true;
				break;
			}
			index++;
			cur_node = *(cur_node.next);
		}
		if (found == true)
			return index;
		else return -1;
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
		Node<T>* mext = nullptr;
		while (cur_node->next != nullptr) {
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

