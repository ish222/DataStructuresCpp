#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

class Queue
{
public:
	Queue();
	Queue(const int&);

	void enqueue(const int& data);
	int& dequeue();
	int& peek();
	int& length();
	bool contains(const int& data);
	std::vector<int> contents();
	void display();

private:
	Node* head;
	Node* tail;
};

