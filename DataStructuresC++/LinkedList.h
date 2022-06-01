#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

template<typename T>
class LinkedList {
public:
	LinkedList() {
		head = new Node<T>();
		tail = head;
	}

	LinkedList(const T& data) {
		head = new Node<T>();
		tail = head;
		append(data);
	}

	void append(const T& data) {
		Node<T>* new_node = new Node<T>();
		new_node->data = data;
		tail->next = new_node;
		tail = new_node;
	}

	std::vector<T> contents() const {
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != NULL) {
			cur_node = *(cur_node.next);
			elems.push_back(cur_node.data);
		}
		return elems;
	}

	void display() const {
		std::vector<T> vals = contents();
		for (T& i : vals) {
			std::cout << i << "\n";
		}
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

	void erase(const int& index) {
		if (index >= length()) {
			throw std::invalid_argument("Invalid index");
		}
		int cur_index = 0;
		Node<T>* cur_node = head;
		while (1) {
			Node<T>* last_node = cur_node;
			cur_node = cur_node->next;
			if (cur_index == index) {
				last_node->next = cur_node->next;
				if (last_node->next == NULL) {
					tail = last_node;
				}
				delete cur_node;
				return;
			}
			cur_index++;
		}
	}

	T get(const int& index) const {
		if (index >= length()) {
			throw std::invalid_argument("Invalid index");
		}
		int cur_index = 0;
		Node<T>* cur_node = head;
		while (1) {
			Node<T>* last_node = cur_node;
			cur_node = cur_node->next;
			if (cur_index == index) {
				return cur_node->data;
			}
			cur_index++;
		}
	}

	T operator[](const int& index) const {
		if (index >= length()) {
			throw std::invalid_argument("Invalid index");
		}
		T ret = get(index);
		return ret;
	}

	LinkedList<T> operator+(LinkedList<T>& right) {
		std::vector<T> data = contents();
		std::vector<T> right_data = right.contents();
		for (T& i : right_data) {
			data.push_back(i);
		}
		LinkedList<T> res;
		for (T& i : data) {
			res.append(i);
		}
		return res;
	}

private:
	Node<T>* head;
	Node<T>* tail;
};
