#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.h"

template<typename T>
class LinkedList {
public:
	LinkedList() : head(nullptr), tail(nullptr), mLength(0) {}

	explicit LinkedList(const T& data) {
		head = new Node<T>(data);
		tail = head;
        mLength = 1;
	}

    LinkedList(LinkedList<T>& other) {
        head = new Node<T>(other.head->data);
        mLength = other.mLength;
        tail = head;
        Node<T>* other_node = other.head->next;
        while (other_node) {
            tail->next = new Node<T>(other_node->data);
            tail = tail->next;
            other_node = other_node->next;
        }
    }

    LinkedList& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            if (mLength > 0)
                clear();
            head = new Node<T>(other.head->data);
            mLength = other.mLength;
            tail = head;
            Node<T>* other_node = other.head->next;
            while (other_node) {
                tail->next = new Node<T>(other_node->data);
                tail = tail->next;
                other_node = other_node->next;
            }
        }
        return *this;
    }

    LinkedList(LinkedList&& other) noexcept {
        head = other.head;
        tail = other.tail;
        mLength = other.mLength;
        other.head = nullptr;
        other.tail = nullptr;
        other.mLength = 0;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            if (mLength > 0)
                clear();
            head = other.head;
            tail = other.tail;
            mLength = other.mLength;
            other.head = nullptr;
            other.tail = nullptr;
            other.mLength = 0;
        }
        return *this;
    }

	void append(const T& data) {
		Node<T>* new_node = new Node<T>(data);
        ++mLength;
        if (head == nullptr) {
			head = new_node;
			tail = head;
			return;
		}
		tail->next = new_node;
		tail = new_node;
	}

    void push_back(const T& data) {
        append(data);
    }

	void insert(const T& data, const size_t& index) {
		if (index > mLength)
			throw std::invalid_argument("Invalid index");
		else if (head == nullptr)
			throw std::invalid_argument("Linked list is empty and uninitialised, use append instead");
		Node<T>* new_node = new Node<T>(data);
        ++mLength;
		if (index == 0) {
			new_node->next = head;
			head = new_node;
			return;
		}
		else if (index == mLength) {
			tail->next = new_node;
			tail = new_node;
			return;
		}
		size_t _index = 1;
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
			++_index;
		}
	}

    void push_front(const T& data) {
        insert(data, 0);
    }

	std::vector<T> contents() const {
		if (head == nullptr)
			throw std::runtime_error("Error: linked list is empty");
		std::vector<T> elems;
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			elems.push_back(cur_node->data);
			cur_node = cur_node->next;
		}
		return elems;
	}

	int find(const T& data) const {
		if (mLength == 0)
			throw std::runtime_error("Error: linkedlist is empty, there is no content to search");
		int index = 0;
		bool found = false;
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			if (cur_node->data == data) {
				found = true;
				break;
			}
			cur_node = cur_node->next;
			++index;
		}
		if (found)
			return index;
		else return -1;
	}

	void display() const {
		if (mLength == 0)
			throw std::runtime_error("Error: linked list is empty");
		std::vector<T> vals = contents();
		for (T& i : vals)
			std::cout << i << "\t";
		std::cout << "\n";
	}

	size_t length() const {
		return mLength;
	}

	operator bool() const noexcept {
		return (head != nullptr);
	}

    bool operator==(const LinkedList<T>& other) const {
        if (mLength != other.mLength)
            return false;
        Node<T>* cur = head;
        Node<T>* other_cur = other.head;
        while (cur) {
            if (cur->data != other_cur->data)
                return false;
            cur = cur->next;
            other_cur = other_cur->next;
        }
        return true;
    }

    bool operator!=(const LinkedList<T>& other) const {
        return !(*this == other);
    }

	void erase(const size_t& index) {
		if (index >= mLength)
			throw std::invalid_argument("Invalid index");
		if (index == 0) {
			Node<T>* head_cpy = head;
			head = head->next;
			delete head_cpy;
            --mLength;
			return;
		}
		size_t cur_index = 1;
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
                --mLength;
				return;
			}
			++cur_index;
		}
	}

	void clear() {
		if (mLength == 0)
			throw std::runtime_error("Error: linked list is empty and so cannot be cleared.");
		Node<T>* cur_node = head;
		while (cur_node != nullptr) {
			cur_node = cur_node->next;
			delete head;
			head = cur_node;
		}
		head = nullptr;
        tail = head;
        mLength = 0;
	}

	T get(const size_t& index) const {
		if (index >= mLength || index < 0)
			throw std::invalid_argument("Invalid index");
		else if (index == 0)
			return head->data;
		size_t cur_index = 1;
		Node<T>* cur_node = head;
		while (1) {
			Node<T>* last_node = cur_node;
			cur_node = cur_node->next;
			if (cur_index == index)
				return cur_node->data;
			++cur_index;
		}
	}

    T front() const {
        if (head == nullptr)
            throw std::runtime_error("List is empty, there is nothing at front");
        return head->data;
    }

    T back() const {
        if (tail == nullptr)
            throw std::runtime_error("List is empty, there is nothing at back");
        return tail->data;
    }

    void pop_front() {
        if (head == nullptr)
            throw std::runtime_error("List is empty, there is nothing to pop front");
        erase(0);
    }

    void pop_back() {
        if (tail == nullptr)
            throw std::runtime_error("List is empty, there is nothing to pop back");
        erase(length()-1);
    }

	void reverse_order() {
		if (mLength == 0)
			throw std::runtime_error("Error: linked list is empty and so cannot be reversed");
		Node<T>* cur_node = head;
		tail = head;
		Node<T>* last = nullptr;
		Node<T>* next = nullptr;
		while (cur_node != nullptr) {
			next = cur_node->next;
			cur_node->next = last;
			last = cur_node;
			cur_node = next;
		}
		head = last;
	}

	T operator[](const size_t& index) const {
		if (index >= mLength)
			throw std::invalid_argument("Invalid index");
		return get(index);
	}

	LinkedList<T> operator+(LinkedList<T>& right) {
		std::vector<T> data = contents();
		std::vector<T> right_data = right.contents();
		for (T& i : right_data)
			data.push_back(i);
		LinkedList<T>* res = new LinkedList<T>();
		for (const T& i : data)
			res->append(i);
		return *res;
	}

	~LinkedList() {
		if (head != nullptr)
			clear();
	}

private:
	Node<T>* head;
	Node<T>* tail;
    size_t mLength;
};
