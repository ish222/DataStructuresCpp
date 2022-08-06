#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	template<typename T>
	class Stack {
	public:
		Stack() noexcept : head(nullptr), mLength(0) {}

		explicit Stack(const T& data) noexcept : mLength(1) {
			head = new Node(data);
		}

		explicit Stack(T&& data) noexcept : mLength(1) {
			head = new Node(std::move(data));
		}

		Stack(std::initializer_list<T> init) noexcept : mLength(0) {
			for (auto it = init.begin(); it != init.end(); ++it)
				push(*it);
		}

		Stack(const Stack<T>& other) noexcept : mLength(other.mLength) {
			if (other.mLength) {
				head = new Node(other.head->data);
				Node* other_node = other.head->next;
				while (other_node) {
					push(other_node->data);
					other_node = other_node->next;
				}
				return;
			}
			head = nullptr;
		}

		Stack<T>& operator=(const Stack<T>& other) noexcept {
			if (this != &other) {
				if (mLength)
					clear();
				if (other.mLength) {
					head = new Node(other.head->data);
					mLength = 1;
					Node* other_node = other.head->next;
					while (other_node) {
						push(other_node->data);
						other_node = other_node->next;
					}
				} else {
					head = nullptr;
					mLength = 0;
				}
			}
			return *this;
		}

		Stack(Stack<T>&& other) noexcept : head(other.head), mLength(other.mLength) {
			other.head = nullptr;
			other.mLength = 0;
		}

		Stack<T>& operator=(Stack<T>&& other) noexcept {
			if (this != &other) {
				if (mLength)
					clear();
				head = other.head;
				mLength = other.mLength;
				other.head = nullptr;
				other.mLength = 0;
			}
			return *this;
		}

		void push(const T& data) noexcept {
			Node* new_node = new Node(data);
			if (mLength) {
				new_node->next = head;
				head = new_node;
				++mLength;
				return;
			}
			head = new_node;
			++mLength;
		}

		void push(T&& data) noexcept {
			Node* new_node = new Node(std::move(data));
			if (mLength) {
				new_node->next = head;
				head = new_node;
				++mLength;
				return;
			}
			head = new_node;
			++mLength;
		}

		void push(std::initializer_list<T> list) noexcept {
			for (auto it = list.begin(); it != list.end(); ++it)
				append(std::move(*it));
		}

		T pop() {
			if (mLength) {
				T result = head->data;
				Node* cur = head;
				head = head->next;
				delete cur;
				--mLength;
				return result;
			}
			throw std::invalid_argument("Stack is empty, there is nothing to pop.");
		}

		[[nodiscard]] T& peek() {
			if (mLength)
				return head->data;
			throw std::invalid_argument("Stack is empty, there is nothing to peek.");
		}

		[[nodiscard]] const T& peek() const {
			if (mLength)
				return head->data;
			throw std::invalid_argument("Stack is empty, there is nothing to peek.");
		}

		[[nodiscard]] int length() const noexcept {
			return mLength;
		}

		[[nodiscard]] bool empty() const noexcept {
			return mLength == 0;
		}

		explicit operator bool() const noexcept {
			return (bool)mLength;
		}

		bool operator==(const Stack<T>& other) const noexcept {
			if (mLength != other.mLength)
				return false;
			Node* cur = head;
			Node* other_cur = other.head;
			while (cur) {
				if (cur->data != other_cur->data)
					return false;
				cur = cur->next;
				other_cur = other_cur->next;
			}
			return true;
		}

		[[nodiscard]] Stack<T> operator+(Stack<T>& right) noexcept {
			if (right.mLength) {
				std::vector<T> right_data = right.contents();
				Stack<T> res(*this);
				for (const T& i: right_data)
					res.push(i);
				return res;
			}
			return *this;
		}

		[[nodiscard]] std::vector<T> contents() const noexcept {
			std::vector<T> elems(mLength);
			Node* cur_node = head;
			for (int i = 0; i < mLength; ++i) {
				elems[i] = cur_node->data;
				cur_node = cur_node->next;
			}
			return elems;
		}

		void display() const {
			if (mLength) {
				std::vector<T> data = contents();
				for (const T& i: data) {
					std::cout << i << "\t";
				}
				std::cout << "\n";
			} else
				throw std::runtime_error("Error: stack is empty, there is nothing to display");
		}

		void clear() noexcept {
			Node* cur_node = head;
			while (cur_node) {
				cur_node = cur_node->next;
				delete head;
				head = cur_node;
			}
			head = nullptr;
			mLength = 0;
		}

		virtual ~Stack() {
			if (mLength)
				clear();
		}

	private:
		struct Node {
			T data;
			Node* next = nullptr;

			explicit Node(const T& data) noexcept : data(data) {}

			explicit Node(T&& data) noexcept : data(std::move(data)) {}
		};

		Node* head;
		size_t mLength;
	};
}// namespace custom

#endif// STACK_H