#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

class Stack
{
public:
	Stack();
	Stack(const int&);

	void push(const int&);
	bool is_empty();
	int pop();
	int peek();
	int length();
	std::vector<int> contents();
	void display();

private:
	int size;
	Node* head;
};

