#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), mLength(0) {}

    explicit DoublyLinkedList(const T& data) {
        head = new Node(data);
        tail = head;
        mLength = 1;
    }

    void append(const T& data) {
        Node* new_node = new Node(data);
        ++mLength;
        if (head == nullptr) {
            head = new_node;
            tail = head;
            return;
        }
        tail->next = new_node;
        new_node->last = tail;
        tail = new_node;
    }

private:
    struct Node {
        T data;
        Node* next = nullptr;
        Node* last = nullptr;

        Node(T data) : data(data) {}
    };

    Node* head;
    Node* tail;
    size_t mLength;
};
