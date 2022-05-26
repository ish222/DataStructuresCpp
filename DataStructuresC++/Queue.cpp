//#include "Queue.h"
//
//Queue::Queue() {
//	head = new Node();
//	tail = head;
//}
//
//Queue::Queue(const int& data) {
//	head = new Node();
//	tail = head;
//	//enqueue(data);
//}
//
//void Queue::enqueue(const int& data) {
//	Node* new_node = new Node();
//	new_node->data = data;
//	tail->next = new_node;
//	tail = new_node;
//}
//
//int& Queue::dequeue() {
//	Node first = *(head->next);
//	head->next = first.next;
//	return first.data;
//}
//
//int& Queue::peek() {
//	Node first = *(head->next);
//	return first.data;
//}
//
//int& Queue::length() {
//	Node cur_node = *head;
//	int total = 0;
//	while (cur_node.next != NULL) {
//		total++;
//		cur_node = *(cur_node.next);
//	}
//	return total;
//}
//
//bool Queue::contains(const int& data) {
//	Node cur_node = *head;
//	while (cur_node.next != NULL) {
//		if (cur_node.data == data)
//			return true;
//		else
//			cur_node = *(cur_node.next);
//	}
//	return false;
//}
//
//std::vector<int> Queue::contents() {
//	std::vector<int> elems;
//	Node cur_node = *head;
//	while (cur_node.next != NULL) {
//		cur_node = *(cur_node.next);
//		elems.push_back(cur_node.data);
//	}
//	return elems;
//}
//
//void Queue::display() {
//	std::vector<int> data = contents();
//	for (int& i : data) {
//		std::cout << i << "\n";
//	}
//}
