//#include "Stack.h"
//
//template<typename T>
//Stack<T>::Stack() {
//	head = new Node<T>();
//	size = 0;
//}
//
//template<typename T>
//Stack<T>::Stack(const T& data) {
//	head = new Node<T>();
//	push(data);
//	size = 1;
//}
//
//template<typename T>
//void Stack<T>::push(const T& data) {
//	Node<T>* new_node = new Node<T>();
//	new_node->data = data;
//	new_node->next = head;
//	head = new_node;
//	size++;
//}
//
//template<typename T>
//bool Stack<T>::is_empty() {
//	return size == 0;
//}
//
//template<typename T>
//T Stack<T>::pop() {
//	if (is_empty()) {
//		throw std::invalid_argument("Stack is empty, there is nothing to pop.");
//	}
//	T result = head->data;
//	head = head->next;
//	size--;
//	return result;
//}
//
//template<typename T>
//T Stack<T>::peek() {
//	if (is_empty()) {
//		throw std::invalid_argument("Stack is empty, there is nothing to peek.");
//	}
//	return head->data;
//}
//
//template<typename T>
//int Stack<T>::length() {
//	return size;
//}
//
//template<typename T>
//std::vector<T> Stack<T>::contents() {
//	std::vector<T> elems;
//	Node<T> cur_node = *head;
//	while (cur_node.next != NULL) {
//		elems.push_back(cur_node.data);
//		cur_node = *(cur_node.next);
//	}
//	return elems;
//}
//
//template<typename T>
//void Stack<T>::display() {
//	std::vector<T> data = contents();
//	for (T& i : data) {
//		std::cout << i << "\n";
//	}
//}
