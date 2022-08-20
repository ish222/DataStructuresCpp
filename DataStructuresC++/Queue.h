#ifndef QUEUE_H
#define QUEUE_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	/**
	 * A template implementation of a queue data structure. Elements are stored in order of insertion and the
	 * FIFO (first-in-first-out) idea is followed, where the first element added to the queue is the first element
	 * to be removed, resembling a real-life queue. This is a linked list implementation of the queue.
	 *
	 * \note
	 * Methods where exceptions are not to be thrown are all explicitly marked noexcept, therefore extreme exceptions
	 * such as <a href="https://en.cppreference.com/w/cpp/memory/new/bad_alloc">`std::bad_alloc`</a> will terminate
	 * the program. Otherwise, exceptions are used when the class is used incorrectly so as to allow for the program
	 * to continue running.
	 *
	 * @tparam T - the type of data to be stored in each node of the queue.
	 * @see <a href="https://en.wikipedia.org/wiki/Queue_(abstract_data_type)">Queue data structure</a>
	 */
	template<typename T>
	class Queue {
	public:
		/**
		 * Default Queue constructor which initialises the head and tail pointer members to nullptr and
		 * the length to 0.
		 */
		Queue() noexcept: head(nullptr), tail(nullptr), mLength(0) {}

		/**
		 * Overloaded Queue constructor which allocates memory for one element node and copies the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - data of type `T` to be copied into the head node of the Queue.
		 */
		explicit Queue(const T& data) noexcept: mLength(1) {
			head = new Node(data);
			tail = head;
		}

		/**
		 * Overloaded Queue constructor which allocates memory for one element node and moves the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - an *r-value reference* to data of type `T`, to be moved into the head node of the Queue.
		 */
		explicit Queue(T&& data) noexcept: mLength(1) {
			head = new Node(std::move(data));
			tail = head;
		}

		/**
		 * Overloaded Queue constructor which takes an argument of an initialiser list of type `T` and appends
		 * its arguments to the queue.
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list.
		 * @param init - an initialiser list of type `T` whose contents will be added to the queue.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		Queue(std::initializer_list<T> init) noexcept: mLength(0) {
			for (auto it = init.begin(); it != init.end(); ++it)
				enqueue(std::move(*it));
		}

		/**
		 * Copy constructor for a Queue which will perform a deep copy, element-wise, of another Queue
		 * object of the same type `T`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue.
		 * @param other - another Queue object of the same type `T` to be copied.
		 */
		Queue(const Queue<T>& other) noexcept: mLength(other.mLength) {
			head = new Node(other.head->data);
			tail = head;
			Node* other_node = other.head->next;
			while (other_node) {
				tail->next = new Node(other_node->data);
				tail = tail->next;
				other_node = other_node->next;
			}
		}

		/**
		 * Copy assignment operator for the Queue which will copy another Queue object of the same type
		 * `T` into the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue + the number of elements
		 * in the current queue.
		 * @param other - another Queue object of the same type `T` to be copied.
		 * @return - a reference to the current object.
		 */
		Queue<T>& operator=(const Queue<T>& other) noexcept {
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

		/**
		 * Move constructor for a Queue which will take the data from another Queue object of the same type
		 * `T` and set the other object to its default state of not have any data.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Queue object of type `T` to be moved.
		 */
		Queue(Queue<T>&& other) noexcept: head(other.head), tail(other.tail), mLength(other.mLength) {
			other.head = nullptr;
			other.tail = nullptr;
			other.mLength = 0;
		}

		/**
		 * Move assignment operator for the Queue which will move another Queue object of type `T` into
		 * the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Queue object of type `T` to be moved.
		 * @return - a reference to the current object.
		 */
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

		/**
		 * Allocates memory for a new element node with the data provided and adds the element to the end of the queue.
		 * If the queue is empty, it initialises the head of the queue with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied into the end of the queue.
		 */
		virtual void enqueue(const T& data) noexcept {
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

		/**
		 * Allocates memory for a new element node with the data provided and adds the element to the end of the queue.
		 * If the queue is empty, it initialises the head of the queue with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved into the end of the queue.
		 */
		virtual void enqueue(T&& data) noexcept {
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

		/**
		 * Adds elements from an initialiser list, in order, to the end of the queue.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the initialiser list.
		 * @param list - the initialiser list whose elements will be appended to the queue.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		virtual void enqueue(std::initializer_list<T> list) noexcept {
			for (auto it = list.begin(); it != list.end(); ++it)
				enqueue(std::move(*it));
		}

		/**
		 * Removes the element at the front of the queue and returns a copy of its data. If the queue is empty, a
		 * `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a copy of the data of the element at the front of the queue.
		 */
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

		/**
		 * Retrieves the data of the element at the beginning of the queue. If the queue is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a reference of the data of the element at the beginning of the queue.
		 */
		[[nodiscard]] T& peek() {
			if (mLength)
				return head->data;
			throw std::runtime_error("Error: queue is empty, there is nothing to peek");
		}

		/**
		 * Retrieves the data of the element at the beginning of the queue. If the queue is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a const reference of the data of the element at the beginning of the queue.
		 */
		[[nodiscard]] const T& peek() const {
			if (mLength)
				return head->data;
			throw std::runtime_error("Error: queue is empty, there is nothing to peek");
		}

		/**
		 * Provides a value for the number of elements in the queue.
		 * **Time Complexity** = *O(1)*.
		 * @return - an unsigned integer representing the number of elements in the queue.
		 */
		[[nodiscard]] int length() const noexcept {
			return mLength;
		}

		/**
		 * Provides a boolean value that indicates whether the queue contains any elements.
		 * **Time Complexity** = *O(1)*.
		 * @return - a boolean value that indicates whether the queue is empty or not.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return mLength == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the queue is not 0, otherwise
		 * it evaluates to `false`.
		 * **Time Complexity** = *O(1)*.
		 * @return - the boolean value of whether the size of the queue is 0.
		 */
		explicit operator bool() const noexcept {
			return (bool)mLength;
		}

		/**
		 * Equivalence operator which compares two Queue objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current queue + the number of elements
		 * in the other queue.
		 * @param other - a Queue object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two queues contain the same data.
		 */
		virtual bool operator==(const Queue<T>& other) const noexcept {
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

		/**
		 * Not-equivalence operator which compares two Queue objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain different data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current queue + the number of elements
		 * in the other queue.
		 * @param other - a Queue object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two queues contain different data.
		 */
		[[nodiscard]] bool operator!=(const Queue<T>& other) const noexcept {
			return !(*this == other);
		}

		/**
		 * Iterates through the queue and checks whether an element with the data specified exists. If the queue is
		 * uninitialized, i.e. the head member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 * @param data - the data of type `T` to check for.
		 * @return - a boolean value indicating whether an element with the data specified exists.
		 */
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
			throw std::runtime_error("Error: queue is empty, cannot check for contents");
		}

		/**
		 * Plus operator which appends another Queue object of type `T` to the current queue.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue.
		 * @param right - a Queue object of type `T` to append to the current queue.
		 * @return - a copy of the current queue object.
		 */
		[[nodiscard]] Queue<T> operator+(Queue<T>& right) const noexcept {
			if (right.mLength) {
				std::vector<T> data = contents();
				std::vector<T> right_data = right.contents();
				for (T& i: right_data)
					data.push_back(i);
				Queue<T> res;
				for (const T& i: data)
					res.enqueue(i);
				return res;
			}
			return *this;
		}

		/**
		 * Adds the contents of the queue, in order, into a `std::vector` of type `T` and returns it.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 * @return - a `std::vector` of type `T` containing the contents of the queue.
		 * @see <a href="https://en.cppreference.com/w/cpp/container/vector">std::vector</a>
		 */
		[[nodiscard]] std::vector<T> contents() const noexcept {
			std::vector<T> elems(mLength);
			Node* cur_node = head;
			for (size_t i = 0; i < mLength; ++i) {
				elems[i] = cur_node->data;
				cur_node = cur_node->next;
			}
			return elems;
		}

		/**
		 * Calls `std::cout` on each element in the queue, to print the data of the queue, in order, onto the console.
		 * If the queue is uninitialized, i.e. the head member pointer is nullptr, a `runtime_error` exception is thrown.
		 * \note
		 * The type `T` must be compatible with `std::cout`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 * @see <a href="https://en.cppreference.com/w/cpp/io/cout">std::cout</a>
		 */
		void display() const {
			if (mLength) {
				std::vector<T> data = contents();
				for (const T& i: data) {
					std::cout << i << "\t";
				}
				std::cout << "\n";
			} else
				throw std::runtime_error("Error: queue is empty, there is nothing to display");
		}

		/**
		 * Erases all elements from the queue and deallocates its memory. Sets the head member pointer to nullptr and
		 * the length to 0.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 */
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

		/**
		 * Queue destructor which clears the queue and releases any memory allocated for each element.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 */
		virtual ~Queue() {
			if (mLength)
				clear();
		}

	protected:
		/**
		 * A node structure to contain the data at each element and a pointer to the next node in the queue.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each element node. */
			Node* next = nullptr;  /**< A pointer to the next node object in the queue. */

			/**
			 * Constructor which copies the data provided into the node object.
			 * @param data - data of type `T` to copy into the node object.
			 */
			explicit Node(const T& data) noexcept: data(data) {}

			/**
			 * Constructor which moves the data provided into the node object.
			 * @param data - an *r-value reference* to data of type `T` to move into the node object.
			 */
			explicit Node(T&& data) noexcept: data(std::move(data)) {}
		};

		Node* head;  /**< A pointer to the first node element of the queue, this will be the first element to be removed. */
		Node* tail;  /**< A pointer to the last node element of the queue, this will be the last element to be removed.  */
		size_t mLength;  /**< An unsigned integer specifying the number of elements in the queue. */
	};

	/**
	 * A specialised version of the Queue class, where the elements in the queue are ordered using a provided condition.
	 * It inherits all the methods from the Queue class while overriding some to meet the requirement described above.
	 *
	 * \note
	 * The type `T` **must** have a valid comparison operator functions.
	 *
	 * @tparam T - the type of data to be stored in each node of the priority queue.
	 * @see <a href="https://en.wikipedia.org/wiki/Priority_queue">Priority Queue</a>
	 */
	template<typename T>
	class PriorityQueue : public Queue<T> {
	public:
		/**
		 * Default PriorityQueue constructor which initialises the priority value to `None` meaning there is no order,
		 * and calls the default Queue() constructor.
		 */
		PriorityQueue() noexcept: priority_val(None), Queue<T>() {}

		/**
		 * Overloaded Queue constructor which allocates memory for one element node and copies the data provided and
		 * sets the priority type to use, which by default is `None`.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data -  data of type `T` to be copied into the head node of the Queue.
		 * @param priority - the priority type of the PriorityQueue.
		 * @see Priority.
		 */
		explicit PriorityQueue(const T& data, unsigned int priority = None) noexcept: priority_val(priority),
		                                                                              Queue<T>(data) {}

		/**
		 * Overloaded Queue constructor which allocates memory for one element node and moves the data provided and
		 * sets the priority type to use, which by default is `None`.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data -  an *r-value reference* to the data of type `T` to be moved into the head node of the Queue.
		 * @param priority - the priority type of the PriorityQueue.
		 * @see Priority.
		 */
		explicit PriorityQueue(T&& data, unsigned int priority = None) noexcept: priority_val(priority),
		                                                                         Queue<T>(std::move(data)) {}

		/**
		 * Copy constructor for PriorityQueue which will perform a deep copy, element-wise, of another PriorityQueue
		 * object of the same type `T`, by calling the base copy constructor. It also copies its priority type.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue.
		 * @param other - another PriorityQueue object of the same type `T` to be copied.
		 */
		PriorityQueue(const PriorityQueue<T>& other) noexcept: Queue<T>(other), priority_val(other.priority_val) {}

		/**
		 * Copy constructor for PriorityQueue which will perform a deep copy, element-wise, of a Queue
		 * object of the same type `T`, by calling the base copy constructor. It sets the priority type to `None`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue.
		 * @param other - a Queue object of the same type `T` to be copied.
		 */
		explicit PriorityQueue(const Queue<T>& other) noexcept: Queue<T>(other), priority_val(None) {}

		/**
		 * Copy assignment operator for the PriorityQueue which will copy another PriorityQueue object of the same type
		 * `T` into the current object, in its order along with its priority type.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other queue + the number of elements
		 * in the current queue.
		 * @param other - another PriorityQueue object of the same type `T` to be copied.
		 * @return - a reference to the current object.
		 */
		PriorityQueue<T>& operator=(const PriorityQueue<T>& other) noexcept {
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

		/**
		 * Move constructor for a PriorityQueue which will take the data from another PriorityQueue object of the same type
		 * `T` and set the other object to its default state of not have any data.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a PriorityQueue object of type `T` to be moved.
		 */
		PriorityQueue(PriorityQueue<T>&& other) noexcept: Queue<T>(std::move(other)),
		                                                  priority_val(other.priority_val) {}

		/**
		 * Move constructor for a PriorityQueue which will take the data from a Queue object of the same type
		 * `T` and set the other object to its default state of not have any data. The priority type will be set to `None`,
		 * and the original order will be maintained.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Queue object of type `T` to be moved.
		 */
		explicit PriorityQueue(Queue<T>&& other) noexcept: Queue<T>(std::move(other)), priority_val(None) {}

		/**
		 * Move assignment operator for the PriorityQueue which will move another PriorityQueue object of type `T` into
		 * the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a PriorityQueue object of type `T` to be moved.
		 * @return - a reference to the current object.
		 */
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

		/**
		 * This is an override for the base class Queue enqueue() method. Allocates memory for a new element node
		 * with the data provided and adds it to the queue positioned at an index based on the priority type. If the queue
		 * is empty, it initialises the head of the queue with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied into the end of the queue.
		 */
		void enqueue(const T& data) noexcept override {
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

		/**
		 * This is an override for the base class Queue enqueue() method. Allocates memory for a new element node
		 * with the data provided and adds it to the queue positioned at an index based on the priority type. If the queue
		 * is empty, it initialises the head of the queue with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved into the end of the queue.
		 */
		void enqueue(T&& data) noexcept override {
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

		/**
		 * This is an override for the base class Queue enqueue() method. Adds each element from an initialiser list,
		 * to the queue positioned at an index based on the priority type.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the initialiser list.
		 * @param list - the initialiser list whose elements will be added to the queue.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		void enqueue(std::initializer_list<T> list) noexcept override {
			for (auto it = list.begin(); it != list.end(); ++it)
				enqueue(std::move(*it));
		}

		/**
		 * Equivalence operator which compares two PriorityQueue objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current queue + the number of elements
		 * in the other queue.
		 * @param other - a PriorityQueue object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two queues contain the same data.
		 */
		[[nodiscard]] bool operator==(const PriorityQueue<T>& other) const noexcept {
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

		/**
		 * This is an override for the base Queue class operator== function. This is an equivalence operator which
		 * compares this PriorityQueue to a Queue object, both of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current queue + the number of elements
		 * in the other queue.
		 * @param other - a Queue object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two queues contain the same data.
		 */
		[[nodiscard]] bool operator==(const Queue<T>& other) const noexcept override {
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

		/**
		 * Plus operator which appends another PriorityQueue object of type `T` to the current queue.
		 * **Time Complexity** = *O(n)* where n is the number of elements in this queue + the number of elements
		 * in the other queue.
		 * @param right - a PriorityQueue object of type `T` to append to the current queue.
		 * @return - a copy of the current queue object.
		 */
		[[nodiscard]] PriorityQueue<T> operator+(PriorityQueue<T>& right) noexcept {
			if (right.mLength) {
				std::vector<T> right_data = right.contents();
				PriorityQueue<T> res(*this);
				for (const T& i: right_data)
					res.enqueue(i);
				return res;
			}
			return *this;
		}

		/**
		 * Plus operator which appends a Queue object of type `T` to the current priority queue.
		 * **Time Complexity** = *O(n)* where n is the number of elements in this queue + the number of elements
		 * in the other queue.
		 * @param right - a Queue object of type `T` to append to the current queue.
		 * @return - a copy of the current queue object.
		 */
		[[nodiscard]] PriorityQueue<T> operator+(Queue<T>& right) noexcept {
			if (right.mLength) {
				std::vector<T> right_data = right.contents();
				PriorityQueue<T> res(*this);
				for (const T& i: right_data)
					res.enqueue(i);
				return res;
			}
			return *this;
		}

		/**
		 * PriorityQueue destructor which clears the queue and releases any memory allocated for each element.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the queue.
		 */
		virtual ~PriorityQueue() {
			if (mLength)
				clear();
		}

	public:
		using Queue<T>::clear;  /**< An alias used to cleanly access clear member function in the base class. */
		using Queue<T>::contents;  /**< An alias used to cleanly access contents member function in the base class. */

	private:
		using typename Queue<T>::Node;  /**< An alias used to easily access the Node structure in the base class. */
		using Queue<T>::head;  /**< An alias used to cleanly access head member in the base class. */
		using Queue<T>::tail;  /**< An alias used to cleanly access tail member in the base class. */
		using Queue<T>::mLength;  /**< An alias used to cleanly access mLength member in the base class. */

		unsigned int priority_val;  /**< An unsigned integer to track the type of the priority applied to the queue. */
		/**
		 * An enum containing the possible priority types for the queue.
		 * **0** indicates no order.
		 * **1** indicates ascending order.
		 * **2** indicates descending order.
		 */
		enum Priority : unsigned int {
			None = 0U,
			Ascending,
			Descending
		};
	};
}// namespace custom

#endif// QUEUE_H