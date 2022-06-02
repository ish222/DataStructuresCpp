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
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty");
			return std::vector<T>();
		}
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != NULL) {
			cur_node = *(cur_node.next);
			elems.push_back(cur_node.data);
		}
		return elems;
	}

	void display() const {
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty");
			return;
		}
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
			return;
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

	void clear(bool destroy = false) {
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty and so cannot be cleared.");
			return;
		}
		Node<T>* cur_node = head;
		while (cur_node != NULL) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}
		if (destroy == false) {
			this->head = new Node<T>();
			this->tail = head;
		}
	}

	T get(const int& index) const {
		if (index >= length()) {
			throw std::invalid_argument("Invalid index");
			return T();
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

	void reverse_order() {
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty and so cannot be reversed");
			return;
		}
		Node<T>* cur_node = head->next;
		tail = head->next;
		Node<T>* last = NULL;
		Node<T>* next = NULL;
		while (cur_node != NULL) {
			next = cur_node->next;
			cur_node->next = last;
			last = cur_node;
			cur_node = next;
		}
		head->next = last;
	}

	T operator[](const int& index) const {
		if (index >= length()) {
			throw std::invalid_argument("Invalid index");
			return T();
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
		LinkedList<T>* res = new LinkedList<T>();
		for (T& i : data) {
			res->append(i);
		}
		return *res;
	}

	~LinkedList() {
		if (length() != 0)
			clear(true);
		else delete head;
	}

private:
	Node<T>* head;
	Node<T>* tail;
};
