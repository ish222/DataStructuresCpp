#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

namespace custom {
    template<typename T, typename U = std::string>
    class Map {
    public:
        Map() : key_list({}) {}

        Map(const U& id, const T& data) {
            key_list = {};
            Value* val = new Value(data);
            Key* key = new Key(id, val);
            key_list.push_back(key);
        }

        Map(const Map& other) {
            key_list = {};
            for (const Key*& key: key_list)
                add(key->id, key->value->data);
        }

        Map& operator=(const Map& other) {
            if (this != &other) {
                if (!key_list.empty())
                    clear();
                for (const Key*& key: key_list)
                    add(key->id, key->value->data);
            }
            return *this;
        }

        Map(Map&& other) noexcept {
            key_list = other.key_list;
            other.key_list.clear();
        }

        Map& operator=(Map&& other) noexcept {
            if (this != &other) {
                if (!key_list.empty())
                    clear();
                key_list = other.key_list;
                other.key_list.clear();
            }
            return *this;
        }

        void add(const U& id, const T& data) {
            if (exists(id))
                throw std::invalid_argument("Key provided already exists");
            Value* val = new Value(data);
            Key* key = new Key(id, val);
            key_list.push_back(key);
        }

        T get(const U& id) const {
            for (Key* key: key_list) {
                if (key->id == id)
                    return key->value->data;
            }
            throw std::invalid_argument("Id provided not found");
        }

        bool exists(const U& id) const {
            for (const Key* key: key_list) {
                if (key->id == id)
                    return true;
            }
            return false;
        }

        bool empty() const noexcept {
            return key_list.empty();
        }

        operator bool() const noexcept {
            return !key_list.empty();
        }

        size_t size() const noexcept {
            return key_list.size();
        }

        void change(const U& id, const T& data) {
            for (Key*& key: key_list) {
                if (key->id == id) {
                    key->value->data = data;
                    return;
                }
            }
            throw std::invalid_argument("Id provided not found");
        }

        T& operator[](const U& id) {
            for (const Key* key: key_list) {
                if (key->id == id)
                    return key->value->data;
            }
            Value* val = new Value(T());
            Key* key = new Key(id, val);
            key_list.push_back(key);
            return val->data;
        }

        std::vector<std::pair<U, T>> contents() const {
            if (!key_list.empty()) {
                std::vector<std::pair<U, T>> ret = {};
                for (const Key* key: key_list) {
                    ret.push_back(std::pair<U, T>(key->id, key->value->data));
                }
                return ret;
            }
            throw std::runtime_error("Map is empty, there is no content");
        }

        void print() const {
            if (!key_list.empty()) {
                for (const Key* key: key_list)
                    std::cout << key->id << " : " << key->value->data << "\n";
            }
            else throw std::runtime_error("Map is empty, there is nothing to print");
        }

        void remove(const U& id) {
            if (!key_list.empty()) {
                int index = 0;
                for (Key*& key: key_list) {
                    if (key->id == id) {
                        delete key->value;
                        delete key;
                        key_list.erase(key_list.begin() + index);
                        return;
                    }
                    ++index;
                }
            }
            if (key_list.empty())
                throw std::runtime_error("Map is empty, there is nothing to remove");
            throw std::invalid_argument("Id provided not found");
        }

        void clear() {
            if (!key_list.empty()) {
                for (Key*& key: key_list) {
                    delete key->value;
                    delete key;
                }
                key_list.clear();
            }
            else throw std::runtime_error("Map is empty, there is nothing to clear");
        }

        ~Map() {
            if (!key_list.empty())
                clear();
        }

    private:
        struct Value {
            T data;

            Value(const T& data) : data(data) {}
        };

        struct Key {
            U id;
            Value* value;

            Key(U id, Value* val = nullptr) : id(id), value(val) {}
        };

        std::vector<Key*> key_list;
    };
}

#endif // MAP_H