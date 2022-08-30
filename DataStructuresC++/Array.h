#ifndef DATA_STRUCTURES_CPP_ARRAY_H
#define DATA_STRUCTURES_CPP_ARRAY_H

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <utility>

#include "Vector.h"

namespace custom {
	/**
	 * A templated array class which provides an alternative to using C-style arrays by making
	 * it easier and simpler to work with arrays through the use of member variables and functions.
	 * The array is a static data structure meaning its size cannot be changed once initialized.
	 * @tparam T - the type of the object to be stored in the array.
	 * @tparam alloc_size - an unsigned integer representing the total capacity of the array.
	 */
	template<typename T, size_t alloc_size>
	class Array {
	public:
		using Type = T;  /**< An alias for the type of data `T` to be used by external utility classes. */
		using Iterator = VectorIterator<Array>;  /**< An alias for the array iterator class. */

		friend class VectorIterator<Array>;  /**< Friend array iterator class, allowing it to access private members. */

	public:
		/**
		 * Default array constructor which allocates memory for the specified number of elements
		 * of type T. Sets the member mSize to the capacity specified.
		 */
		constexpr Array() noexcept : data(new T[alloc_size]), mSize{alloc_size} {}

		/**
		 * Overloaded initializer list constructor which sets the elements of the array using an
		 * initializer list provided.
		 * The size of the initializer list must be less than or equal to the capacity of the array.
		 * @param init - the initialiser list whose contents will be added to the array.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		constexpr Array(std::initializer_list<T> init) noexcept : data(new T[alloc_size]), mSize{alloc_size} {
			assert(mSize >= init.size());
			for (size_t i = 0; i < init.size(); ++i)
				data[i] = std::move(*(init.begin()+i));
		}

		/**
		 * Array copy constructor, which copies the elements of another array of type T into this array.
		 * @param other - an array object with data of the same type, `T`.
		 */
		constexpr Array(const Array& other) noexcept : data(new T[other.mSize]), mSize{other.mSize} {
			for (size_t i = 0; i < mSize; ++i)
				data[i] = other.data[i];
		}

		/**
		 * Copy assignment operator which copies another array object's contents into the current array.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * @param other - the array object to copy from.
		 * @return - a reference to the current array object.
		 */
		constexpr Array& operator=(const Array& other) noexcept {
			if (this != &other) {
				if (mSize)
					delete[] data;
				mSize = other.mSize;
				data = new T[mSize];
				for (size_t i = 0; i < mSize; ++i)
					data[i] = other.data[i];
			}
			return *this;
		}

		/**
		 * Array move constructor, which moves the elements of another array of type T into this array and
		 * sets the other array to its default state.
		 * @param other - an `r-value reference` to the array to be moved.
		 */
		constexpr Array(Array&& other) noexcept : data(other.data), mSize{other.mSize} {
			other.data = nullptr;
			other.mSize = 0;
		}

		/**
		 * Move assignment operator which moves another array object's contents into the current array and
		 * sets the other array to its default state.
		 * \note
		 * If the current object, that is being moved into, is not empty, **it will be cleared**.
		 * @param other - an `r-value reference` to the array object to be moved.
		 * @return - a reference to the current array object.
		 */
		constexpr Array& operator=(Array&& other) noexcept {
			if (this != &other) {
				if (mSize)
					delete[] data;
				data = other.data;
				mSize = other.mSize;
				other.data = nullptr;
				other.mSize = 0;
			}
			return *this;
		}

		/**
		 * Square bracket operator which allows for access to the data of an element at the
		 * specified index in the array. If the index is out of bounds, the program is terminated.
		 * @param index - an unsigned integer representing the index of the element to access.
		 * @return - a reference to the data at the specified index.
		 */
		constexpr T& operator[](const size_t& index) noexcept {
			assert(index < mSize);
			return data[index];
		}

		/**
		 * Square bracket operator which allows for access to the data of an element at the
		 * specified index in the array. If the index is out of bounds, the program is terminated.
		 * @param index - an unsigned integer representing the index of the element to access.
		 * @return - a const reference to the data at the specified index.
		 */
		constexpr const T& operator[](const size_t& index) const noexcept {
			assert(index < mSize);
			return data[index];
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the size of the array is not 0, otherwise
		 * it evaluates to `false`.
		 *
		 * @return - the boolean value of whether the size of the array is 0.
		 */
		constexpr explicit operator bool() const noexcept {
			return mSize != 0;
		}

		/**
		 * Returns the number of elements in the array.
		 *
		 * @return - an unsigned integer representing the capacity of the array.
		 */
		[[nodiscard]] constexpr size_t size() const noexcept {
			return mSize;
		}

		/**
		 * Creates and returns an iterator with the position of the beginning of the array.
		 * @return - a VectorIterator object with the position of the beginning element of the array.
		 */
		Iterator begin() const noexcept {
			return Iterator(data, data, &data[mSize]);
		}

		/**
		 * Creates and returns an iterator with the position past the end of the array.
		 * @return - a VectorIterator object with the position past the ending element of the array.
		 */
		Iterator end() const noexcept {
			return Iterator((&data[mSize]), data, (&data[mSize]));
		}

		/**
		 * Array destructor which calls calls the destructor of each element in the array
		 * and frees its allocated memory.
		 */
		constexpr virtual ~Array() {
			delete[] data;
		}

	private:
		T* data;  /**< A pointer of type `T` which points to the beginning of the array. */
		size_t mSize;  /**< An unsigned integer representing the number of elements in the array. */
	};
}

#endif//DATA_STRUCTURES_CPP_ARRAY_H
