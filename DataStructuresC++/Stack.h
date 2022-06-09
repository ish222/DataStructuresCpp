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
		head = nullptr;
	}

	Stack(const T& data) {
		head = new Node<T>(data);
	}

	void push(const T& data) {
		Node<T>* new_node = new Node<T>(data);
		if (head == nullptr) {
			head = new_node;
			return;
		}
		new_node->next = head;
		head = new_node;
	}

	bool is_empty() const {
		return length() == 0;
	}

	T pop() {
		if (head == nullptr) {
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
		if (head == nullptr) {
			throw std::invalid_argument("Stack is empty, there is nothing to peek.");
			return T();
		}
		return head->data;
	}

	int length() const {
		if (head == nullptr)
			return 0;
		Node<T>* cur_node = head;
		int total = 0;
		while (cur_node != nullptr) {
			cur_node = cur_node->next;
			total++;
		}
		return total;
	}

	operator bool() const {
		return (head != nullptr);
	}

	std::vector<T> contents() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: stack is empty, there is no content");
			return std::vector<T>();
		}
		std::vector<T> elems;
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			elems.push_back(cur_node->data);
			cur_node = cur_node->next;
		}
		return elems;
	}

	int find(const T& data) const {
		if (head == nullptr) {
			throw std::runtime_error("Error: stack is empty, there is no content to search");
			return -1;
		}
		int index = 0;
		bool found = false;
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			if (cur_node->data == data) {
				found = true;
				break;
			}
			cur_node = cur_node->next;
			index++;
		}
		if (found == true)
			return index;
		else return -1;
	}

	void display() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: stack is empty, there is nothing to display");
			return;
		}
		std::vector<T> data = contents();
		for (T& i : data) {
			std::cout << i << "\t";
		}
		std::cout << "\n";
	}

	void clear() {
		if (head == nullptr) {
			throw std::runtime_error("Error: stack is empty and so cannot be cleared");
			return;
		}
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}
		head = nullptr;
	}

	~Stack() {
		if (length() != 0)
			clear();
	}

private:
	Node<T>* head;
};

