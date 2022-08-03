#ifndef VECTOR_H
#define VECTOR_H

#include <utility>
#include <stdexcept>

namespace custom {
    template<typename T>
    class Vector {
    public:
        explicit Vector(size_t capacity = 10) : capacity(capacity), mSize(0) {
            data = (T*)::operator new(capacity * sizeof(T));
        }

        void push_back(const T& value) {
            if (mSize >= capacity)
                grow();
            data[mSize++] = value;
        }

        void push_back(T&& value) {
            if (mSize >= capacity)
                grow();
            data[mSize++] = std::move(value);
        }

        template<typename... Ts>
        T& emplace_back(Ts&& ... args) {
            if (mSize >= capacity)
                grow();
            new(&data[mSize]) T(std::forward<Ts>(args)...);
            return data[mSize++];
        }

        void pop_back() {
            if (mSize) {
                data[--mSize].~T();
                if (mSize < (capacity / 2))
                    shrink();
            } else throw std::runtime_error("Vector is empty, there is nothing to pop.");
        }

        T& front() {
            if (mSize)
                return data[0];
            throw std::runtime_error("Vector is empty, there is nothing at the front.");
        }

        const T& front() const {
            if (mSize)
                return data[0];
            throw std::runtime_error("Vector is empty, there is nothing at the front.");
        }

        T& back() {
            if (mSize)
                return data[mSize - 1];
            throw std::runtime_error("Vector is empty, there is nothing at the back");
        }

        const T& back() const {
            if (mSize)
                return data[mSize - 1];
            throw std::runtime_error("Vector is empty, there is nothing at the back");
        }

        size_t size() const noexcept {
            return mSize;
        }

        bool empty() const noexcept {
            return (bool)mSize;
        }

        explicit operator bool() const noexcept {
            return mSize != 0;
        }

        T& operator[](const size_t& index) {
            if (index >= 0 && index < mSize)
                return data[index];
            throw std::invalid_argument("Invalid index, out of range");
        }

        const T& operator[](const size_t& index) const {
            if (index >= 0 && index < mSize)
                return data[index];
            throw std::invalid_argument("Invalid index, out of range");
        }

        void clear() {
            if (mSize) {
                for (size_t i = 0; i < mSize; ++i)
                    data[i].~T();
                mSize = 0;
            } else throw std::runtime_error("Vector is empty, cannot be cleared");
        }

        ~Vector() {
            clear();
            ::operator delete(data, capacity * sizeof(T));
        }

    private:
        size_t mSize;
        size_t capacity;
        T* data;

        void grow() {
            if (!data) {
                capacity = 10;
                data = (T*)::operator new(
                        capacity * sizeof(T));  // Allocates memory without calling constructor, analogous to malloc
                return;
            }
            size_t new_capacity = capacity + capacity / 2;
            T* new_data = (T*)::operator new(new_capacity * sizeof(T));

            for (size_t i = 0; i < mSize; ++i) {
                new_data[i] = std::move(data[i]);
                data[i].~T();
            }

            ::operator delete(data, capacity * sizeof(T));  // deallocated memory without calling destructor
            data = new_data;
            capacity = new_capacity;
        }

        void shrink() {
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
}

#endif // VECTOR_H