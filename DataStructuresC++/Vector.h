#pragma once
#include <utility>
#include <stdexcept>

namespace Vector {
	template<typename T>
	class Vector {
	public:
		Vector(size_t capacity = 0) {
			m_size = 0;
			this->capacity = capacity;
			if (!capacity) data = nullptr;
			else data = new T[capacity];
		}

		void push_back(const T& value) {
			if (m_size >= capacity)
				grow();
			data[m_size++] = value;
		}

		void pop_back() {
			if (m_size == 0) {
				throw std::runtime_error("Vector is empty, there is nothing to pop.");
				return;
			}
			data[m_size--].~T();
		}

		T front() const {
			if (data[0])
				return data[0];
			throw std::runtime_error("Vector is empty, there is nothing at the front.");
			return T();
		}

		size_t size() const {
			return m_size;
		}

		T operator[](const size_t& index) const {
			if (index < 0 || index > m_size) {
				throw std::invalid_argument("Invalid index, out of range");
				return T();
			}
			return data[index];
		}

		void clear() {
			if (!data) {
				throw std::runtime_error("Vector is empty, cannot be cleared");
				return;
			}
			delete[] data;
			data = nullptr;
		}

		~Vector() {
			if (data)
				clear();
		}

	private:
		size_t m_size;
		size_t capacity;
		T* data;

		void grow() {
			if (!data) {
				capacity = 2;
				data = new T[capacity];
				return;
			}
			size_t new_capacity = capacity * 1.5;
			T* new_data = new T[new_capacity];

			for (size_t i = 0; i < m_size; ++i)
				new_data[i] = std::move(data[i]);

			delete[] data;
			data = new_data;
			capacity = new_capacity;
		}

		void shrink() {
			size_t new_capacity = capacity / 1.5;
			T* new_data = new T[new_capacity];
			for (size_t i = 0; i < m_size; ++i)
				new_data[i] = std::move(data[i]);

			delete[] data;
			data = new_data;
			capacity = new_capacity;
		}
	};
}