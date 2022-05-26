//#include "LinkedList.h"
//
//LinkedList::LinkedList() {
//	head = new Node();
//	tail = head;
//}
//
//LinkedList::LinkedList(const int& data) {
//	head = new Node();
//	tail = head;
//	append(data);
//}
//
//void LinkedList::append(const int& data) {
//	Node* new_node = new Node();
//	new_node->data = data;
//	tail->next = new_node;
//	tail = new_node;
//}
//
//std::vector<int> LinkedList::contents() {
//	std::vector<int> elems;
//	Node cur_node = *head;
//	while (cur_node.next != NULL) {
//		cur_node = *(cur_node.next);
//		elems.push_back(cur_node.data);
//	}
//	return elems;
//}
//
//void LinkedList::print_contents() {
//	std::vector<int> vals = contents();
//	for (int& i : vals) {
//		std::cout << i << "\n";
//	}
//}
//
//int& LinkedList::length() {
//	Node cur_node = *head;
//	int total = 0;
//	while (cur_node.next != NULL) {
//		total++;
//		cur_node = *(cur_node.next);
//	}
//	return total;
//}
//
//void LinkedList::erase(const int& index) {
//	if (index >= this->length()) {
//		throw std::invalid_argument("Invalid index");
//	}
//	int cur_index = 0;
//	Node* cur_node = head;
//	while (1) {
//		Node* last_node = cur_node;
//		cur_node = cur_node->next;
//		if (cur_index == index) {
//			last_node->next = cur_node->next;
//			if (last_node->next == NULL) {
//				tail = last_node;
//			}
//			delete cur_node;
//			return;
//		}
//		cur_index++;
//	}
//}
//
//int LinkedList::get(const int& index) {
//	if (index >= this->length()) {
//		throw std::invalid_argument("Invalid index");
//	}
//	int cur_index = 0;
//	Node* cur_node = head;
//	while (1) {
//		Node* last_node = cur_node;
//		cur_node = cur_node->next;
//		if (cur_index == index) {
//			return cur_node->data;
//		}
//		cur_index++;
//	}
//}
//
//int& LinkedList::operator[](const int& index) {
//	if (index >= this->length()) {
//		throw std::invalid_argument("Invalid index");
//	}
//	int ret = get(index);
//	return ret;
//}
//
//LinkedList& LinkedList::operator+(LinkedList& right) {
//	std::vector<int> data = contents();
//	std::vector<int> right_data = right.contents();
//	for (int& i : right_data) {
//		data.push_back(i);
//	}
//	LinkedList res;
//	for (int& i : data) {
//		res.append(i);
//	}
//	return res;
//}
