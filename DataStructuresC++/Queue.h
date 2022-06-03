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
		head = new Node<T>();
		tail = head;
	}

	Queue(const T& data) {
		head = new Node<T>();
		tail = head;
		enqueue(data);
	}

	void enqueue(const T& data) {
		Node<T>* new_node = new Node<T>();
		new_node->data = data;
		tail->next = new_node;
		tail = new_node;
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
		while (cur_node.next != NULL) {
			total++;
			cur_node = *(cur_node.next);
		}
		return total;
	}

	bool contains(const T& data) const {
		Node<T> cur_node = *head;
		while (cur_node.next != NULL) {
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
		while (cur_node.next != NULL) {
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

	std::vector<T> contents() const {
		if (length() == 0) {
			throw std::runtime_error("Error: queue is empty, there is no content");
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
		Node<T>* mext = NULL;
		while (cur_node->next != NULL) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}
		if (destroy == false) {
			this->head = new Node<T>();
			this->tail = head;
		}
	}

	~Queue() {
		if (length() != 0)
			clear(true);
		else delete head;
	}

private:
	Node<T>* head;
	Node<T>* tail;
};

