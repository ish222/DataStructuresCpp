#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <initializer_list>

namespace custom {
	template<typename T>
	class Queue {
	public:
		Queue() noexcept: head(nullptr), tail(nullptr), mLength(0) {}

		explicit Queue(const T& data) : mLength(1) {
			head = new Node(data);
			tail = head;
		}

		explicit Queue(T&& data) : mLength(1) {
			head = new Node(std::move(data));
			tail = head;
		}

		Queue(std::initializer_list<T> init) : mLength(0) {
			for (auto it = init.begin(); it != init.end(); ++it)
				enqueue(*it);
		}

		Queue(const Queue<T>& other) : mLength(other.mLength) {
			head = new Node(other.head->data);
			tail = head;
			Node* other_node = other.head->next;
			while (other_node) {
				tail->next = new Node(other_node->data);
				tail = tail->next;
				other_node = other_node->next;
			}
		}

		Queue<T>& operator=(const Queue<T>& other) {
			if (this != &other) {
				if (mLength > 0)
					clear();
				if (other.mLength) {
					head = new Node(other.head->data);
					mLength = other.mLength;
					tail = head;
					Node* other_node = other.head->next;
					while (other_node) {
						tail->next = new Node(other_node->data);
						tail = tail->next;
						other_node = other_node->next;
					}
				} else {
					head = nullptr;
					tail = head;
					mLength = 0;
				}
			}
			return *this;
		}

		Queue(Queue<T>&& other) noexcept: head(other.head), tail(other.tail), mLength(other.mLength) {
			other.head = nullptr;
			other.tail = nullptr;
			other.mLength = 0;
		}

