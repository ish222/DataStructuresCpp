#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

template<typename T>
class Queue
{
public:
	Queue() {
		head = nullptr;
		tail = head;
		this->priority_val = None;
	}

	Queue(const T& data, int priority = None) {
		head = new Node<T>(data);
		tail = head;
		this->priority_val = priority;
	}

	void enqueue(const T& data) {
		if (head == nullptr) {
			Node<T>* new_node = new Node<T>(data);
			head = new_node;
			tail = head;
			return;
		}
		else if (priority_val == None || (priority_val == None && head == tail)) {
			Node<T>* new_node = new Node<T>(data);
			tail->next = new_node;
			tail = new_node;
			return;
		}
		else {
			Node<T>* cur_node = head;
			int index = 0;
			switch (priority_val) {
			case Ascending:
				while (cur_node != nullptr && cur_node->data <= data) {
					cur_node = cur_node->next;
					index++;
				}
				insert(data, index);
				break;

			case Descending:
				while (cur_node != nullptr && cur_node->data >= data) {
					cur_node = cur_node->next;
					index++;
				}
				insert(data, index);
				break;
			}
		}
	}

	T dequeue() {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty, there is nothing to dequeue");
			return T();
		}
		Node<T>* first = head;
		head = head->next;
		T data = first->data;
		delete first;
		return data;
	}

	T peek() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty, there is nothing to peek");
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

	bool contains(const T& data) const {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty");
			return false;
		}
		Node<T>* cur_node = head;
		while (cur_node->next != nullptr) {
			if (cur_node->data == data)
				return true;
			cur_node = cur_node->next;
		}
		return false;
	}

	int find(const T& data) const {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty, there is no content to search");
			return -1;
		}
		int index = 0;
		bool found = false;
		Node<T>* cur_node = head;
		while (cur_node->next != nullptr) {
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

	void insert(const T& data,  const int& index) {
		if (index > length() || index < 0) {
			throw std::invalid_argument("Invalid index");
			return;
		}
		else if (head == nullptr) {
			throw std::invalid_argument("Queue is empty and uninitialised, use enqueue instead");
			return;
		}
		Node<T>* new_node = new Node<T>(data);
		if (index == 0) {
			new_node->next = head;
			if (head == tail)
				tail = new_node->next;
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

	std::vector<T> contents() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty, there is no content");
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

	void display() const {
		if (head == nullptr) {
			throw std::runtime_error("Error: queue is empty, there is nothing to display");
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
			throw std::runtime_error("Error: queue is empty and so cannot be cleared");
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

	operator bool() const {
		return (head != nullptr);
	}

	virtual ~Queue() {
		if (length() != 0)
			clear();
	}

private:
	Node<T>* head;
	Node<T>* tail;
	int priority_val;
	enum Priority {
		None=0,
		Ascending,
		Descending
	};
};

