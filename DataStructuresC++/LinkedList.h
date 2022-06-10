#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

template<typename T>
class LinkedList {
public:
	LinkedList() {
		head = nullptr;
		tail = head;
	}

	LinkedList(const T& data) {
		head = new Node<T>(data);
		tail = head;
	}

	void append(const T& data) {
		Node<T>* new_node = new Node<T>(data);
		if (head == nullptr) {
			head = new_node;
			tail = head;
			return;
		}
		tail->next = new_node;
		tail = new_node;
	}

	void insert(const T& data, const int& index) {
		if (index > length() || index < 0) {
			throw std::invalid_argument("Invalid index");
			return;
		}
		else if (head == nullptr) {
			throw std::invalid_argument("Linked list is empty and uninitialised, use append instead");
			return;
		}
		Node<T>* new_node = new Node<T>(data);
		if (index == 0) {
			new_node->next = head;
			head = new_node;
			return;
		}
		else if (index == length()) {
			tail->next = new_node;
			tail = new_node;
			return;
		}
		int _index = 1;
		Node<T>* cur_node = head;
		Node<T>* last_node = nullptr;
		while (1) {
			last_node = cur_node;
			cur_node = cur_node->next;
			if (_index == index) {
				last_node->next = new_node;
				new_node->next = cur_node;
				return;
			}
			_index++;
		}
	}

	std::vector<T> contents() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: linked list is empty");
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
		if (length() == 0) {
			throw std::runtime_error("Error: linkedlist is empty, there is no content to search");
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
		if (found)
			return index;
		else return -1;
	}

	void display() const {
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty");
			return;
		}
		std::vector<T> vals = contents();
		for (T& i : vals) {
			std::cout << i << "\t";
		}
		std::cout << "\n";
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

	void erase(const int& index) {
		if (index >= length() || index < 0) {
			throw std::invalid_argument("Invalid index");
			return;
		}
		if (index == 0) {
			Node<T>* head_cpy = head;
			head = head->next;
			delete head_cpy;
			return;
		}
		int cur_index = 1;
		Node<T>* cur_node = head;
		while (1) {
			Node<T>* last_node = cur_node;
			cur_node = cur_node->next;
			if (cur_index == index) {
				last_node->next = cur_node->next;
				if (last_node->next == nullptr) {
					tail = last_node;
				}
				delete cur_node;
				return;
			}
			cur_index++;
		}
	}

	void clear() {
		if (length() == 0) {
			throw std::runtime_error("Error: linked list is empty and so cannot be cleared.");
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

	T get(const int& index) const {
		if (index >= length() || index < 0) {
			throw std::invalid_argument("Invalid index");
			return T();
		}
		else if (index == 0)
			return head->data;
		int cur_index = 1;
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
		Node<T>* cur_node = head;
		tail = head;
		Node<T>* last = nullptr;
		Node<T>* next = nullptr;
		while (cur_node != nullptr) {
			next = cur_node->next;
			cur_node->next = last;
			last = cur_node;
			cur_node = next;
		}
		head = last;
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
		if (head != nullptr)
			clear();
	}

private:
	Node<T>* head;
	Node<T>* tail;
};
