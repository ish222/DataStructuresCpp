#ifndef MAP_H
#define MAP_H

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

namespace custom {
	/**
	 * A template implementation of an unordered map, also known as a hash table, data structure. Each element of the
	 * map has a key, used to access the element's value, of type `U` and a value of type 'T'. The key of each element
	 * must be unique. For hashing, the default hash function of `std::hash` of type `U` is used.
	 * \note
	 * The key type `U` must be hashable.
	 * @tparam U - the type of the key used to access an element's value.
	 * @tparam T - the type of the value of each element.
	 * @tparam hasher - the hashing function used on the key of type `U`, set by default to `std::hash<U>`.
	 * @see <a href="https://en.cppreference.com/w/cpp/utility/hash">std::hash</a>
	 * @see <a href="https://en.wikipedia.org/wiki/Hash_table">Hash table</a>
	 */
	template<typename U, typename T, typename hasher = std::hash<U>>
	class Map {
	public:
		/**
		 * Default Map constructor which sets the number of hash buckets with the specified value of capacity, which
		 * has a default value of 12, and initializes the hash table to an empty `std::vector` object, of type
		 * `std::list<std::pair<U, T>>` with the capacity specified.
		 * @param cap - an unsigned integer specifying the initial capacity of the hash table.
		 */
		explicit Map(size_t cap = 12) noexcept: capacity(cap), mSize(0), hash_table(cap) {}

		/**
		 * Overloaded Map constructor which adds an element to the map with the key and value specified and sets the
		 * capacity, i.e. number of hash buckets, of the map to the specified value, with a default value of 12.
		 * @param id - a key of type `U` to be hashed and copied into the hash table.
		 * @param data - value of type `T` to be copied into the hash table.
		 * @param cap - an unsigned integer specifying the initial capacity of the hash table.
		 */
		Map(const U& id, const T& data, size_t cap = 12) noexcept: capacity(cap), mSize(1),
		                                                           hash_table(cap) {
			size_t hash_value = hash(id) %
			                    capacity;  // Calculate the index of the element. using its hash value and the map capacity.
			hash_table[hash_value].push_back(
					{id, data});  // Adds a pair containing the key and value to the list at the hash index.
		}

		/**
		 * Overloaded Map constructor which adds an element to the map with the key and value specified and sets the
		 * capacity, i.e. number of hash buckets, of the map to the specified value, with a default value of 12.
		 * @param id - a *r-value reference* to a key of type `U` to be hashed and moved into the hash table.
		 * @param data - a *r-value reference* to a value of type `T` to be moved into the hash table.
		 * @param cap - an unsigned integer specifying the initial capacity of the hash table.
		 */
		Map(U&& id, T&& data, size_t cap = 12) noexcept: capacity(cap), mSize(1), hash_table(cap) {
			size_t hash_value = hash(std::move(id)) % capacity;
			hash_table[hash_value].push_back({id, data});
		}

		/**
		 * Map copy constructor which performs a deep copy of another map object of the same types `U` and `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other map.
		 *
		 * @param other - another map object of the same types `U` and `T` to copy.
		 */
		Map(const Map<U, T>& other) noexcept: capacity(other.capacity), mSize(other.mSize),
		                                      hash_table(other.hash_table.begin(), other.hash_table.end()) {}

		/**
		 * Map copy assignment operator which copies another Map object of the same types `U` and `T` into the current
		 * object.
		 *
		 * \note
		 * If the current object is initialized, it will be cleared before copying the other object.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other map.
		 *
		 * @param other - another map object of the same types `U` and `T` to copy.
		 * @return - a reference to the current object.
		 */
		Map& operator=(const Map<U, T>& other) {
			if (this != &other) {
				if (!hash_table.empty())
					clear();
				capacity = other.capacity;
				mSize = other.mSize;
				hash_table = other.hash_table;
			}
			return *this;
		}

		/**
		 * Map move constructor which moves another map object of the same types `U` and `T` to this new map object.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other map.
		 *
		 * @param other - a *r-value reference* to another map object of the same types `U` and `T` to move.
		 */
		Map(Map<U, T>&& other) noexcept: capacity(other.capacity), mSize(other.mSize),
		                                 hash_table(other.capacity) {
			hash_table.insert(hash_table.end(), std::make_move_iterator(other.hash_table.begin()),
			                  std::make_move_iterator(other.hash_table.end()));
			other.hash_table.clear();
			other.hash_table.shrink_to_fit();// Frees memory allocated to the vector
			other.capacity = 0;
		}

