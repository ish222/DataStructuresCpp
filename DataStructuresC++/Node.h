#pragma once

template<typename T>
struct Node {
	T data;
	Node<T>* next = nullptr;

	Node(T data) {
		this->data = data;
	}
};