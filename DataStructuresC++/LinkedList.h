#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

class LinkedList {
public:
	LinkedList();
	LinkedList(const int&);

	void append(const int&);
	std::vector<int> contents();
	void print_contents();
	int& length();
	void erase(const int&);
	int get(const int&);

	int& operator[](const int&);
	LinkedList& operator+(LinkedList&);

private:
	Node* head;
	Node* tail;
};