		/**
		 * Map move assignment operator which moves another Map object of the same types `U` and `T` into the current
		 * object.
		 *
		 * \note
		 * If the current object is initialized, it will be cleared before copying the other object.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the other map.
		 *
		 * @param other - a *r-value reference* to another map object of the same types `U` and `T` to move.
		 * @return - a reference to the current object.
		 */
		Map& operator=(Map<U, T>&& other) noexcept {
			if (this != &other) {
				if (!hash_table.empty())
					clear();
				capacity = other.capacity;
				mSize = other.mSize;
				hash_table.insert(hash_table.end(), std::make_move_iterator(other.hash_table.begin()),
				                  std::make_move_iterator(other.hash_table.end()));
				other.hash_table.clear();
				other.hash_table.shrink_to_fit();
				other.capacity = 0;
			}
			return *this;
		}

		/**
		 * Adds a **new** key-value pair, of types `U` and `T`, to the map.
		 *
		 * If the key already exists in the map, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param id - a key of type `U` to be hashed and copied into the hash table.
		 * @param data - value of type `T` to be copied into the hash table.
		 */
		void add(const U& id, const T& data) {
			if (!exists(id)) {
				size_t hash_value = hash(id) % capacity;
				hash_table[hash_value].push_back({id, data});
				++mSize;
				return;
			}
			throw std::invalid_argument("Key provided already exists");
		}

		/**
		 * Adds a **new** key-value pair, of types `U` and `T`, to the map.
		 *
		 * If the key already exists in the map, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param id - a *r-value reference* to a key of type `U` to be hashed and moved into the hash table.
		 * @param data - a *r-value reference* to a value of type `T` to be moved into the hash table.
		 */
		void add(U&& id, T&& data) {
			if (!exists(id)) {
				size_t hash_value = hash(id) % capacity;
				hash_table[hash_value].push_back({id, data});
				++mSize;
				return;
			}
			throw std::invalid_argument("Key provided already exists");
		}

