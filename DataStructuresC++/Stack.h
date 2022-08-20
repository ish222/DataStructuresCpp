#ifndef STACK_H
#define STACK_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	/**
	 * A template implementation of the stack data structure. Elements are stored in the order of insertion and the
	 * LIFO (last-in-first-out) idea is followed, where the last element to be inserted into the queue is the first
	 * to be removed. This is a linked list implementation of the stack.
	 *
	 * \note
	 * Methods where exceptions are not to be thrown are all explicitly marked noexcept, therefore extreme exceptions
	 * such as <a href="https://en.cppreference.com/w/cpp/memory/new/bad_alloc">`std::bad_alloc`</a> will terminate
	 * the program. Otherwise, exceptions are used when the class is used incorrectly so as to allow for the program
	 * to continue running.
	 *
	 * @tparam T - the type of data to be stored in each node of the stack.
	 * @see <a href="https://en.wikipedia.org/wiki/Stack_(abstract_data_type)">Stack data structure</a>
	 */
	template<typename T>
	class Stack {
	public:
		/**
		 * Default Stack constructor which initialises the head pointer member to nullptr and the length to 0.
		 */
		Stack() noexcept: head(nullptr), mLength(0) {}

		/**
		 * Overloaded Stack constructor which allocates memory for one element node and copies the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - data of type `T` to be copied into the head node of the Stack.
		 */
		explicit Stack(const T& data) noexcept: mLength(1) {
			head = new Node(data);
		}

		/**
		 * Overloaded Stack constructor which allocates memory for one element node and moves the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - an *r-value reference* to data of type `T`, to be moved into the head node of the Stack.
		 */
		explicit Stack(T&& data) noexcept: mLength(1) {
			head = new Node(std::move(data));
		}

		/**
		 * Overloaded Stack constructor which takes an argument of an initialiser list of type `T` and adds
		 * its arguments, in order, to the top of the stack.
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list.
		 * @param init - an initialiser list of type `T` whose contents will be added to the stack.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		Stack(std::initializer_list<T> init) noexcept: mLength(0) {
			for (auto it = init.begin(); it != init.end(); ++it)
				push(std::move(*it));
		}

		/**
		 * Copy constructor for a Stack which will perform a deep copy, element-wise, of another Stack
		 * object of the same type `T`.
		 * **Time Complexity** = *O(n)* where n is the twice the number of elements in the other stack.
		 * @param other - another Stack object of the same type `T` to be copied.
		 */
		Stack(const Stack<T>& other) noexcept {
			if (other.mLength) {
				const auto other_contents = other.contents();
				mLength = 0;
				for (size_t i = other_contents.size(); i > 0; --i)  // Needs to be pushed into the stack in reverse order.
					push(other_contents[i-1]);
				return;
			}
			head = nullptr;
			mLength = 0;
		}

		/**
		 * Copy assignment operator for the Stack which will copy another Stack object of the same type
		 * `T` into the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other stack + the number of elements
		 * in the current stack.
		 * @param other - another Stack object of the same type `T` to be copied.
		 * @return - a reference to the current object.
		 */
		Stack<T>& operator=(const Stack<T>& other) noexcept {
			if (this != &other) {
				if (mLength)
					clear();
				if (other.mLength) {
					const auto other_contents = other.contents();
					mLength = 0;
					for (size_t i = other_contents.size(); i > 0; --i)
						push(other_contents[i-1]);
				} else {
					head = nullptr;
					mLength = 0;
				}
			}
			return *this;
		}

		/**
		 * Move constructor for a Stack which will take the data from another Stack object of the same type
		 * `T` and set the other object to its default state of not have any data.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Stack object of type `T` to be moved.
		 */
		Stack(Stack<T>&& other) noexcept: head(other.head), mLength(other.mLength) {
			other.head = nullptr;
			other.mLength = 0;
		}

		/**
		 * Move assignment operator for the Stack which will move another Stack object of type `T` into
		 * the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Stack object of type `T` to be moved.
		 * @return - a reference to the current object.
		 */
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

		/**
		 * Allocates memory for a new element node with the data provided and adds the element to the top of the stack.
		 * If the stack is empty, it initialises the head of the stack with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied onto the top of the stack.
		 */
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

		/**
		 * Allocates memory for a new element node with the data provided and adds the element to the top of the stack.
		 * If the stack is empty, it initialises the head of the stack with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved onto the top of the stack.
		 */
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

		/**
		 * Adds elements from an initialiser list, in order, to the top of the stack.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the initialiser list.
		 * @param list - the initialiser list whose elements will be added to the top of the stack
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		void push(std::initializer_list<T> list) noexcept {
			for (auto it = list.begin(); it != list.end(); ++it)
				push(std::move(*it));
		}

		/**
		 * Removes the element at the top of the stack and returns a copy of its data. If the stack is empty, a
		 * `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a copy of the data of the element at the top of the stack.
		 */
		T pop() {
			if (mLength) {
				T result = head->data;
				Node* cur = head;
				head = head->next;
				delete cur;
				--mLength;
				return result;
			}
			throw std::runtime_error("Stack is empty, there is nothing to pop.");
		}

		/**
		 * Retrieves the data of the element at the top of the stack. If the stack is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a reference of the data of the element at the top of the stack.
		 */
		[[nodiscard]] T& peek() {
			if (mLength)
				return head->data;
			throw std::runtime_error("Stack is empty, there is nothing to peek.");
		}

		/**
		 * Retrieves the data of the element at the top of the stack. If the stack is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a const reference of the data of the element at the top of the stack.
		 */
		[[nodiscard]] const T& peek() const {
			if (mLength)
				return head->data;
			throw std::runtime_error("Stack is empty, there is nothing to peek.");
		}

		/**
		 * Provides a value for the number of elements in the stack.
		 * **Time Complexity** = *O(1)*.
		 * @return - an unsigned integer representing the number of elements in the stack.
		 */
		[[nodiscard]] size_t length() const noexcept {
			return mLength;
		}

		/**
		 * Provides a boolean value that indicates whether the stack contains any elements.
		 * **Time Complexity** = *O(1)*.
		 * @return - a boolean value that indicates whether the stack is empty or not.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return mLength == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the stack is not 0, otherwise
		 * it evaluates to `false`.
		 * **Time Complexity** = *O(1)*.
		 * @return - the boolean value of whether the size of the stack is 0.
		 */
		explicit operator bool() const noexcept {
			return (bool)mLength;
		}

		/**
		 * Equivalence operator which compares two Stack objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current stack + the number of elements
		 * in the other stack.
		 * @param other - a Stack object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two stacks contain the same data.
		 */
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

		/**
		 * Not-equivalence operator which compares two Stack objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain different data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current stack + the number of elements
		 * in the other stack.
		 * @param other - a Stack object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two stacks contain different data.
		 */
		[[nodiscard]] bool operator!=(const Stack<T>& other) const noexcept {
			return !(*this == other);
		}

		/**
		 * Iterates through the stack and checks whether an element with the data specified exists. If the stack is
		 * uninitialized, i.e. the head member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the stack.
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
			throw std::runtime_error("Error: stack is empty, cannot check for contents");
		}

		/**
		 * Plus operator which adds the data of another Stack object of type `T` to the top of the stack.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other stack.
		 * @param right - a Stack object of type `T` to append to the current stack.
		 * @return - a copy of the current stack object.
		 */
		[[nodiscard]] Stack<T> operator+(Stack<T>& right) noexcept {
			if (right.mLength) {
				std::vector<T> right_data = right.contents();
				Stack<T> res(*this);
				for (size_t i = right_data.size(); i > 0; --i)
					res.push(right_data[i-1]);
				return res;
			}
			return *this;
		}

		/**
		 * Adds the contents of the stack, in order, into a `std::vector` of type `T` and returns it.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the stack.
		 * @return - a `std::vector` of type `T` containing the contents of the stack.
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
		 * Calls `std::cout` on each element in the stack, to print the data of the stack, in order, onto the console.
		 * If the stack is uninitialized, i.e. the head member pointer is nullptr, a `runtime_error` exception is thrown.
		 * \note
		 * The type `T` must be compatible with `std::cout`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the stack.
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
				throw std::runtime_error("Error: stack is empty, there is nothing to display");
		}

		/**
		 * Erases all elements from the stack and deallocates its memory. Sets the head member pointer to nullptr and
		 * the length to 0.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the stack.
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
		 * Stack destructor which clears the stack and releases any memory allocated for each element.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the stack.
		 */
		virtual ~Stack() {
			if (mLength)
				clear();
		}

	private:
		/**
		 * A node structure to contain the data at each element and a pointer to the next node in the stack.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each element node. */
			Node* next = nullptr;  /**< A pointer to the next node object in the stack. */

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

		Node* head;  /**< A pointer to the node element at the top of the stack, this will be the first element to be removed. */
		size_t mLength;  /**< An unsigned integer specifying the number of elements in the stack. */
	};
}// namespace custom

#endif// STACK_H