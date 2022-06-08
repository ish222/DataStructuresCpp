#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <functional>

#include "Node.h"

template<typename T>
class Queue
{
public:
	Queue() {
		head = new Node<T>();
		tail = head;
		this->priority_val = None;
	}

	Queue(const T& data, int priority = None) {
		head = new Node<T>();
		tail = head;
		this->priority_val = priority;
		enqueue(data);
	}

	void enqueue(const T& data) {
		if (priority_val == None || head->next == nullptr || head == nullptr) {
			Node<T>* new_node = new Node<T>();
			new_node->data = data;
			tail->next = new_node;
			tail = new_node;
			return;
		}
		else {
			Node<T>* cur_node = head->next;
			int index = 0;
			switch (priority_val) {
			case Ascending:
				while (cur_node->data < data && cur_node->next != nullptr) {
					cur_node = cur_node->next;
					index++;
				}
				insert(data, index);
				break;

			case Descending:
				while (cur_node->data > data && cur_node->next != nullptr) {
					cur_node = cur_node->next;
					index++;
				}
				insert(data, index);
				break;
			}
		}
	}

	T dequeue() {
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty, there is nothing to dequeue");
			return T();
		}
		Node<T>* first = (head->next);
		head->next = first->next;
		T data = first->data;
		delete first;
		return data;
	}

	T peek() const {
		Node<T> first = *(head->next);
		return first.data;
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

	bool contains(const T& data) const {
		Node<T> cur_node = *head;
		while (cur_node.next != nullptr) {
			if (cur_node.data == data)
				return true;
			else
				cur_node = *(cur_node.next);
		}
		return false;
	}

	int find(const T& data) const {
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty, there is no content to search");
			return -1;
		}
		int index = 0;
		bool found = false;
		Node<T> cur_node = *head;
		while (cur_node.next != nullptr) {
			cur_node = *(cur_node.next);
			if (cur_node.data == data) {
				found = true;
				break;
			}
			index++;
		}
		if (found == true)
			return index;
		else return -1;
	}

	void insert(const T& data,  const int& index) {
		if(index > length() || index < 0)
			throw std::invalid_argument("Invalid index");
		Node<T>* new_node = new Node<T>();
		new_node->data = data;
		if (head->next == nullptr && index == 0) {
			head->next = new_node;
			tail = new_node;
			return;
		}
		int _index = 0;
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
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty, there is no content");
			return std::vector<T>();
		}
		std::vector<T> elems;
		Node<T> cur_node = *head;
		while (cur_node.next != nullptr) {
			cur_node = *(cur_node.next);
			elems.push_back(cur_node.data);
		}
		return elems;
	}

	void display() const {
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty, there is nothing to display");
			return;
		}
		std::vector<T> data = contents();
		for (T& i : data) {
			std::cout << i << "\n";
		}
	}

	void clear(bool destroy = false) {
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty and so cannot be cleared");
			return;
		}
		Node<T>* cur_node = head;
		Node<T>* mext = nullptr;
		while (cur_node->next != nullptr) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}
		if (destroy == false) {
			this->head = new Node<T>();
			this->tail = head;
		}
	}

	operator bool() const {
		return (length() != 0);
	}

	virtual ~Queue() {
		if (length() != 0)
			clear(true);
		else delete head;
	}

private:
	Node<T>* head;
	Node<T>* tail;
	int priority_val;
	enum Priority {
		None,
		Ascending,
		Descending
	};
};

