#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Stack {
public:
    Stack() : head(nullptr), mLength(0) {}

    explicit Stack(const T& data) {
        head = new Node(data);
        mLength = 1;
    }

    Stack(const Stack& other) {
        if (other.mLength) {
            head = new Node(other.head->data);
            mLength = 1;
            Node* other_node = other.head->next;
            while (other_node) {
                push(other_node->data);
                other_node = other_node->next;
            }
            return;
        }
        head = nullptr;
        mLength = 0;
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            if (mLength)
                clear();
            if (other.mLength) {
                head = new Node(other.head->data);
                mLength = 1;
                Node* other_node = other.head->next;
                while (other_node) {
                    push(other_node->data);
                    other_node = other_node->next;
                }
            }
            else {
                head = nullptr;
                mLength = 0;
            }
        }
        return *this;
    }

    Stack(Stack&& other) noexcept {
        head = other.head;
        mLength = other.mLength;
        other.head = nullptr;
        other.mLength = 0;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            if (mLength)
                clear();
            head = other.head;
            mLength = other.mLength;
            other.head = nullptr;
            other.mLength = 0;
        }
        return *this;
    }

    void push(const T& data) {
        Node* new_node = new Node(data);
        if (mLength) {
            new_node->next = head;
            head = new_node;
            ++mLength;
            return;
        }
        head = new_node;
        ++mLength;
    }

    T pop() {
        if (mLength) {
            T result = head->data;
            Node* cur = head;
            head = head->next;
            delete cur;
            --mLength;
            return result;
        }
        throw std::invalid_argument("Stack is empty, there is nothing to pop.");
    }

    T peek() const {
        if (mLength)
            return head->data;
        throw std::invalid_argument("Stack is empty, there is nothing to peek.");
    }

    int length() const noexcept {
        return mLength;
    }

    bool empty() const noexcept {
        return mLength == 0;
    }

    explicit operator bool() const noexcept {
        return (bool) mLength;
    }

    bool operator==(const Stack<T>& other) const {
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
        throw std::runtime_error("Error: stack is empty, there is no content");
    }

    void display() const {
        if (mLength) {
            std::vector<T> data = contents();
            for (const T& i: data) {
                std::cout << i << "\t";
            }
            std::cout << "\n";
        }
        else throw std::runtime_error("Error: stack is empty, there is nothing to display");
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
            mLength = 0;
        }
        else throw std::runtime_error("Error: stack is empty and so cannot be cleared");
    }

    ~Stack() {
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
    size_t mLength;
};