		Queue<T>& operator=(Queue<T>&& other) noexcept {
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

		virtual void enqueue(const T& data) {
			Node* new_node = new Node(data);
			if (mLength) {
				tail->next = new_node;
				tail = new_node;
				++mLength;
				return;
			}
			head = new_node;
			tail = head;
			mLength = 1;
		}

		virtual void enqueue(T&& data) {
			Node* new_node = new Node(std::move(data));
			if (mLength) {
				tail->next = new_node;
				tail = new_node;
				++mLength;
				return;
			}
			head = new_node;
			tail = head;
			mLength = 1;
		}

		T dequeue() {
			if (mLength) {
				Node* first = head;
				head = head->next;
				T data = first->data;
				delete first;
				--mLength;
				return data;
			}
			throw std::runtime_error("Error: queue is empty, there is nothing to dequeue");
		}

		T& peek() {
			if (mLength)
				return head->data;
			throw std::runtime_error("Error: queue is empty, there is nothing to peek");
		}

		const T& peek() const {
			if (mLength)
				return head->data;
			throw std::runtime_error("Error: queue is empty, there is nothing to peek");
		}

		int length() const noexcept {
			return mLength;
		}

		bool empty() const noexcept {
			return mLength == 0;
		}

		explicit operator bool() const noexcept {
			return (bool)mLength;
		}

		virtual bool operator==(const Queue<T>& other) const {
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

		bool contains(const T& data) const {
			if (mLength) {
				Node* cur_node = head;
				while (cur_node) {
					if (cur_node->data == data)
						return true;
					cur_node = cur_node->next;
				}
				return false;
			}
			throw std::runtime_error("Error: queue is empty");
		}

		Queue<T> operator+(Queue<T>& right) {
			if (right.mLength) {
				std::vector<T> data = contents();
				std::vector<T> right_data = right.contents();
				for (T& i: right_data)
					data.push_back(i);
				Queue<T>* res = new Queue<T>();
				for (const T& i: data)
					res->enqueue(i);
				return *res;
			}
			return *this;
		}

		std::vector<T> contents() const {
			if (mLength) {
				std::vector<T> elems(mLength);
				Node* cur_node = head;
				for (size_t i = 0; i < mLength; ++i) {
					elems[i] = cur_node->data;
					cur_node = cur_node->next;
				}
				return elems;
			}
			throw std::runtime_error("Error: queue is empty, there is no content");
		}

		void display() const {
			if (mLength) {
				std::vector<T> data = contents();
				for (const T& i: data) {
					std::cout << i << "\t";
				}
				std::cout << "\n";
			} else throw std::runtime_error("Error: queue is empty, there is nothing to display");
		}

		void clear() {
			if (mLength) {
				Node* cur_node = head;
				while (cur_node) {
					cur_node = cur_node->next;
					delete head;
					head = cur_node;
				}
				head = nullptr;
			} else throw std::runtime_error("Error: queue is empty and so cannot be cleared");
		}

		virtual ~Queue() {
			if (mLength)
				clear();
		}

	protected:
		struct Node {
			T data;
			Node* next = nullptr;

			explicit Node(const T& data) : data(data) {}

			explicit Node(T&& data) : data(std::move(data)) {}
		};

		Node* head;
		Node* tail;
		size_t mLength;
	};

	template<typename T>
	class PriorityQueue : public Queue<T> {
	public:
		PriorityQueue() : priority_val(None), Queue<T>() {}

		explicit PriorityQueue(const T& data, unsigned int priority = None) : priority_val(priority), Queue<T>(data) {}

		explicit PriorityQueue(T&& data, unsigned int priority = None) : priority_val(priority),
		                                                                 Queue<T>(std::move(data)) {}

		PriorityQueue(const PriorityQueue<T>& other) : Queue<T>(other), priority_val(other.priority_val) {}

		PriorityQueue(const Queue<T>& other) : Queue<T>(other), priority_val(None) {}

		PriorityQueue<T>& operator=(const PriorityQueue<T>& other) {
			if (this != &other) {
				if (mLength > 0)
					clear();
				if (other.mLength) {
					head = new Node(other.head->data);
					mLength = other.mLength;
					priority_val = other.priority_val;
					tail = head;
					Node* other_node = other.head->next;
					while (other_node) {
						tail->next = new Node(other_node->data);
						tail = tail->next;
						other_node = other_node->next;
					}
				} else {
					head = nullptr;
					tail = head;
					mLength = 0;
				}
			}
			return *this;
		}

		PriorityQueue(PriorityQueue<T>&& other) noexcept: Queue<T>(std::move(other)),
		                                                  priority_val(other.priority_val) {}

		PriorityQueue(Queue<T>&& other) noexcept: Queue<T>(std::move(other)), priority_val(None) {}

		PriorityQueue<T>& operator=(PriorityQueue<T>&& other) noexcept {
			if (this != &other) {
				if (mLength > 0)
					clear();
				head = other.head;
				tail = other.tail;
				mLength = other.mLength;
				priority_val = other.priority_val;
				other.head = nullptr;
				other.tail = nullptr;
				other.mLength = 0;
				other.priority_val = None;
			}
			return *this;
		}

		void enqueue(const T& data) override {
			Node* new_node = new Node(data);
			if (priority_val == None && mLength) {
				tail->next = new_node;
				tail = new_node;
				++mLength;
				return;
			}
			if (mLength) {
				Node* cur_node = head;
				size_t index = 0;
				switch (priority_val) {
					case Ascending:
						while (cur_node && cur_node->data <= data) {
							cur_node = cur_node->next;
							++index;
						}
						break;

					case Descending:
						while (cur_node && cur_node->data >= data) {
							cur_node = cur_node->next;
							++index;
						}
						break;
				}
				if (index == 0) {
					new_node->next = head;
					if (head == tail)
						tail = new_node->next;
					head = new_node;
					++mLength;
					return;
				}
				size_t _index = 1;
				cur_node = head;
				Node* last_node = nullptr;
				while (true) {
					last_node = cur_node;
					cur_node = cur_node->next;
					if (_index == index) {
						last_node->next = new_node;
						new_node->next = cur_node;
						++mLength;
						return;
					}
					++_index;
				}
			}
			head = new_node;
			tail = head;
			mLength = 1;
		}

		void enqueue(T&& data) override {
			Node* new_node = new Node(std::move(data));
			if (priority_val == None && mLength) {
				tail->next = new_node;
				tail = new_node;
				++mLength;
				return;
			}
			if (mLength) {
				Node* cur_node = head;
				size_t index = 0;
				switch (priority_val) {
					case Ascending:
						while (cur_node && cur_node->data <= data) {
							cur_node = cur_node->next;
							++index;
						}
						break;

					case Descending:
						while (cur_node && cur_node->data >= data) {
							cur_node = cur_node->next;
							++index;
						}
						break;
				}
				if (index == 0) {
					new_node->next = head;
					if (head == tail)
						tail = new_node->next;
					head = new_node;
					++mLength;
					return;
				}
				size_t _index = 1;
				cur_node = head;
				Node* last_node = nullptr;
				while (true) {
					last_node = cur_node;
					cur_node = cur_node->next;
					if (_index == index) {
						last_node->next = new_node;
						new_node->next = cur_node;
						++mLength;
						return;
					}
					++_index;
				}
			}
			head = new_node;
			tail = head;
			mLength = 1;
		}

		bool operator==(const PriorityQueue<T>& other) const {
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

		bool operator==(const Queue<T>& other) const override {
			PriorityQueue<T> temp(other);
			if (mLength != temp.mLength)
				return false;
			Node* cur = head;
			Node* other_cur = temp.head;
			while (cur) {
				if (cur->data != other_cur->data)
					return false;
				cur = cur->next;
				other_cur = other_cur->next;
			}
			return true;
		}

		PriorityQueue<T> operator+(PriorityQueue<T>& right) {
			if (right.mLength) {
				std::vector<T> data = contents();
				std::vector<T> right_data = right.contents();
				for (T& i: right_data)
					data.push_back(i);
				PriorityQueue<T>* res = new PriorityQueue<T>();
				for (const T& i: data)
					res->enqueue(i);
				return *res;
			}
			return *this;
		}

		virtual ~PriorityQueue() {
			if (mLength)
				clear();
		}

	private:
		using typename Queue<T>::Node;
		using Queue<T>::head;
		using Queue<T>::tail;
		using Queue<T>::mLength;
		using Queue<T>::clear;
		using Queue<T>::contents;

		unsigned int priority_val;
		enum Priority : unsigned int {
			None = 0U,
			Ascending,
			Descending
		};
	};
}

#endif // QUEUE_H