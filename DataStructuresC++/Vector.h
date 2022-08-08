#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace custom {
	/**
	 * An template implementation of a dynamic array. Automatically grows and shrinks
	 * its capacity based on the number of elements in the array.
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
		/**
		 * Default constructor of the Vector class. Allocates memory on the heap for the capacity provided.
		 * This method is noexcept, meaning that failure to allocate memory or
		 * a <a href="https://en.cppreference.com/w/cpp/memory/new/bad_alloc">`std::bad_alloc`</a>
		 * exception will terminate the program.
		 *
		 * @param capacity - an unsigned integer to specify the total capacity of the array at initialization.
		 */
		explicit Vector(size_t capacity = 10) noexcept: capacity(capacity), mSize(0) {
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
			for (int i = 0; i < mSize; ++i)
				data[i] = *(init.begin() + i);
		}

		/**
		 * Copy constructor for the Vector class. This will create a new Vector object and perform an element wise,
		 * deep copy of another vector of the same type `T`.
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
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
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
		 * @return - an unsigned integer representing the number of elements in the array.
		 */
		[[nodiscard]] size_t size() const noexcept {
			return mSize;
		}

		/**
		 * Returns a boolean value that indicates whether the Vector object's array is empty.
		 *
		 * @return  - a boolean value that indicates whether the Vector object's array is empty.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return (bool)mSize;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the array is not 0, otherwise
		 * it evaluates to `false`.
		 *
		 * @return - the boolean value of whether the size of the array is 0.
		 */
		explicit operator bool() const noexcept {
			return mSize != 0;
		}

		/**
		 * Square brackets operator to access elements of a given index in the array.
		 * If the index provided is out of bounds, an invalid argument exception is thrown.
		 *
		 * @param index - an unsigned integer indicating the index in the array for whose data to return.
		 *
		 * @return - a reference, of type `T`, to the data at the element specified by index.
		 */
		[[nodiscard]] T& operator[](const size_t& index) {
			if (index >= 0 && index < mSize)
				return data[index];
			throw std::invalid_argument("Invalid index, out of range");
		}

		/**
		 * Square brackets operator used to access elements of a given index in the array.
		 * If the index provided is out of bounds, an invalid argument exception is thrown.
		 *
		 * @param index - an unsigned integer indicating the index in the array for whose data to return.
		 *
		 * @return - a const reference, of type `T`, to the data at the element specified by index.
		 */
		[[nodiscard]] const T& operator[](const size_t& index) const {
			if (index >= 0 && index < mSize)
				return data[index];
			throw std::invalid_argument("Invalid index, out of range");
		}

		/**
		 * Plus operator used to append another Vector objects of the same type `T` to the current object.
		 *
		 * @param right - a Vector object of the same type `T` to be appended to the current object.
		 *
		 * @return - a new Vector object containing the data of the current and `right` objects.
		 */
		[[nodiscard]] Vector<T> operator+(const Vector<T>& right) const noexcept {
			if (right.mSize) {
				Vector<T> res(*this);
				for (size_t i = 0; i < right.mSize; ++i)
					res.append(right[i]);
				return res;
			}
			return *this;
		}

		/**
		 * This function calls the destructor of each object in the array and then sets the size member variable to 0.
		 */
		void clear() noexcept {
			for (size_t i = 0; i < mSize; ++i)
				data[i].~T();
			mSize = 0;
		}

		/**
		 * Destructor for the Vector class which calls clear() and then deallocates the memory of the vector.
		 */
		virtual ~Vector() {
			clear();
			::operator delete(data, capacity * sizeof(T));
		}

	private:
		/** \brief An unsigned integer representing the number of elements in the array. */
		size_t mSize;
		/** \brief An unsigned integer representing the number of elements for which memory is allocated. */
		size_t capacity;
		/** \brief A pointer of type `T` which points to the beginning of the array. */
		T* data;

		/**
		 * Grows the array and copies, or moves if possible, the elements from the old array to the new array.
		 * If the array is not initialised, memory for an array with the default capacity of 10 is allocated.
		 * Otherwise, memory for a new array with a capacity one and a half times larger is allocated.
		 * The old array's objects are destroyed and its memory released.
		 */
		void grow() noexcept {
			if (!data) {
				capacity = 10;
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