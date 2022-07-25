#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Queue {
public:
    Queue() : head(nullptr), tail(nullptr), priority_val(None), mLength(0) {}

    explicit Queue(const T& data, int priority = None) {
        head = new Node(data);
        tail = head;
        mLength = 1;
        priority_val = priority;
    }

    Queue(Queue<T>& other) {
        head = new Node(other.head->data);
        mLength = other.mLength;
        priority_val = other.priority_val;
        tail = head;
        Node* other_node = other.head->next;
        while (other_node) {
            tail->next = new Node(other_node->data);
            tail = tail->next;
            other_node = other_node->next;
        }
    }

    Queue& operator=(const Queue<T>& other) {
        if (this != &other) {
            if (mLength > 0)
                clear();
            if (other.mLength) {
                head = new Node(other.head->data);
                mLength = other.mLength;
                priority_val = other.priority_val;
                tail = head;
                Node* other_node = other.head->next;
                while (other_node) {
                    tail->next = new Node(other_node->data);
                    tail = tail->next;
                    other_node = other_node->next;
                }
            }
            else {
                head = nullptr;
                tail = head;
                mLength = 0;
            }
        }
        return *this;
    }

    Queue(Queue&& other) noexcept {
        head = other.head;
        tail = other.tail;
        mLength = other.mLength;
        priority_val = other.priority_val;
        other.head = nullptr;
        other.tail = nullptr;
        other.mLength = 0;
        other.priority_val = None;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            if (mLength > 0)
                clear();
            head = other.head;
            tail = other.tail;
            mLength = other.mLength;
            priority_val = other.priority_val;
            other.head = nullptr;
            other.tail = nullptr;
            other.mLength = 0;
            other.priority_val = None;
        }
        return *this;
    }

    void enqueue(const T& data) {
        if (priority_val == None && mLength) {
            Node* new_node = new Node(data);
            tail->next = new_node;
            tail = new_node;
            ++mLength;
            return;
        }
        if (mLength) {
            Node* cur_node = head;
            size_t index = 0;
            switch (priority_val) {
                case Ascending:
                    while (cur_node && cur_node->data <= data) {
                        cur_node = cur_node->next;
                        ++index;
                    }
                    insert(data, index);
                    return;

                case Descending:
                    while (cur_node && cur_node->data >= data) {
                        cur_node = cur_node->next;
                        ++index;
                    }
                    insert(data, index);
                    return;
            }
        }
        Node* new_node = new Node(data);
        head = new_node;
        tail = head;
        mLength = 1;
    }

    T dequeue() {
        if (mLength) {
            Node* first = head;
            head = head->next;
            T data = first->data;
            delete first;
            --mLength;
            return data;
        }
        throw std::runtime_error("Error: queue is empty, there is nothing to dequeue");
    }

    T peek() const {
        if (mLength)
            return head->data;
        throw std::runtime_error("Error: queue is empty, there is nothing to peek");
    }

    int length() const noexcept {
        return mLength;
    }

    bool empty() const noexcept {
        return mLength == 0;
    }

    operator bool() const noexcept {
        return (bool) mLength;
    }

    bool operator==(const Queue<T>& other) const {
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

    T get(const size_t& index) const {
        if (index < mLength) {
            if (index == 0)
                return head->data;
            size_t cur_index = 1;
            Node* cur_node = head;
            while (1) {
                Node* last_node = cur_node;
                cur_node = cur_node->next;
                if (cur_index == index)
                    return cur_node->data;
                ++cur_index;
            }
        }
        throw std::invalid_argument("Invalid index, out of range");
    }

    T operator[](const size_t& index) const {
        if (index < mLength)
            return get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    T& operator[](const size_t& index) {
        if (index < mLength)
            return ref_get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    bool contains(const T& data) const {
        if (mLength) {
            Node* cur_node = head;
            while (cur_node) {
                if (cur_node->data == data)
                    return true;
                cur_node = cur_node->next;
            }
            return false;
        }
        throw std::runtime_error("Error: queue is empty");
    }

    int find(const T& data) const {
        if (mLength) {
            int index = 0;
            Node* cur_node = head;
            while (cur_node) {
                if (cur_node->data == data)
                    return index;
                cur_node = cur_node->next;
                ++index;
            }
            return -1;
        }
        throw std::runtime_error("Error: queue is empty, there is no content to search");
    }

    void insert(const T& data, const size_t& index) {
        if (mLength && index <= mLength) {
            Node* new_node = new Node(data);
            if (index == 0) {
                new_node->next = head;
                if (head == tail)
                    tail = new_node->next;
                head = new_node;
                ++mLength;
                return;
            }
            if (index == mLength) {
                tail->next = new_node;
                tail = new_node;
                ++mLength;
                return;
            }
            size_t _index = 1;
            Node* cur_node = head;
            Node* last_node = nullptr;
            while (1) {
                last_node = cur_node;
                cur_node = cur_node->next;
                if (_index == index) {
                    last_node->next = new_node;
                    new_node->next = cur_node;
                    ++mLength;
                    return;
                }
                ++_index;
            }
        }
        if (index > mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::invalid_argument("Queue is empty and uninitialised, use enqueue instead");
    }

    Queue<T> operator+(Queue<T>& right) {
        if (right.mLength) {
            std::vector<T> data = contents();
            std::vector<T> right_data = right.contents();
            for (T& i: right_data)
                data.push_back(i);
            Queue<T>* res = new Queue<T>();
            for (const T& i: data)
                res->enqueue(i);
            return *res;
        }
        return *this;
    }

    void erase(const size_t& index) {
        if (index < mLength) {
            if (index == 0) {
                Node* head_cpy = head;
                head = head->next;
                delete head_cpy;
                return;
            }
            size_t cur_index = 1;
            Node* cur_node = head;
            while (1) {
                Node* last_node = cur_node;
                cur_node = cur_node->next;
                if (cur_index == index) {
                    last_node->next = cur_node->next;
                    if (last_node->next == nullptr) {
                        tail = last_node;
                    }
                    delete cur_node;
                    return;
                }
                ++cur_index;
            }
        }
        throw std::invalid_argument("Invalid index, out of range");
    }

    std::vector<T> contents() const {
        if (mLength) {
            std::vector<T> elems(mLength);
            Node* cur_node = head;
            for (size_t i = 0; i < mLength; ++i) {
                elems[i] = cur_node->data;
                cur_node = cur_node->next;
            }
            return elems;
        }
        throw std::runtime_error("Error: queue is empty, there is no content");
    }

    void display() const {
        if (mLength) {
            std::vector<T> data = contents();
            for (const T& i: data) {
                std::cout << i << "\t";
            }
            std::cout << "\n";
        }
        else throw std::runtime_error("Error: queue is empty, there is nothing to display");
    }

    void clear() {
        if (mLength) {
            Node* cur_node = head;
            while (cur_node) {
                cur_node = cur_node->next;
                delete head;
                head = cur_node;
            }
            head = nullptr;
        }
        else throw std::runtime_error("Error: queue is empty and so cannot be cleared");
    }

    virtual ~Queue() {
        if (mLength)
            clear();
    }

private:
    struct Node {
        T data;
        Node* next = nullptr;

        explicit Node(T data) : data(data) {}
    };

    Node* head;
    Node* tail;
    int priority_val;
    size_t mLength;
    enum Priority {
        None = 0,
        Ascending,
        Descending
    };

    T& ref_get(const size_t& index) {
        if (index < mLength) {
            if (index == 0)
                return head->data;
            size_t cur_index = 1;
            Node* cur_node = head;
            while (1) {
                Node* last_node = cur_node;
                cur_node = cur_node->next;
                if (cur_index == index)
                    return cur_node->data;
                ++cur_index;
            }
        }
        throw std::invalid_argument("Invalid index, out of range");
    }
};

