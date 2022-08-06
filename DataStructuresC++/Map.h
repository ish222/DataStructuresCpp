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
	template<typename U, typename T, typename hasher = std::hash<U>>
	class Map {
	public:
		explicit Map(size_t cap = 12) noexcept : capacity(cap), mSize(0), key_list(cap), hash_table(cap) {}

		Map(const U& id, const T& data, size_t cap = 12) noexcept : capacity(cap), mSize(1), key_list(cap), hash_table(cap) {
			size_t hash_value = hash(id) % capacity;
			hash_table[hash_value].push_back(data);
			key_list[hash_value].push_back(id);
		}

		Map(U&& id, T&& data, size_t cap = 12) noexcept : capacity(cap), mSize(1), hash_table(cap) {
			size_t hash_value = hash(std::move(id)) % capacity;
			hash_table[hash_value] = std::move(data);
			key_list[hash_value].push_back(std::move(id));
		}

		Map(const Map<U, T>& other) noexcept : capacity(other.capacity), mSize(other.mSize), key_list(other.key_list.begin(), other.key_list.end()), hash_table(other.hash_table.begin(), other.hash_table.end()) {}

		Map& operator=(const Map<U, T>& other) {
			if (this != &other) {
				if (!key_list.empty())
					clear();
				capacity = other.capacity;
				mSize = other.mSize;
				key_list = other.key_list;
				hash_table = other.hash_table;
			}
			return *this;
		}

		Map(Map<U, T>&& other) noexcept : capacity(other.capacity), mSize(other.mSize), key_list(other.capacity), hash_table(other.capacity) {
			hash_table.insert(hash_table.end(), std::make_move_iterator(other.hash_table.begin()), std::make_move_iterator(other.hash_table.end()));
			other.hash_table.clear();
			other.hash_table.shrink_to_fit();// Frees memory allocated to the vector
			key_list.insert(key_list.end(), std::make_move_iterator(other.key_list.begin()), std::make_move_iterator(other.key_list.end()));
			other.key_list.clear();
			other.key_list.shrink_to_fit();
			other.capacity = 0;
		}

		Map& operator=(Map<U, T>&& other) noexcept {
			if (this != &other) {
				if (!hash_table.empty())
					clear();
				capacity = other.capacity;
				mSize = other.mSize;
				hash_table.insert(hash_table.end(), std::make_move_iterator(other.hash_table.begin()), std::make_move_iterator(other.hash_table.end()));
				other.hash_table.clear();
				other.hash_table.shrink_to_fit();
				key_list.insert(key_list.end(), std::make_move_iterator(other.key_list.begin()), std::make_move_iterator(other.key_list.end()));
				other.key_list.clear();
				other.key_list.shrink_to_fit();
				other.capacity = 0;
			}
			return *this;
		}

		void add(const U& id, const T& data) {
			if (!exists(id)) {
				size_t hash_value = hash(id) % capacity;
				hash_table[hash_value].push_back(data);
				key_list[hash_value].push_back(id);
				++mSize;
				return;
			}
			throw std::invalid_argument("Key provided already exists");
		}

		void add(const U& id, T&& data) {
			if (!exists(id)) {
				size_t hash_value = hash(id) % capacity;
				hash_table[hash_value].push_back(std::move(data));
				key_list[hash_value].push_back(id);
				++mSize;
				return;
			}
			throw std::invalid_argument("Key provided already exists");
		}

		[[nodiscard]] T& at(const U& id) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
			if (it != key_list[hash_value].end()) {
				size_t index = it - key_list[hash_value].begin();
				return hash_table[hash_value][index];
			}
			throw std::invalid_argument("Id provided not found");
		}

		[[nodiscard]] const T& at(const U& id) const {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
			if (it != key_list[hash_value].end()) {
				size_t index = it - key_list[hash_value].begin();
				return hash_table[hash_value][index];
			}
			throw std::invalid_argument("Id provided not found");
		}

		[[nodiscard]] bool exists(const U& id) const noexcept {
			if (mSize) {
				size_t hash_value = hash(id) % capacity;
				return std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id) != key_list[hash_value].end();
			}
			return false;
		}

		[[nodiscard]] bool empty() const noexcept {
			return mSize == 0;
		}

		explicit operator bool() const noexcept {
			return (bool)mSize;
		}

		[[nodiscard]] size_t size() const noexcept {
			return mSize;
		}

		void change(const U& id, const T& data) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
			if (it != key_list[hash_value].end()) {
				size_t index = it - key_list[hash_value].begin();
				hash_table[hash_value][index] = data;
				return;
			}
			throw std::invalid_argument("Id provided not found");
		}

		void change(const U& id, T&& data) {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
			if (it != key_list[hash_value].end()) {
				size_t index = it - key_list[hash_value].begin();
				hash_table[hash_value][index] = std::move(data);
				return;
			}
			throw std::invalid_argument("Id provided not found");
		}

		T& operator[](const U& id) noexcept {
			size_t hash_value = hash(id) % capacity;
			auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
			if (it != key_list[hash_value].end()) {
				size_t index = it - key_list[hash_value].begin();
				return hash_table[hash_value][index];
			}
			return add_op(id, T());
		}

		[[nodiscard]] std::vector<std::pair<U, T>> contents() const noexcept {
			std::vector<std::pair<U, T>> ret = {};
			if (mSize) {
				for (int i = 0; i < capacity; ++i) {
					for (int j = 0; j < hash_table[i].size(); ++j) {
						ret.push_back(std::pair<U, T>(key_list[i][j], hash_table[i][j]));
					}
				}
			}
			return ret;
		}

		void print() const {
			if (mSize) {
				std::vector<std::pair<U, T>> data = contents();
				for (const auto& [key, value]: data)
					std::cout << key << " : " << value << "\n";
			} else
				throw std::runtime_error("Map is empty, there is nothing to print");
		}

		void remove(const U& id) {
			if (mSize) {
				size_t hash_value = hash(id) % capacity;
				auto it = std::find(key_list[hash_value].begin(), key_list[hash_value].end(), id);
				if (it != key_list[hash_value].end()) {
					size_t index = it - key_list[hash_value].begin();
					key_list[hash_value].erase(it);
					hash_table[hash_value].erase(hash_table[hash_value].begin() + index);
					--mSize;
					return;
				}
			}
			if (mSize == 0)
				throw std::runtime_error("Map is empty, there is nothing to remove");
			throw std::invalid_argument("Id provided not found");
		}

		void clear() noexcept {
			hash_table.clear();
			key_list.clear();
		}

		virtual ~Map() {
			if (mSize)
				clear();
		}

	private:
		size_t capacity;
		size_t mSize;
		std::vector<std::vector<U>> key_list;
		std::vector<std::vector<T>> hash_table;
		hasher hash;

		T& add_op(const U& id, T&& data) noexcept {
			size_t hash_value = hash(id) % capacity;
			hash_table[hash_value].push_back(std::move(data));
			key_list[hash_value].push_back(std::move(id));
			++mSize;
			return hash_table[hash_value].back();
		}
	};
}// namespace custom

#endif// MAP_H