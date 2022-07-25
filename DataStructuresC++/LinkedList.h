#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr), tail(nullptr), mLength(0) {}

    explicit LinkedList(const T& data) {
        head = new Node(data);
        tail = head;
        mLength = 1;
    }

    LinkedList(LinkedList<T>& other) {
        if (other.mLength) {
            head = new Node(other.head->data);
            mLength = other.mLength;
            tail = head;
            Node* other_node = other.head->next;
            while (other_node) {
                tail->next = new Node(other_node->data);
                tail = tail->next;
                other_node = other_node->next;
            }
            return;
        }
        head = nullptr;
        tail = head;
        mLength = 0;
    }

    LinkedList& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            if (mLength)
                clear();
            if (other.mLength) {
                head = new Node(other.head->data);
                mLength = other.mLength;
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

    LinkedList(LinkedList&& other) noexcept {
        head = other.head;
        tail = other.tail;
        mLength = other.mLength;
        other.head = nullptr;
        other.tail = nullptr;
        other.mLength = 0;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            if (mLength)
                clear();
            head = other.head;
            tail = other.tail;
            mLength = other.mLength;
            other.head = nullptr;
            other.tail = nullptr;
            other.mLength = 0;
        }
        return *this;
    }

    void append(const T& data) {
        Node* new_node = new Node(data);
        if (mLength) {
            ++mLength;
            tail->next = new_node;
            tail = new_node;
            return;
        }
        ++mLength;
        head = new_node;
        tail = head;
    }

    void push_back(const T& data) {
        append(data);
    }

    void insert(const T& data, const size_t& index) {
        if (mLength && index <= mLength) {
            Node* new_node = new Node(data);
            ++mLength;
            if (index == 0) {
                new_node->next = head;
                head = new_node;
                return;
            }
            if (index == mLength) {
                tail->next = new_node;
                tail = new_node;
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
                    return;
                }
                ++_index;
            }
        }
        if (index > mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::invalid_argument("Linked list is empty and uninitialised, use append instead");
    }

    void push_front(const T& data) {
        insert(data, 0);
    }

    std::vector<T> contents() const {
        if (mLength) {
            std::vector<T> elems(mLength);
            Node* cur_node = head;
            for (int i = 0; i < mLength; ++i) {
                elems[i] = cur_node->data;
                cur_node = cur_node->next;
            }
            return elems;
        }
        throw std::runtime_error("Error: Linked list is empty");
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
        throw std::runtime_error("Error: Linked list is empty, there is no content to search");
    }

    void display() const {
        if (mLength) {
            std::vector<T> vals = contents();
            for (const T& i: vals)
                std::cout << i << "\t";
            std::cout << "\n";
        }
        else throw std::runtime_error("Error: Linked list is empty");
    }

    size_t length() const noexcept {
        return mLength;
    }

    bool empty() const noexcept {
        return mLength == 0;
    }

    operator bool() const noexcept {
        return (bool) mLength;
    }

    bool operator==(const LinkedList<T>& other) const {
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

    void erase(const size_t& index) {
        if (mLength && index < mLength) {
            if (index == 0) {
                Node* head_cpy = head;
                head = head->next;
                delete head_cpy;
                --mLength;
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
                    --mLength;
                    return;
                }
                ++cur_index;
            }
        }
        if (index >= mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::runtime_error("Error: Linked list is empty, there is nothing to erase");
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
            tail = head;
            mLength = 0;
        }
        else throw std::runtime_error("Error: linked list is empty and so cannot be cleared.");
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

    T front() const {
        if (mLength)
            return head->data;
        throw std::runtime_error("List is empty, there is nothing at front");
    }

    T back() const {
        if (mLength)
            return tail->data;
        throw std::runtime_error("List is empty, there is nothing at back");
    }

    void pop_front() {
        if (mLength)
            erase(0);
        else throw std::runtime_error("List is empty, there is nothing to pop front");
    }

    void pop_back() {
        if (mLength)
            erase(mLength - 1);
        else throw std::runtime_error("List is empty, there is nothing to pop back");
    }

    void reverse_order() {
        if (mLength) {
            Node* cur_node = head;
            tail = head;
            Node* last = nullptr;
            Node* next = nullptr;
            while (cur_node) {
                next = cur_node->next;
                cur_node->next = last;
                last = cur_node;
                cur_node = next;
            }
            head = last;
        }
        else throw std::runtime_error("Error: linked list is empty and so cannot be reversed");
    }

    T operator[](const size_t& index) const {
        if (index < mLength)
            return get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    T& operator[](const size_t index) {
        if (index < mLength)
            return ref_get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    LinkedList<T> operator+(LinkedList<T>& right) {
        if (right.mLength) {
            std::vector<T> data = contents();
            std::vector<T> right_data = right.contents();
            for (T& i: right_data)
                data.push_back(i);
            LinkedList<T>* res = new LinkedList<T>();
            for (const T& i: data)
                res->append(i);
            return *res;
        }
        return *this;
    }

    virtual ~LinkedList() {
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
    size_t mLength;

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
