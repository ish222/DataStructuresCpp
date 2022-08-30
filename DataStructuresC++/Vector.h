#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace custom {

	/**
	 * An iterator class for forwards or backwards iterating over the elements of a Vector. Provides
	 * functionality for incrementing or decrementing the iterator and allows for C++ operations such as range
	 * based for loops and other iterator methods. The current position of the iterator along with the Vector's
	 * beginning and, past the ending positions are tracked using pointers.
	 * @tparam Vector - the Vector type to iterate over.
	 */
	template<typename Vector>
	class VectorIterator {
	public:
		using DataType = typename Vector::Type;  /**< An alias for the type of the data in the Vector. */

	public:
		/**
		 * Default Vector iterator constructor which sets the member pointers of the iterator to `nullptr`.
		 */
		VectorIterator() noexcept : mPtr(nullptr), mBegin(nullptr), mEnd(nullptr) {}

		/**
		 * Overloaded iterator constructor which provides a pointer to an element in the Vector.
		 * @param ptr - a pointer to a valid position in the Vector.
		 * @param begin - a pointer to the first element in the Vector.
		 * @param end - a pointer past the last element in the Vector.
		 */
		VectorIterator(DataType* ptr, DataType* begin, DataType* end) noexcept : mPtr(ptr), mBegin(begin), mEnd(end) {}

		/**
		 * Copy constructor for the iterator which copies the other iterator's member pointers.
		 * @param other - an iterator to copy.
		 */
		VectorIterator(const VectorIterator& other) noexcept : mPtr(other.mPtr), mBegin(other.mBegin), mEnd(other.mEnd) {}

		/**
		 * Copy assignment operator which copies anther Vector iterator into the current object.
		 * Checks for and ignores self-assignment.
		 * @param other - an iterator to copy.
		 * @return - a reference to the resultant current object.
		 */
		VectorIterator& operator=(const VectorIterator& other) noexcept {
			if (this != &other) {
				mPtr = other.mPtr;
				mBegin = other.mBegin;
				mEnd = other.mEnd;
			}
			return *this;
		}

		/**
		 * Move constructor for the iterator.
		 * @param other - an iterator to move into the current object.
		 */
		VectorIterator(VectorIterator&& other) noexcept : mPtr(other.mPtr), mBegin(other.mBegin), mEnd(other.mEnd) {
			other.mPtr = nullptr;
			other.mEnd = nullptr;
		}

		/**
		 * Move assignment operator. Checks for and ignores self-assignment.
		 * @param other - an iterator object to move into the current object.
		 * @return - a reference to the resultant current object.
		 */
		VectorIterator& operator=(VectorIterator&& other) noexcept {
			if (this != &other) {
				mPtr = other.mPtr;
				mBegin = other.mBegin;
				mEnd = other.mEnd;
				other.mPtr = nullptr;
				other.mBegin = nullptr;
				other.mEnd = nullptr;
			}
			return *this;
		}

		/**
		 * Prefix-increment operator which increments the iterator to the next position. This will throw an
		 * `out_of_range` exception if an invalid iterator, one which points to an element before the beginning
		 * or past the end of the vector, is incremented.
		 * @return - a reference to the current object after incrementing.
		 */
		VectorIterator& operator++() {
			if (mPtr != mEnd) {
				++mPtr;
				return *this;
			}
			throw std::out_of_range("Cannot increment vector iterator past end of vector");
		}

		/**
		 * Postfix-increment operator which increments the iterator to the next position, but returns a copy of the
		 * iterator at its previous position. This will throw an `out_of_range` exception if an invalid iterator, one which
		 * points to an element before the beginning or past the end of the vector, is incremented.
		 * @return - a copy VectorIterator object at the position before incrementing.
		 */
		const VectorIterator operator++(int) {
			if (mPtr != mEnd) {
				const VectorIterator temp(*this);
				++*this;
				return temp;
			}
			throw std::out_of_range("Cannot increment vector iterator past end of vector");
		}

		/**
		 * Prefix-decrement operator which decrements the iterator to the next position. This will throw an
		 * `out_of_range` exception if an invalid iterator, one which points to an element before the beginning
		 * or past the end of the vector, is decremented.
		 * @return - a reference to the current object after decrementing.
		 */
		VectorIterator& operator--() {
			if (mPtr != mBegin) {
				--mPtr;
				return *this;
			}
			throw std::out_of_range("Cannot decrement vector iterator before beginning of vector");
		}

		/**
		 * Postfix-decrement operator which decrements the iterator to the next position. This will throw an
		 * `out_of_range` exception if an invalid iterator, one which points to an element before the beginning
		 * or past the end of the vector, is decremented.
		 * @return - a copy VectorIterator object at the position before decrementing.
		 */
		const VectorIterator operator--(int) {
			if (mPtr != mBegin) {
				const VectorIterator temp(*this);
				--*this;
				return temp;
			}
			throw std::out_of_range("Cannot decrement vector iterator before beginning of vector");
		}

		/**
		 * Advances the iterator by a given value, which could be negative to advance backwards. If the value is out of
		 * the range of the iterator, an `invalid_argument` exception is thrown.
		 *
		 * @param distance - an unsigned integer to represent the number of positions to advance.
		 * @return - a reference to the current object.
		 */
		VectorIterator& advance(const int& distance) {
			if (mPtr != mEnd && mPtr != (mBegin-1)) {
				int moved = 0;
				if (distance > 0) {
					while (mPtr != mEnd && moved < distance) {
						++*this;
						++moved;
					}
				} else {
					while (mPtr != mBegin && moved > distance) {
						--*this;
						--moved;
					}
				}
				if (moved != distance)
					throw std::invalid_argument("Distance out of range of iterator");
				return *this;
			}
			throw std::runtime_error("Iterator is at an invalid position, cannot advance");
		}

		/**
		 * Plus operator which advances the iterator by the distance specified. If the distance goes out of the
		 * range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a copy of an advanced iterator.
		 */
		VectorIterator operator+(const size_t& amount) {
			VectorIterator result(*this);
			size_t moved = 0;
			while (result.mPtr != result.mEnd && moved < amount) {
				++result.mPtr;
				++moved;
			}
			if (moved == amount)
				return result;
			throw std::out_of_range("Cannot move vector iterator past end of vector");
		}

		/**
		 * Plus-equals operator which advances the current object by the distance specified. If the distance goes
		 * out of the range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a reference to the current advanced iterator.
		 */
		VectorIterator& operator+=(const size_t& amount) {
			size_t moved = 0;
			while (mPtr != mEnd && moved < amount) {
				++mPtr;
				++moved;
			}
			if (moved == amount)
				return *this;
			throw std::out_of_range("Cannot move vector iterator past end of vector");
		}

		/**
		 * Minus operator which advances the iterator backwards by the distance specified. If the distance goes out
		 * of the range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a copy of an advanced iterator.
		 */
		VectorIterator operator-(const size_t& amount) {
			VectorIterator result(*this);
			size_t moved = 0;
			while (result.mPtr != result.mBegin && moved < amount) {
				--result.mPtr;
				++moved;
			}
			if (moved == amount)
				return result;
			throw std::out_of_range("Cannot move vector iterator before beginning of vector");
		}

		/**
		 * Minus-equals operator which advances the current object backwards by the distance specified. If the
		 * distance goes out of the range of the iterator, an `out_of_range` exception is thrown.
		 * @param amount - an unsigned integer to represent the distance to advance the iterator by.
		 * @return - a reference to the current advanced iterator.
		 */
		VectorIterator& operator-=(const size_t& amount) {
			size_t moved = 0;
			while (mPtr != mBegin && moved < amount) {
				--mPtr;
				++moved;
			}
			if (moved == amount)
				return *this;
			throw std::out_of_range("Cannot move vector iterator before beginning of vector");
		}

		/**
		 * Equivalence operator which compares two Vector iterators to see if they are at the same position.
		 * @param other - another Vector iterator to compare.
		 * @return - a boolean indicating if the two iterators are at the same position.
		 */
		bool operator==(const VectorIterator& other) const noexcept {
			return mPtr == other.mPtr;
		}

		/**
		 * Not-equivalence operator which compares two Vector iterators to see if they are not at the same position.
		 * @param other - another Vector iterator to compare.
		 * @return - a boolean indicating if the two iterators are not at the same position.
		 */
		bool operator!=(const VectorIterator& other) const noexcept {
			return mPtr != other.mPtr;
		}

		/**
		 * De-reference operator which returns the data at the current iterator position. If the iterator points
		 * to an invalid position, a `runtime_error` exception is thrown.
		 * @return - A reference to the data at the current iterator position.
		 */
		DataType& operator*() const {
			if (mPtr != mEnd && mPtr != (mBegin-1))
				return *mPtr;
			throw std::runtime_error("Iterator does not point to a valid position, cannot dereference");
		}

		/**
		 * Member access operator allows access to the member function of the object being iterated over, directly from the iterator.
		 * @return - a pointer to the current position of the iterator.
		 */
		DataType* operator->() const noexcept {
			return mPtr;
		}

		/**
		 * Returns the length of the Vector object being iterated over.
		 * @return - an unsigned integer representing the length of the Vector object.
		 */
		size_t _size() const noexcept {
			return Vector::mSize;
		}

		virtual ~VectorIterator() = default;

	private:
		DataType* mPtr;  /**< A pointer of type Vector::DataType which points to the current position in the Vector. */
		DataType* mBegin;  /**< A pointer of type Vector::DataType which points to the beginning position of the Vector. */
		DataType* mEnd;  /**< A pointer of type Vector::DataType which points past the ending position of the Vector. */
	};

	/**
	 * A template implementation of a dynamic array. Automatically grows and shrinks
	 * its capacity based on the number of elements in the array. Provides *O(1)* element retrieval and on average,
	 * *O(1)* element addition to the end of the array. Insertion and deletion have a time complexity of *O(n)*.
	 *
	 * Default capacity is 10.
	 *
	 * Where possible, methods which should not throw exceptions are explicitly marked noexcept.
	 *
	 * @tparam T - the type of the data to be stored in the array.
	 */
	template<typename T>
	class Vector {
	public:
		using Type = T;  /**< An alias for the type of data `T` to be used by external utility classes. */
		using Iterator = VectorIterator<Vector>;  /**< An alias for the Vector iterator class. */

		friend class VectorIterator<Vector>;  /**< Friend Vector iterator class, allowing it to access private members. */

	public:
		/**
		 * Default constructor of Vector class which initialises an empty vector array with no
		 * allocated memory. Sets member variables to their default values.
		 */
		Vector() noexcept: capacity{0}, mSize{0}, data{nullptr} {}

		/**
		 * Overloaded constructor of the Vector class. Allocates memory on the heap for the capacity provided.
		 * This method is noexcept, meaning that failure to allocate memory or
		 * a <a href="https://en.cppreference.com/w/cpp/memory/new/bad_alloc">`std::bad_alloc`</a>
		 * exception will terminate the program.
		 * This constructor is explicit, meaning implicit conversion is not supported.
		 *
		 * @param capacity - an unsigned integer to specify the total capacity of the array at initialization.
		 */
		explicit Vector(size_t capacity) noexcept: capacity(capacity), mSize(0) {
			data = (T*)::operator new(capacity *
			                          sizeof(T));  // ::operator new is analogous to malloc i.e. it does not call the constructor of the object within
		}

		/**
		 * Overloaded constructor for the Vector class which takes an argument of an initialiser list of type `T`
		 * and adds its arguments to the array.
		 *
		 * Sets the capacity of the array to one and a half times the number of elements in the initialiser list or
		 * at least 10.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the initialiser list.
		 *
		 * @param init - the initialiser list whose contents will be added to the array
		 *
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		Vector(std::initializer_list<T> init) noexcept: mSize(init.size()) {
			if (mSize < 10)
				capacity = 10;
			else
				capacity = mSize + mSize / 2;
			data = (T*)::operator new(capacity * sizeof(T));
			for (size_t i = 0; i < mSize; ++i)
				data[i] = *(init.begin() + i);
		}

		/**
		 * Copy constructor for the Vector class. This will create a new Vector object and perform an element wise,
		 * deep copy of another vector of the same type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other vector.
		 *
		 * @param other
		 */
		Vector(const Vector<T>& other) noexcept: mSize(other.mSize), capacity(other.capacity) {
			data = (T*)::operator new(capacity * sizeof(T));
			for (int i = 0; i < mSize; ++i)
				data[i] = other[i];
		}

		/**
		 * Copy assignment operator will perform an element wise, deep copy of another vector of the same type `T` into
		 * this Vector object. This function will check for and ignore self assignment.
		 *
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other vector + the number of elements
		 * in the existing vector.
		 *
		 * @param other - the Vector to be copied.
		 * @return - a reference to the current Vector object.
		 */
		Vector<T>& operator=(const Vector<T>& other) noexcept {
			if (this != &other) {
				if (mSize) {
					// Call destructor of elements and deallocate memory
					clear();
					::operator delete(data, capacity * sizeof(T));
				}
				capacity = other.capacity;
				mSize = other.mSize;
				data = (T*)::operator new(capacity * sizeof(T));
				for (int i = 0; i < mSize; ++i)
					data[i] = other[i];
			}
			return *this;
		}

		/**
		 * Move constructor for the Vector class. This will create a new Vector object and move an existing Vector
		 * object's data to this object. Both Vector objects must be of the same type `T`.
		 *
		 * \note
		 * As expected of move operations, the other Vector object will be uninitialized.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param other - an *r-value reference* to the Vector object to be moved.
		 */
		Vector(Vector<T>&& other) noexcept: data(other.data), capacity(other.capacity), mSize(other.mSize) {
			other.data = nullptr;
			other.capacity = 0;
			other.mSize = 0;
		}

		/**
		 * Move assignment operator will move an existing Vector object's data into this object. Both Vector objects
		 * must be of the same type `T`. This function will check for and ignore self assignment.
		 *
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the existing Vector object.
		 *
		 * @param other - an *r-value reference* to the Vector object to be moved.
		 * @return - a reference to the current Vector object.
		 */
		Vector<T>& operator=(Vector<T>&& other) noexcept {
			if (this != &other) {
				if (mSize) {
					clear();
					::operator delete(data, capacity * sizeof(T));
				}
				data = other.data;
				capacity = other.capacity;
				mSize = other.mSize;
				other.data = nullptr;
				other.capacity = 0;
				other.mSize = 0;
			}
			return *this;
		}

		/**
		 * Adds a new element of the type `T` to the end of the array. If the array is at capacity, grow() is called.
		 *
		 * **Time Complexity** = *O(1)* if no growth is necessary, otherwise *O(n)* where n is the number of elements in
		 * the array.
		 *
		 * @param value - an element of the type `T` to be added to the end of the array.
		 *
		 * @see grow()
		 */
		void push_back(const T& value) noexcept {
			if (mSize >= capacity)
				grow();
			data[mSize++] = value;
		}

		/**
		 * Moves an element from an *r-value reference* to the end of the array. If the array is at capacity, grow() is
		 * called.
		 *
		 * **Time Complexity** = *O(1)* if no growth is necessary, otherwise *O(n)* where n is the number of elements in
		 * the array.
		 *
		 * @param value - an *r-value reference* to move to the end of the array.
		 *
		 * @see grow()
		 */
		void push_back(T&& value) noexcept {
			if (mSize >= capacity)
				grow();
			data[mSize++] = std::move(value);
		}

		/**
		 * Adds elements from an initialiser list, in order, to the end of the array. If the size of the initialiser
		 * list combined with the array size exceeds the capacity of the array, init_grow will be called. This will
		 * grow the capacity by one and a half times the total size of the initialiser list and the array size.
		 *
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list + the number of elements
		 * in the existing array (if growth is necessary).
		 *
		 * @param list - the initialiser list whose elements will be appended to the array.
		 *
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 *
		 * @see init_grow()
		 */
		void push_back(std::initializer_list<T> list) noexcept {
			size_t new_size = mSize + list.size();
			if (new_size >= capacity)
				init_grow(new_size + new_size / 2);
			for (auto it = list.begin(); it != list.end(); ++it)
				data[mSize++] = std::move(*it);
		}

		/**
		 * This will create the object of type `T` in-place by calling its constructor with the arguments provided.
		 * This will provide speed benefits by negating the need for copying or moving a potentially large object.
		 * If the size of the array exceeds the capacity, grow() will be called.
		 *
		 * **Time Complexity** = *O(1)* if growth is unnecessary otherwise *O(n)* where n is the number of elements
		 * in the existing array.
		 *
		 * @tparam Ts - dummy template parameters to hold the arguments for the object constructor.
		 * @param args - the arguments to be forwarded to the object of type `T`'s constructor.
		 * @return - a reference to the object created.
		 *
		 * @see grow()
		 */
		template<typename... Ts>
		T& emplace_back(Ts&& ... args) noexcept {
			if (mSize >= capacity)
				grow();
			new(&data[mSize]) T(std::forward<Ts>(
					args)...);  // Placement new will create and 'place' the object in already allocated memory.
			return data[mSize++];
		}

		/**
		 * Removes the element at the end of the array and calls its destructor. If the new size of the array is less
		 * than half of the capacity, shrink() will be called to reduce the capacity of the array. If the array is
		 * empty, a runtime exception will be thrown.
		 *
		 * **Time Complexity** = *O(1)* if no shrink is necessary, otherwise *O(n)* where n is the number of elements in
		 * the array.
		 *
		 * @see shrink()
		 */
		void pop_back() {
			if (mSize) {
				data[--mSize].~T();
				if (mSize < (capacity / 2))
					shrink();
			} else
				throw std::runtime_error("Vector is empty, there is nothing to pop.");
		}

		/**
		 * Returns a reference to the object at the beginning of the array. If the array is empty, a runtime
		 * exception will be thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a reference to the object of type `T` at the beginning of the array.
		 */
		T& front() {
			if (mSize)
				return data[0];
			throw std::runtime_error("Vector is empty, there is nothing at the front.");
		}

		/**
		 * Returns a const reference to the the object at the beginning of the array. This function will be relevant to
		 * const Vector objects. If the array is empty, a runtime exception will be thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a const reference to the object of type `T` at the beginning of the array.
		 */
		const T& front() const {
			if (mSize)
				return data[0];
			throw std::runtime_error("Vector is empty, there is nothing at the front.");
		}

		/**
		 * Returns a reference to the object at the end of the array. If the array is empty, a runtime exception
		 * will be thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a reference to the object of type `T` at the end of the array.
		 */
		T& back() {
			if (mSize)
				return data[mSize - 1];
			throw std::runtime_error("Vector is empty, there is nothing at the back");
		}

		/**
		 * Returns a const reference to the the object at the end of the array. This function will be relevant to
		 * const Vector objects. If the array is empty, a runtime exception will be thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a const reference to the object of type `T` at the end of the array.
		 */
		const T& back() const {
			if (mSize)
				return data[mSize - 1];
			throw std::runtime_error("Vector is empty, there is nothing at the back");
		}

		/**
		 * Returns the number of elements in the array.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - an unsigned integer representing the number of elements in the array.
		 */
		[[nodiscard]] size_t size() const noexcept {
			return mSize;
		}

		/**
		 * Returns a boolean value that indicates whether the Vector object's array is empty.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return  - a boolean value that indicates whether the Vector object's array is empty.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return mSize == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the array is not 0, otherwise
		 * it evaluates to `false`.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - the boolean value of whether the size of the array is 0.
		 */
		explicit operator bool() const noexcept {
			return mSize != 0;
		}

		/**
		 * Equivalence operator which compares two Vector objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain the same data.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current vector + the number of elements
		 * in the other vector.
		 *
		 * @param other - a Vector object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two arrays contain the same data.
		 */
		bool operator==(const Vector<T>& other) const noexcept {
			if (mSize != other.mSize)
				return false;
			for (size_t i = 0; i < mSize; ++i) {
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		/**
		 * Not-equivalence operator which compares two Vector objects of the same type `T`, element-wise, and returns
		 * a boolean value indicating whether the two objects contain different data.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the current vector + the number of elements
		 * in the other vector.
		 *
		 * @param other - a Vector object of the same type `T`, whose data to compare against.
		 * @return - a boolean value indicating whether the two arrays contain different data.
		 */
		bool operator!=(const Vector<T>& other) const noexcept {
			return !(*this == other);
		}

		/**
		 * Square brackets operator to access elements of a given index in the array.
		 * If the index provided is out of bounds, an invalid argument exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param index - an unsigned integer indicating the index in the array for whose data to return.
		 *
		 * @return - a reference, of type `T`, to the data at the element specified by index.
		 */
		[[nodiscard]] T& operator[](const size_t& index) {
			if (index >= 0 && index < capacity)
				return data[index];
			throw std::invalid_argument("Invalid index, out of range");
		}

		/**
		 * Square brackets operator used to access elements of a given index in the array.
		 * If the index provided is out of bounds, an invalid argument exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param index - an unsigned integer indicating the index in the array for whose data to return.
		 *
		 * @return - a const reference, of type `T`, to the data at the element specified by index.
		 */
		[[nodiscard]] const T& operator[](const size_t& index) const {
			if (index >= 0 && index < capacity)
				return data[index];
			throw std::invalid_argument("Invalid index, out of range");
		}

		/**
		 * Plus operator used to append another Vector objects of the same type `T` to the current object.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the `right` Vector object.
		 *
		 * @param right - a Vector object of the same type `T` to be appended to the current object.
		 *
		 * @return - a new Vector object containing the data of the current and `right` objects.
		 */
		[[nodiscard]] Vector<T> operator+(const Vector<T>& right) const noexcept {
			if (right.mSize) {
				Vector<T> res(*this);
				for (size_t i = 0; i < right.mSize; ++i)
					res.push_back(right[i]);
				return res;
			}
			return *this;
		}

		/**
		 * Creates and returns an iterator with the position of the beginning of the vector.
		 * **Time Complexity** = *O(1)*.
		 * @return - a VectorIterator object with the position of the beginning element of the vector.
		 */
		Iterator begin() const noexcept {
			return Iterator(data, data, &data[mSize]);
		}

		/**
		 * Creates and returns an iterator with the position past the end of the vector.
		 * **Time Complexity** = *O(1)*.
		 * @return - a VectorIterator object with the position past the ending element of the vector.
		 */
		Iterator end() const noexcept {
			return Iterator(&data[mSize], data, &data[mSize]);
		}

		/**
		 * This function calls the destructor of each object in the array and then sets the size member variable to 0.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the array.
		 */
		void clear() noexcept {
			for (size_t i = 0; i < mSize; ++i)
				data[i].~T();
			mSize = 0;
		}

		/**
		 * Destructor for the Vector class which calls clear() and then deallocates the memory of the vector.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the array.
		 */
		virtual ~Vector() {
			clear();
			::operator delete(data, capacity * sizeof(T));
		}

	private:
		size_t mSize;  /**< An unsigned integer representing the number of elements in the array. */
		size_t capacity; /**< An unsigned integer representing the number of elements for which memory is allocated. */
		T* data;  /**< A pointer of type `T` which points to the beginning of the array. */

		/**
		 * Grows the array and copies, or moves if possible, the elements from the old array to the new array.
		 * If the array is not initialised, memory for an array with the default capacity of 10 is allocated.
		 * Otherwise, memory for a new array with a capacity one and a half times larger is allocated.
		 * The old array's objects are destroyed and its memory released.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the array.
		 */
		void grow() noexcept {
			if (!data) {
				capacity = 1;
				data = (T*)::operator new(
						capacity * sizeof(T));// Allocates memory without calling constructor, analogous to malloc
				return;
			}
			size_t new_capacity = capacity + capacity / 2;
			T* new_data = (T*)::operator new(new_capacity * sizeof(T));

			for (size_t i = 0; i < mSize; ++i) {
				new_data[i] = std::move(data[i]);
				data[i].~T();
			}

			::operator delete(data, capacity * sizeof(T));// deallocated memory without calling destructor
			data = new_data;
			capacity = new_capacity;
		}

		/**
		 * Function used to grow the capacity of the array after the use of an initialiser list.
		 * Performs the same operation as grow() but with the new capacity provided.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the array.
		 *
		 * @param cap - the new capacity of the array.
		 */
		void init_grow(size_t cap) noexcept {
			if (!data) {
				capacity = cap;
				data = (T*)::operator new(
						capacity * sizeof(T));
				return;
			}
			T* new_data = (T*)::operator new(cap * sizeof(T));

			for (size_t i = 0; i < mSize; ++i) {
				new_data[i] = std::move(data[i]);
				data[i].~T();
			}

			::operator delete(data, capacity * sizeof(T));
			data = new_data;
			capacity = cap;
		}

		/**
		 * Shrinks the capacity of the array by copying, or moving if possible. the elements of the array into a
		 * smaller array and then releasing the memory of the old array.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the array.
		 */
		void shrink() noexcept {
			size_t new_capacity = capacity - capacity / 2;
			T* new_data = (T*)::operator new(new_capacity * sizeof(T));
			for (size_t i = 0; i < mSize; ++i) {
				new_data[i] = std::move(data[i]);
				data[i].~T();
			}

			::operator delete(data, capacity * sizeof(T));
			data = new_data;
			capacity = new_capacity;
		}
	};
}// namespace custom

#endif// VECTOR_H