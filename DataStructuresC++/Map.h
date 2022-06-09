#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

namespace Map {

	template<typename T>
	struct Value {
		T data;

		Value(const T& data) : data(data) {}
	};

	template<typename T, typename U>
	struct Key {
		U id;
		Value<T>* value;

		Key(U id, Value<T>* val = nullptr) : id(id), value(val) {}
	};

	template<typename T, typename U = std::string>
	class Map {
	public:
		Map() {
			key_list = {};
		}

		Map(const U& id, const T& data) {
			key_list = {};
			Value<T>* val = new Value<T>(data);
			Key<T, U>* key = new Key<T, U>(id, val);
			key_list.push_back(key);
		}

		void add(const U& id, const T& data) {
			for (Key<T, U>*& key : key_list) {
				if (key->id == id) {
					throw std::invalid_argument("Key provided already exists");
					return;
				}
			}
			Value<T>* val = new Value<T>(data);
			Key<T, U>* key = new Key<T, U>(id, val);
			key_list.push_back(key);
		}

		int get(const U& id) const {
			for (Key<T, U>* key : key_list) {
				if (key->id == id)
					return key->value->data;
			}
			throw std::invalid_argument("Id provided not found");
			return -1;
		}

		bool exists(const U& id) const {
			for (Key<T, U>* key : key_list) {
				if (key->id == id)
					return true;
			}
			return false;
		}

		void change(const U& id, const T& data) {
			for (Key<T, U>*& key : key_list) {
				if (key->id == id) {
					key->value->data = data;
					return;
				}
			}
			throw std::invalid_argument("Id provided not found");
		}

		std::vector<std::pair<U, T>> contents() const {
			std::vector<std::pair<U, T>> ret = {};
			if (key_list.empty()) {
				throw std::runtime_error("Map is empty, there is no content");
				return ret;
			}
			for (Key<T, U>* key : key_list) {
				ret.push_back(std::pair<U, T>(key->id, key->value->data));
			}
			return ret;
		}

		void print() const {
			if (key_list.empty()) {
				throw std::runtime_error("Map is empty, there is nothing to print");
				return;
			}
			for (Key<T, U>* key : key_list) {
				std::cout << key->id << " : " << key->value->data << "\n";
			}
		}

		void remove(const U& id) {
			if (key_list.empty()) {
				throw std::invalid_argument("Id provided not found");
				return;
			}
			int index = 0;
			for (Key<T, U>*& key : key_list) {
				if (key->id == id) {
					delete key->value;
					delete key;
					key_list.erase(key_list.begin()+index);
					return;
				}
				index++;
			}
			throw std::invalid_argument("Id provided not found");
		}

		void clear() {
			for (Key<T, U>*& key : key_list) {
				delete key->value;
				delete key;
			}
			key_list.clear();
		}

		~Map() {
			if (!key_list.empty())
				clear();
		}

	private:
		std::vector<Key<T, U>*> key_list;
	};
}