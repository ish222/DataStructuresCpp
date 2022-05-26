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
		size = 0;
	}

	Stack(const T& data) {
		head = new Node<T>();
		push(data);
		size = 1;
	}

	void push(const T& data) {
		Node<T>* new_node = new Node<T>();
		new_node->data = data;
		new_node->next = head;
		head = new_node;
		size++;
	}

	bool is_empty() {
		return size == 0;
	}

	T pop() {
		if (is_empty()) {
			throw std::invalid_argument("Stack is empty, there is nothing to pop.");
		}
		T result = head->data;
		head = head->next;
		size--;
		return result;
	}

	T peek() {
		if (is_empty()) {
			throw std::invalid_argument("Stack is empty, there is nothing to peek.");
		}
		return head->data;
	}

	int length() {
		return size;
	}

	std::vector<T> contents() {
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != NULL) {
			elems.push_back(cur_node.data);
			cur_node = *(cur_node.next);
		}
		return elems;
	}

	void display() {
		std::vector<T> data = contents();
		for (T& i : data) {
			std::cout << i << "\n";
		}
	}

private:
	int size;
	Node<T>* head;
};