		/**
		 * Obtains the value, of type `T`, at a specified key, of type `U`.
		 *
		 * If the key is not found in the map, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U`, to obtain the value of.
		 * @return - a reference to the value, of type `T`, at the specified key.
		 */
		[[nodiscard]] T& at(const U& id) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
			                       [&](const auto& element) {
				                       return element.first == id;
			                       });
			if (it != hash_table[hash_value].end()) {
				return it->second;
			}
			throw std::invalid_argument("Id provided not found");
		}

		/**
		 * Obtains the value, of type `T`, at a specified key, of type `U`.
		 *
		 * If the key is not found in the map, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U`, to obtain the value of.
		 * @return - a const reference to the value, of type `T`, at the specified key.
		 */
		[[nodiscard]] const T& at(const U& id) const {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
			                       [&](const auto& element) {
				                       return element.first == id;
			                       });
			if (it != hash_table[hash_value].end())
				return it->second;
			throw std::invalid_argument("Id provided not found");
		}

		/**
		 * Checks the map to see if an element with the given key exists.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U`, to search in the map for.
		 * @return - a boolean value indicating whether an element with the given key exists.
		 */
		[[nodiscard]] bool exists(const U& id) const noexcept {
			if (mSize) {
				size_t hash_value = hash(id) % capacity;
				return std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
				                    [&](const auto& element) {
					                    return element.first == id;
				                    }) != hash_table[hash_value].end();
			}
			return false;
		}

		/**
		 * Provides a boolean value that indicates whether the map is empty and uninitialised.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a boolean value that indicates whether the hash table is empty.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return mSize == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to true if the hash table is not empty.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a boolean value indicating whether the hash table is empty.
		 */
		explicit operator bool() const noexcept {
			return (bool)mSize;
		}

		/**
		 * Returns the number of elements in the map.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - an unsigned integer specifying the number of elements in the map.
		 */
		[[nodiscard]] size_t size() const noexcept {
			return mSize;
		}

		/**
		 * Changes the value, of type `T`, for a given key, of type `U`.
		 *
		 * If an element with the key provided is not found, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U` to change the value of.
		 * @param data - a value of type `T` to change the key's value to.
		 */
		void change(const U& id, const T& data) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
			                       [&](const auto& element) {
				                       return element.first == id;
			                       });
			if (it != hash_table[hash_value].end()) {
				it->second = data;
				return;
			}
			throw std::invalid_argument("Id provided not found");
		}

		/**
		 * Changes the value, of type `T`, for a given key, of type `U`.
		 *
		 * If an element with the key provided is not found, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U` to change the value of.
		 * @param data - a *r-value reference* to a value of type `T` to change the key's value to.
		 */
		void change(const U& id, T&& data) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
			                       [&](const auto& element) {
				                       return element.first == id;
			                       });
			if (it != hash_table[hash_value].end()) {
				it->second = std::move(data);
				return;
			}
			throw std::invalid_argument("Id provided not found");
		}

		/**
		 * Square brackets operator which returns the value, of type `T` of a given key, of type `U`. If an element
		 * with the key specified does not exist, a new element is created with a default constructed value.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U`, to obtain the value of, or add to the map.
		 * @return - a reference to the value of the key specified.
		 */
		T& operator[](const U& id) noexcept {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
			                       [&](const auto& element) {
				                       return element.first == id;
			                       });
			if (it != hash_table[hash_value].end())
				return it->second;
			return add_op(id, T());
		}

		/**
		 * Adds the contents of the whole map with each element represented by a `std::pair` of types `U` and `T`, into
		 * a `std::vector`, and returns this vector.
		 *
		 * **Time Complexity** = *O(n)* where n is the number element in the map.
		 *
		 * @return - a `std::vector` containing a `std::pair` of the key and value of each element.
		 */
		[[nodiscard]] std::vector<std::pair<U, T>> contents() const noexcept {
			std::vector<std::pair<U, T>> ret = {};
			if (mSize) {
				for (int i = 0; i < capacity; ++i) {
					for (auto pair: hash_table[i])
						ret.push_back(pair);
				}
			}
			return ret;
		}

		/**
		 * Calls `std::cout` on each element in the map, printing its key and value.
		 *
		 * If the map is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * \note
		 * The type `T` must be compatible with `std::cout`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of elements in the map.
		 *
		 * @see <a href="https://en.cppreference.com/w/cpp/io/cout">std::cout</a>
		 */
		void print() const {
			if (mSize) {
				std::vector<std::pair<U, T>> data = contents();
				for (const auto& [key, value]: data)
					std::cout << key << " : " << value << "\n";
			} else
				throw std::runtime_error("Map is empty, there is nothing to print");
		}

		/**
		 * Removes an element with the specified key, of type `U`, from the map.
		 *
		 * If an element with the specified key is not found in the map, an `invalid_argument` exception is thrown.
		 *
		 * If the map is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*, (*(O(n)* if there are large amounts of hash collisions).
		 *
		 * @param id - the key, of type `U`, of the element to remove.
		 */
		void remove(const U& id) {
			if (mSize) {
				size_t hash_value = hash(id) % capacity;
				auto it = std::find_if(hash_table[hash_value].begin(), hash_table[hash_value].end(),
				                       [&](const auto& element) {
					                       return element.first == id;
				                       });
				if (it != hash_table[hash_value].end()) {
					hash_table[hash_value].erase(it);
					--mSize;
					return;
				}
			}
			if (mSize == 0)
				throw std::runtime_error("Map is empty, there is nothing to remove");
			throw std::invalid_argument("Id provided not found");
		}

		/**
		 * Clears the hash table of the map.
		 */
		void clear() noexcept {
			hash_table.clear();
		}

		/**
		 * Map destructor which clears the hash table of the map, if the map is initialized.
		 */
		virtual ~Map() {
			if (mSize)
				clear();
		}

	private:
		size_t capacity;  /**< An unsigned integer representing the number of buckets in the hash table. */
		size_t mSize;  /**< An unsigned integer representing the number of elements in the map. */
		std::vector<std::list<std::pair<U, T>>> hash_table; /**< The hash table containing all the elements of the map, stored in their hashed indices. */
		hasher hash;  /**< A hash object created from the `hasher` template argument, which can act as a functor to hash a given id. */

		/**
		 * Private helper function which adds a new element to the map, called from the operator[] function.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param id - the key, of type `U`, of the new element.
		 * @param data - the value, of type `T`, of the new element.
		 * @return - a reference to the new element's value.
		 */
		T& add_op(const U& id, T&& data) noexcept {
			size_t hash_value = hash(id) % capacity;
			hash_table[hash_value].push_back({id, data});
			++mSize;
			return hash_table[hash_value].back().second;
		}
	};
}// namespace custom

#endif// MAP_H