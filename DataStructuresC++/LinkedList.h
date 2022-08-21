#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	/**
	 * An iterator class for forwards iterating over the elements of a LinkedList. Provides functionality for incrementing
	 * the iterator and allows for C++ operations such as range based for loops and other iterator methods.
	 * @tparam LinkedList - the LinkedList type to iterate over.
	 */
	template<typename LinkedList>
	class ListIterator {
	public:
		using ListType = typename LinkedList::Node;  /**< An alias for the Node sub-class in the LinkedList. */
		using ValueType = typename LinkedList::ValueType;  /**< An alias for the type of the data in the LinkedList. */

	public:
		/**
		 * Default LinkedList iterator constructor which sets the member pointer to `nullptr`.
		 */
		ListIterator() noexcept: mPtr(nullptr) {}

		/**
		 * Overloaded iterator constructor which provides a pointer to a `Node` in the LinkedList.
		 * @param ptr - pointer a node in the LinkedList.
		 */
		ListIterator(ListType* ptr) noexcept: mPtr(ptr) {}

		/**
		 * Copy constructor for the iterator which copies the other iterator's member pointer.
		 * @param other - an iterator to copy.
		 */
		ListIterator(const ListIterator& other) noexcept: mPtr(other.mPtr) {}

		/**
		 * Copy assignment operator which copies anther LinkedList iterator into the current object.
		 * Checks for and ignores self-assignment.
		 * @param other - an iterator to copy.
		 * @return - a reference to the resultant current object.
		 */
		ListIterator& operator=(const ListIterator& other) noexcept {
			if (this != &other)
				mPtr = other.mPtr;
			return *this;
		}

		/**
		 * Move constructor for the iterator.
		 * @param other - an iterator to move into the current object.
		 */
		ListIterator(ListIterator&& other) noexcept {
			mPtr = other.mPtr;
			other.mPtr = nullptr;
		}

		/**
		 * Move assignment operator. Checks for and ignores self-assignment.
		 * @param other - an iterator object to move into the current object.
		 * @return - a reference to the resultant current object.
		 */
		ListIterator& operator=(ListIterator&& other) noexcept {
			if (this != &other) {
				mPtr = other.mPtr;
				other.mPtr = nullptr;
			}
			return *this;
		}

		/**
		 * Prefix-increment operator which increments the iterator to the next position. This will throw an
		 * `out_of_range` exception if an invalid iterator, one whose member pointer is nullptr, is incremented.
		 * @return - a reference to the current object after incrementing.
		 */
		ListIterator& operator++() {
#ifdef DEBUG
			if (mPtr) {
#endif
				mPtr = mPtr->next;
				return *this;
#ifdef DEBUG
			}
			throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
		}

		/**
		 * Postfix-increment operator which increments the iterator to the next position, but returns a copy of the
		 * iterator at its previous position. This will throw an `out_of_range` exception if an invalid iterator, one
		 * whose member pointer is nullptr, is incremented.
		 * @return - a copy ListIterator object at the position before incrementing.
		 */
		const ListIterator operator++(int) {
#ifdef DEBUG
			if (mPtr) {
#endif
				ListIterator temp(*this);
				++*this;
				return temp;
#ifdef DEBUG
			}
			throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
		}

		/**
		 * Advances the iterator by a given value. If the value is out of the range of the iterator, an
		 * `invalid_argument` exception is thrown.
		 *
		 * \note
		 * As the data structure is a LinkedList, the iterator is forward only.
		 *
		 * @param distance - an unsigned integer to represent the number of positions to advance.
		 * @return - a reference to the current object.
		 */
		ListIterator& advance(const size_t& distance) {
#ifdef DEBUG
			if (mPtr) {
#endif
				size_t moved = 0;
				while (mPtr && moved < distance) {
					++*this;
					++moved;
				}
#ifdef DEBUG
				if (moved != distance)
					throw std::invalid_argument("Distance out of range of iterator");
#endif
				return *this;
			}
#ifdef DEBUG
			throw std::runtime_error("Iterator is at an invalid position, cannot advance");
#endif
		}

		/**
		 * Advances the iterator to the next position. If the current position is not valid, i.e. the iterator points
		 * to nullptr, an `out_of_range` exception is thrown.
		 * @return - a copy of the incremented object.
		 */
		ListIterator next() const {
#ifdef DEBUG
			if (mPtr)
#endif
				return ++ListIterator(*this);
#ifdef DEBUG
			throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
		}

		/**
		 * Plus operator which advances the iterator by the distance specified. If the distance goes out of the
		 * range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a copy of the advanced object.
		 */
		ListIterator operator+(const size_t& amount) {
			ListIterator result(*this);
#ifdef DEBUG
			size_t moved = 0;
			while (result.mPtr && moved < amount) {
				result.mPtr = result.mPtr->next;
				++moved;
			}
			if (moved == amount)
				return result;
			throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
#ifdef RELEASE
			for (size_t i = 0; i < amount; ++i)
				result.mPtr = result.mPtr->next;
			return result;
#endif
		}

		/**
		 * Plus-equals operator which advances the current object by the distance specified. If the distance goes
		 * out of the range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a reference to the advanced object.
		 */
		ListIterator& operator+=(const size_t& amount) {
#ifdef DEBUG
			size_t moved = 0;
			while (mPtr && moved < amount) {
				mPtr = mPtr->next;
				++moved;
			}
			if (moved == amount)
				return *this;
			throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
#ifdef RELEASE
			for (size_t i = 0; i < amount; ++i)
				mPtr = mPtr->next;
			return *this;
#endif
		}

		/**
		 * Equivalence operator which compares two LinkedList iterators to see if they are at the same position.
		 * @param other - another LinkedList iterator to compare.
		 * @return - a boolean indicating if the two iterators are at the same position.
		 */
		bool operator==(const ListIterator& other) const noexcept {
			return mPtr == other.mPtr;
		}

		/**
		 * Not-equivalence operator which compares two LinkedList iterators to see if they are not at the same position.
		 * @param other - another LinkedList iterator to compare.
		 * @return - a boolean indicating if the two iterators are not at the same position.
		 */
		bool operator!=(const ListIterator& other) const noexcept {
			return mPtr != other.mPtr;
		}

		/**
		 * De-reference operator which returns the data at the current iterator position. If the iterator points
		 * to an invalid position, a `runtime_error` exception is thrown.
		 * @return - A reference to the data at the current iterator position.
		 */
		ValueType& operator*() const {
#ifdef DEBUG
			if (mPtr)
#endif
				return mPtr->data;
#ifdef DEBUG
			throw std::runtime_error("Iterator does not point to a valid position, cannot dereference");
#endif
		}

		/**
		 * Member access operator allows access to the member function of the object being iterated over, directly from the iterator.
		 * @return - a pointer to the current position of the iterator.
		 */
		ListType* operator->() const noexcept {
			return mPtr;
		}

		/**
		 * Returns the length of the LinkedList object being iterated over.
		 * @return - an unsigned integer representing the length of the LinkedList object.
		 */
		size_t _size() const noexcept {
			return LinkedList::mLength;
		}

		virtual ~ListIterator() = default;

	private:
		ListType* mPtr;  /**< A pointer of type LinkedList::Node which points to the current position in the LinkedList. */
	};

	/**
	 * A template implementation of a linked list where nodes of data are stored in non-contiguous memory and each
	 * node has a member data and a pointer to the next node. The first and last nodes in the list are tracked using
	 * the head and tail member variables. This data structure offers *O(1)* element insertion, deletion, and element
	 * retrieval from the beginning or end of the list. The time complexity of retrieving any other element is
	 * *O(n)*.
	 *
	 * \note
	 * Methods where exceptions are not to be thrown are all explicitly marked noexcept, therefore extreme exceptions
	 * such as <a href="https://en.cppreference.com/w/cpp/memory/new/bad_alloc">`std::bad_alloc`</a> will terminate
	 * the program. Otherwise, exceptions are used when the class is used incorrectly so as to allow for the program
	 * to continue running.
	 *
	 * @tparam T - the type of the data to be stored in each node.
	 * @see <a href="https://en.wikipedia.org/wiki/Linked_list">Linked list</a>
	 */
	template<typename T>
	class LinkedList {
	public:
		using ValueType = T;  /**< An alias for the type of data `T` to be used by external utility classes. */
		using Iterator = ListIterator<LinkedList>;  /**< An alias for the LinkedList iterator class. */

		friend class ListIterator<LinkedList>;  /**< Friend LinkedList iterator class, allowing it to access private members. */

	public:
		/**
		 * Default LinkedList constructor which initialises the head and tail pointer members to nullptr and
		 * the length to 0.
		 */
		LinkedList() noexcept: head(nullptr), tail(nullptr), mLength(0) {}

		/**
		 * Overloaded LinkedList constructor which allocates memory for one element node and copies the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - data of type `T` to be copied into the head node of the LinkedList.
		 */
		explicit LinkedList(const T& data) noexcept: mLength(1) {
			head = new Node(data);
			tail = head;
		}

		/**
		 * Overloaded LinkedList constructor which allocates memory for one element node and moves the data provided.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 * @param data - an *r-value reference* to data of type `T`, to be moved into the head node of the LinkedList.
		 */
		explicit LinkedList(T&& data) noexcept: mLength(1) {
			head = new Node(std::move(data));
			tail = head;
		}

		/**
		 * Overloaded LinkedList constructor which takes an argument of an initialiser list of type `T` and appends
		 * its arguments to the list.
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list.
		 * @param init - an initialiser list of type `T` whose contents will be added to the list.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		LinkedList(std::initializer_list<T> init) noexcept: mLength(0) {
			for (auto it = init.begin(); it != init.end(); ++it)
				append(std::move(*it));
		}

		/**
		 * Copy constructor for a LinkedList which will perform a deep copy, element-wise, of another LinkedList
		 * object of the same type `T`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other list.
		 * @param other - another LinkedList object of the same type `T` to be copied.
		 */
		LinkedList(LinkedList<T>& other) noexcept: mLength(other.mLength) {
			if (other.mLength) {
				head = new Node(other.head->data);
				tail = head;
				Node* other_node = other.head->next;
				while (other_node) {
					tail->next = new Node(other_node->data);
					tail = tail->next;
					other_node = other_node->next;
				}
				return;
			}
			head = nullptr;
			tail = head;
			mLength = 0;
		}

		/**
		 * Copy assignment operator for the LinkedList which will copy another LinkedList object of the same type
		 * `T` into the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other list + the number of elements
		 * in the current list.
		 * @param other - another LinkedList object of the same type `T` to be copied.
		 * @return - a reference to the current object.
		 */
		LinkedList<T>& operator=(const LinkedList<T>& other) noexcept {
			if (this != &other) {
				if (mLength)
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
		 * Move constructor for a LinkedList which will take the data from another LinkedList object of the same type
		 * `T` and set the other object to its default state of not have any data.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a LinkedList object of type `T` to be moved.
		 */
		LinkedList(LinkedList<T>&& other) noexcept: head(other.head), tail(other.tail), mLength(other.mLength) {
			other.head = nullptr;
			other.tail = nullptr;
			other.mLength = 0;
		}

		/**
		 * Move assignment operator for the LinkedList which will move another LinkedList object of type `T` into
		 * the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a LinkedList object of type `T` to be moved.
		 * @return - a reference to the current object.
		 */
		LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
			if (this != &other) {
				if (mLength)
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
		 * Allocates memory for a new element node with the data provided and adds the element to the end of the list.
		 * If the list is empty, it initialises the head of the list with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied into the end of the list.
		 */
		void append(const T& data) noexcept {
			Node* new_node = new Node(data);
			if (mLength) {
				++mLength;
				tail->next = new_node;
				tail = new_node;
				return;
			}
			++mLength;
			head = new_node;
			tail = head;
		}

		/**
		 * Allocates memory for a new element node with the data provided and adds the element to the end of the list.
		 * If the list is empty, it initialises the head of the list with the data provided.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved into the end of the list.
		 */
		void append(T&& data) noexcept {
			Node* new_node = new Node(std::move(data));
			if (mLength) {
				++mLength;
				tail->next = new_node;
				tail = new_node;
				return;
			}
			++mLength;
			head = new_node;
			tail = head;
		}

		/**
		 * Adds elements from an initialiser list, in order, to the end of the list.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the initialiser list.
		 * @param list - the initialiser list whose elements will be appended to the list.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		void append(std::initializer_list<T> list) noexcept {
			for (auto it = list.begin(); it != list.end(); ++it)
				append(std::move(*it));
		}

		/**
		 * An alias method for append(), appends a node with the data provided to the end of the list.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied into the end of the list.
		 */
		void push_back(const T& data) noexcept {
			append(data);
		}

		/**
		 * An alias method for append(), appends a node with the data provided to the end of the list.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved into the end of the list.
		 */
		void push_back(T&& data) noexcept {
			append(std::move(data));
		}

		/**
		 * Allocates memory for and inserts an element node with the data provided at a given index of the list.
		 * If the index is out of the range of the list, an `invalid_argument` exception is thrown.
		 * If the list is uninitialized, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param data - the data of type `T` to be copied into a new node at the given index of the list.
		 * @param index - an unsigned integer to represent the index of the list to insert into.
		 */
		void insert(const T& data, const size_t& index) {
#ifdef DEBUG
			if (mLength && index <= mLength) {
#endif
				Node* new_node = new Node(data);
				++mLength;
				if (index == 0) {
					new_node->next = head;
					head = new_node;
					return;
				}
				if (index == mLength) {
					tail->next = new_node;
					tail = new_node;
					return;
				}
				size_t _index = 1;
				Node* cur_node = head;
				Node* last_node;
				while (true) {
					last_node = cur_node;
					cur_node = cur_node->next;
					if (_index == index) {
						last_node->next = new_node;
						new_node->next = cur_node;
						return;
					}
					++_index;
				}
#ifdef DEBUG
			}
			if (index > mLength)
				throw std::invalid_argument("Invalid index, out of range");
			throw std::runtime_error("Linked list is empty and uninitialised, use append instead");
#endif
		}

		/**
		 * Allocates memory for and inserts an element node with the data provided at a given index of the list.
		 * If the index is out of the range of the list, an `invalid_argument` exception is thrown.
		 * If the list is uninitialized, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param data - an *r-value reference* to the data to be moved into a new node at the given index of the list.
		 * @param index - an unsigned integer to represent the index of the list to insert into.
		 */
		void insert(T&& data, const size_t& index) {
#ifdef DEBUG
			if (mLength && index <= mLength) {
#endif
				Node* new_node = new Node(std::move(data));
				++mLength;
				if (index == 0) {
					new_node->next = head;
					head = new_node;
					return;
				}
				if (index == mLength) {
					tail->next = new_node;
					tail = new_node;
					return;
				}
				size_t _index = 1;
				Node* cur_node = head;
				Node* last_node;
				while (true) {
					last_node = cur_node;
					cur_node = cur_node->next;
					if (_index == index) {
						last_node->next = new_node;
						new_node->next = cur_node;
						return;
					}
					++_index;
				}
#ifdef DEBUG
			}
			if (mLength && index > mLength)
				throw std::invalid_argument("Invalid index, out of range");
			throw std::runtime_error("Linked list is empty and uninitialised, use append instead");
#endif
		}

		/**
		 * Allocates memory for and inserts an element to the beginning of the list, by calling insert() with the
		 * data forwarded and the index of 0.
		 * **Time Complexity** = *O(1)*.
		 * @param data - the data to be copied into a new node at the beginning of the list.
		 */
		void push_front(const T& data) noexcept {
			Node* new_node = new Node(data);
			++mLength;
			new_node->next = head;
			head = new_node;
		}

		/**
		 * Allocates memory for and inserts an element to the beginning of the list, by calling insert() with the
		 * data forwarded and the index of 0.
		 * **Time Complexity** = *O(1)*.
		 * @param data - an *r-value reference* to the data to be moved into a new node at the beginning of the list.
		 */
		void push_front(T&& data) noexcept {
			Node* new_node = new Node(std::move(data));
			++mLength;
			new_node->next = head;
			head = new_node;
		}

		/**
		 * Adds the contents of the list, in order, into a `std::vector` of type `T` and returns it.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @return - a `std::vector` of type `T` containing the contents of the list.
		 * @see <a href="https://en.cppreference.com/w/cpp/container/vector">std::vector</a>
		 */
		[[nodiscard]] std::vector<T> contents() const noexcept {
			std::vector<T> elems(mLength);
			Node* cur_node = head;
			for (int i = 0; i < mLength; ++i) {
				elems[i] = cur_node->data;
				cur_node = cur_node->next;
			}
			return elems;
		}

		/**
		 * Finds the index of the node with the data provided. If multiple nodes exist with the same data, the index
		 * of only the first node is returned. If a node with the data provided is not found, a value of **-1**
		 * is returned. If the list is uninitialized, i.e. the head member pointer is nullptr, a `runtime_error`
		 * exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param data - the data to be searched for in the list.
		 * @return - an integer value representing the index of the node with the data.
		 */
		[[nodiscard]] int find(const T& data) const {
#ifdef DEBUG
			if (mLength) {
#endif
				int index = 0;
				Node* cur_node = head;
				while (cur_node) {
					if (cur_node->data == data)
						return index;
					cur_node = cur_node->next;
					++index;
				}
				return -1;
#ifdef DEBUG
			}
			throw std::runtime_error("Error: Linked list is empty, there is no content to search");
#endif
		}

		/**
		 * Calls `std::cout` on each element in the list, to print the data of the list, in order, onto the console.
		 * If the list is uninitialized, i.e. the head member pointer is nullptr, a `runtime_error` exception is thrown.
		 * \note
		 * The type `T` must be compatible with `std::cout`.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @see <a href="https://en.cppreference.com/w/cpp/io/cout">std::cout</a>
		 */
		void display() const {
#ifdef DEBUG
			if (mLength) {
#endif
				std::vector<T> vals = contents();
				for (const T& i: vals)
					std::cout << i << "\t";
				std::cout << "\n";
#ifdef DEBUG
			} else
				throw std::runtime_error("Error: Linked list is empty, nothing to display");
#endif
		}

		/**
		 * Provides a value for the number of elements in the list.
		 * **Time Complexity** = *O(1)*.
		 * @return - an unsigned integer representing the number of elements in the list.
		 */
		[[nodiscard]] size_t length() const noexcept {
			return mLength;
		}

		/**
		 * Provides a boolean value that indicates whether the list contains any elements.
		 * **Time Complexity** = *O(1)*.
		 * @return - a boolean value that indicates whether the list is empty or not.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return mLength == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the list is not 0, otherwise
		 * it evaluates to `false`.
		 * **Time Complexity** = *O(1)*.
		 * @return - the boolean value of whether the size of the list is 0.
		 */
		explicit operator bool() const noexcept {
			return (bool)mLength;
		}

		/**
		 * Equivalence operator which compares two LinkedList objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current list + the number of elements
		 * in the other list.
		 * @param other - a LinkedList object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two lists contain the same data.
		 */
		[[nodiscard]] bool operator==(const LinkedList<T>& other) const noexcept {
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
		 * Not-equivalence operator which compares two LinkedList objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain different data.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current list + the number of elements
		 * in the other list.
		 * @param other - a LinkedList object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two lists contain different data.
		 */
		[[nodiscard]] bool operator!=(const LinkedList<T>& other) const noexcept {
			return !(*this == other);
		}

		/**
		 * Removes the element at the specified index from the list. If the index is out of the range of the list,
		 * an `invalid_argument` exception is thrown. If the list is empty, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param index - an unsigned integer specifying the index of the element to be removed.
		 */
		void erase(const size_t& index) {
#ifdef DEBUG
			if (mLength && index < mLength) {
#endif
				if (index == 0) {
					Node* head_cpy = head;
					head = head->next;
					delete head_cpy;
					--mLength;
					return;
				}
				size_t cur_index = 1;
				Node* cur_node = head;
				while (true) {
					Node* last_node = cur_node;
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
#ifdef DEBUG
			}
			if (mLength && index >= mLength)
				throw std::invalid_argument("Invalid index, out of range");
			throw std::runtime_error("Error: Linked list is empty, there is nothing to erase");
#endif
		}

		/**
		 * Erases all elements from the list and deallocates its memory. Sets the head member pointer to nullptr and
		 * the length to 0.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 */
		void clear() noexcept {
			Node* cur_node = head;
			while (cur_node) {
				cur_node = cur_node->next;
				delete head;
				head = cur_node;
			}
			head = nullptr;
			tail = head;
			mLength = 0;
		}

		/**
		 * Retrieves the data of the element at the specified index. If the index is out of the range of the list,
		 * an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param index - an unsigned integer specifying the index of the element whose data to retrieve.
		 * @return - a reference to the data of the element at the specified index.
		 */
		[[nodiscard]] T& get(const size_t& index) {
#ifdef DEBUG
			if (index < mLength) {
#endif
				if (index == 0)
					return head->data;
				size_t cur_index = 1;
				Node* cur_node = head;
				while (true) {
					cur_node = cur_node->next;
					if (cur_index == index)
						return cur_node->data;
					++cur_index;
				}
#ifdef DEBUG
			}
			if (mLength && index >= mLength)
				throw std::invalid_argument("Invalid index, out of range");
			throw std::runtime_error("Error: Linked list is empty, there is nothing to get");
#endif
		}

		/**
		 * Retrieves the data of the element at the specified index. If the index is out of the range of the list,
		 * an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param index - an unsigned integer specifying the index of the element whose data to retrieve.
		 * @return - a const reference to the data of the element at the specified index.
		 */
		[[nodiscard]] const T& get(const size_t& index) const {
#ifdef DEBUG
			if (index < mLength) {
#endif
				if (index == 0)
					return head->data;
				size_t cur_index = 1;
				Node* cur_node = head;
				while (true) {
					cur_node = cur_node->next;
					if (cur_index == index)
						return cur_node->data;
					++cur_index;
				}
#ifdef DEBUG
			}
			if (mLength && index >= mLength)
				throw std::invalid_argument("Invalid index, out of range");
			throw std::runtime_error("Error: Linked list is empty, there is nothing to get");
#endif
		}

		/**
		 * Retrieves the data of the element at the beginning of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a reference of the data of the element at the beginning of the list.
		 */
		T& front() {
#ifdef DEBUG
			if (mLength)
#endif
				return head->data;
			throw std::runtime_error("List is empty, there is nothing at front");
		}

		/**
		 * Retrieves the data of the element at the beginning of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a const reference of the data of the element at the beginning of the list.
		 */
		const T& front() const {
#ifdef DEBUG
			if (mLength)
#endif
				return head->data;
			throw std::runtime_error("List is empty, there is nothing at front");
		}

		/**
		 * Retrieves the data of the element at the end of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a reference of the data of the element at the end of the list.
		 */
		T& back() {
#ifdef DEBUG
			if (mLength)
#endif
				return tail->data;
			throw std::runtime_error("List is empty, there is nothing at back");
		}

		/**
		 * Retrieves the data of the element at the end of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 * @return - a const reference of the data of the element at the end of the list.
		 */
		const T& back() const {
#ifdef DEBUG
			if (mLength)
#endif
				return tail->data;
			throw std::runtime_error("List is empty, there is nothing at back");
		}

		/**
		 * Removes the element at the beginning of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 */
		void pop_front() {
#ifdef DEBUG
			if (mLength) {
#endif
				Node* temp = head;
				head = head->next;
				delete temp;
				--mLength;
#ifdef DEBUG
			} else
				throw std::runtime_error("List is empty, there is nothing to pop front");
#endif
		}

		/**
		 * Removes the element at the end of the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(1)*.
		 */
		void pop_back() {
#ifdef DEBUG
			if (mLength)
#endif
				erase(mLength - 1);
#ifdef DEBUG
			else
				throw std::runtime_error("List is empty, there is nothing to pop back");
#endif
		}

		/**
		 * Reverses the order of the elements in the list. If the list is uninitialized, i.e. the head
		 * member pointer is `nullptr`, a `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 */
		void reverse_order() {
#ifdef DEBUG
			if (mLength) {
#endif
				Node* cur_node = head;
				tail = head;
				Node* last = nullptr;
				Node* next;
				while (cur_node) {
					next = cur_node->next;
					cur_node->next = last;
					last = cur_node;
					cur_node = next;
				}
				head = last;
#ifdef DEBUG
			} else
				throw std::runtime_error("Error: linked list is empty and so cannot be reversed");
#endif
		}

		/**
		 * Square brackets operator which retrieves the data for the element at a specified index using get().
		 * If the index provided is out of the range of the list, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param index - an unsigned integer specifying the index of the element whose data to retrieve.
		 * @return - a reference to the data of the element at the specified index.
		 */
		[[nodiscard]] T& operator[](const size_t& index) {
			return get(index);
		}

		/**
		 * Square brackets operator which retrieves the data for the element at a specified index using get().
		 * If the index provided is out of the range of the list, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 * @param index - an unsigned integer specifying the index of the element whose data to retrieve.
		 * @return - a const reference to the data of the element at the specified index.
		 */
		[[nodiscard]] const T& operator[](const size_t index) const {
			return get(index);
		}

		/**
		 * Plus operator which appends another LinkedList object of type `T` to the current list.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other list.
		 * @param right - a LinkedList object of type `T` to append to the current list.
		 * @return - a copy of the current list object.
		 */
		[[nodiscard]] LinkedList<T> operator+(LinkedList<T>& right) noexcept {
			if (right.mLength) {
				std::vector<T> right_data = right.contents();
				LinkedList<T> res(*this);
				for (const T& i: right_data)
					res.append(i);
				return res;
			}
			return *this;
		}

		/**
		 * Creates and returns an iterator with the position of the beginning of the list.
		 * **Time Complexity** = *O(1)*.
		 * @return - a ListIterator object with the position of the beginning of the list.
		 */
		Iterator begin() const noexcept {
			return Iterator(head);
		}

		/**
		 * Creates and returns an iterator with the position of the end of the list.
		 * **Time Complexity** = *O(1)*.
		 * @return - a ListIterator object with the position of the end of the list.
		 */
		Iterator end() const noexcept {
			return Iterator(nullptr);
		}

		/**
		 * LinkedList destructor which clears the list and releases any memory allocated for each element.
		 * **Time Complexity** = *O(n)* where n is the number of elements in the list.
		 */
		virtual ~LinkedList() {
			if (mLength)
				clear();
		}

	private:
		/**
		 * A node structure to contain the data at each element and a pointer to the next node in the list.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each element node. */
			Node* next = nullptr;  /**< A pointer to the next node object in the list. */

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

		Node* head;  /**< A pointer to the first node element of the list. */
		Node* tail;  /**< A pointer to the last node element of the list.  */
		size_t mLength;  /**< An unsigned integer specifying the number of elements in the list. */
	};
}// namespace custom

#endif// LINKED_LIST_H