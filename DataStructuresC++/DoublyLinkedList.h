#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename LinkedList>
class DoublyListIterator {
public:
    using ListType = typename LinkedList::Node;
    using ValueType = typename LinkedList::ValueType;

public:
    DoublyListIterator() : mPtr(nullptr) {}

    DoublyListIterator(ListType* ptr) : mPtr(ptr) {}

    DoublyListIterator(const DoublyListIterator& other) : mPtr(other.mPtr) {}

    DoublyListIterator& operator=(const DoublyListIterator& other) {
        if (this != &other)
            mPtr = other.mPtr;
        return *this;
    }

    DoublyListIterator(DoublyListIterator&& other) noexcept {
        mPtr = other.mPtr;
        other.mPtr = nullptr;
    }

    DoublyListIterator& operator=(DoublyListIterator&& other) noexcept {
        if (this != &other) {
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }

    DoublyListIterator& operator++() {
#ifdef DEBUG
        if (mPtr) {
#endif
            mPtr = mPtr->next;
            return *this;
#ifdef DEBUG
        }
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator operator++(int) {
#ifdef DEBUG
        if (mPtr) {
#endif
            const DoublyListIterator temp(*this);
            ++*this;
            return temp;
#ifdef DEBUG
        }
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator& operator--() {
#ifdef DEBUG
        if (mPtr) {
#endif
            mPtr = mPtr->last;
            return *this;
#ifdef DEBUG
        }
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator operator--(int) {
#ifdef DEBUG
        if (mPtr) {
#endif
            const DoublyListIterator temp(*this);
            --*this;
            return temp;
#ifdef DEBUG
        }
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator& advance(const size_t& distance) {
#ifdef DEBUG
        if (mPtr) {
#endif
            size_t moved = 0;
            if (distance > 0) {
                while (mPtr && moved < distance) {
                    ++*this;
                    ++moved;
                }
            }
            else {
                while (mPtr && moved > distance) {
                    --*this;
                    --moved;
                }
            }
#ifdef DEBUG
            if (moved != distance)
                throw std::invalid_argument("Distance out of range of iterator");
#endif
            return *this;
        }
#ifdef DEBUG
        throw std::runtime_error("Iterator is at an invalid position, cannot advance");
#endif
    }

    DoublyListIterator next() const {
#ifdef DEBUG
        if (mPtr)
#endif
            return ++DoublyListIterator(*this);
#ifdef DEBUG
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator prev() const {
#ifdef DEBUG
        if (mPtr)
#endif
            return --DoublyListIterator(*this);
#ifdef DEBUG
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
    }

    DoublyListIterator operator+(const size_t& amount) {
        DoublyListIterator result(*this);
#ifdef DEBUG
        size_t moved = 0;
        while (mPtr && moved < amount) {
            result.mPtr = result.mPtr->next;
            ++moved;
        }
        if (moved == amount)
            return result;
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
#ifdef RELEASE
        for (size_t i = 0; i < amount; ++i)
            result.mPtr = result.mPtr->next;
        return result;
#endif
    }

    DoublyListIterator& operator+=(const size_t& amount) {
#ifdef DEBUG
        size_t moved = 0;
        while (mPtr && moved < amount) {
            mPtr = mPtr->next;
            ++moved;
        }
        if (moved == amount)
            return *this;
        throw std::out_of_range("Cannot increment list iterator past end of list");
#endif
#ifdef RELEASE
        for (size_t i = 0; i < amount; ++i)
            mPtr = mPtr->next;
        return *this;
#endif
    }

    bool operator==(const DoublyListIterator& other) const noexcept {
        return mPtr == other.mPtr;
    }

    bool operator!=(const DoublyListIterator& other) const noexcept {
        return mPtr != other.mPtr;
    }

    ValueType& operator*() const {
#ifdef DEBUG
        if (mPtr)
#endif
            return mPtr->data;
#ifdef DEBUG
        throw std::runtime_error("Iterator does not point to a valid position, cannot dereference");
#endif
    }

    size_t _size() const noexcept {
        return LinkedList::mLength;
    }

private:
    ListType* mPtr;
};

template<typename T>
class DoublyLinkedList {
public:
    using ValueType = T;
    using Iterator = DoublyListIterator<DoublyLinkedList>;

    friend class DoublyListIterator<DoublyLinkedList>;


public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), mLength(0) {}

    explicit DoublyLinkedList(const T& data) : mLength(1) {
        head = new Node(data);
        tail = head;
    }

    explicit DoublyLinkedList(T&& data) : mLength(1) {
        head = new Node(std::move(data));
        tail = head;
    }

    DoublyLinkedList(const DoublyLinkedList& other) {
        if (other.mLength) {
            head = new Node(other.head->data);
            mLength = other.mLength;
            tail = head;
            Node* other_node = other.head->next;
            while (other_node) {
                Node* new_node = new Node(other_node->data);
                tail->next = new_node;
                new_node->last = tail;
                tail = tail->next;
                other_node = other_node->next;
            }
            return;
        }
        head = nullptr;
        tail = head;
        mLength = 0;
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
//            if (mLength)
//                clear();
            if (other.mLength) {
                head = new Node(other.head->data);
                mLength = other.mLength;
                tail = head;
                Node* other_node = other.head->next;
                while (other_node) {
                    Node* new_node = new Node(other_node->data);
                    tail->next = new_node;
                    new_node->last = tail;
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

    DoublyLinkedList(DoublyLinkedList&& other) noexcept {
        head = other.head;
        tail = other.tail;
        mLength = other.mLength;
        other.head = nullptr;
        other.tail = nullptr;
        other.mLength = 0;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
//            if (mLength)
//                clear();
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
            new_node->last = tail;
            tail = new_node;
            return;
        }
        ++mLength;
        head = new_node;
        tail = head;
    }

    void append(T&& data) {
        Node* new_node = new Node(std::move(data));
        if (mLength) {
            ++mLength;
            tail->next = new_node;
            new_node->last = tail;
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

    void push_back(T&& data) {
        append(std::move(data));
    }

    void insert(const T& data, const size_t& index) {
#ifdef DEBUG
        if (mLength && index <= mLength) {
#endif
            Node* new_node = new Node(data);
            ++mLength;
            if (index != 0 && index < mLength) {
                if (index < mLength/2) {
                    size_t _index = 1;
                    Node* cur_node = head;
                    Node* last_node;
                    while (true) {
                        last_node = cur_node;
                        cur_node = cur_node->next;
                        if (_index == index) {
                            last_node->next = new_node;
                            new_node->last = last_node;
                            new_node->next = cur_node;
                            cur_node->last = new_node;
                            return;
                        }
                        ++_index;
                    }
                }
                else {
                    size_t _index = mLength-1;
                    Node* cur_node = tail;
                    Node* next_node;
                    while (true) {
                        next_node = cur_node;
                        cur_node = cur_node->last;
                        if (_index == index) {
                            next_node->last = new_node;
                            new_node->next = next_node;
                            new_node->last = cur_node;
                            cur_node->next = new_node;
                            return;
                        }
                        --_index;
                    }
                }
            }
            if (index == 0) {
                new_node->next = head;
                head->last = new_node;
                head = new_node;
                return;
            }
            if (index == mLength) {
                tail->next = new_node;
                new_node->last = tail;
                tail = new_node;
                return;
            }
#ifdef DEBUG
        }
        if (index > mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::invalid_argument("Linked list is empty and uninitialised, use append instead");
#endif
    }

    void insert(T&& data, const size_t& index) {
#ifdef DEBUG
        if (mLength && index <= mLength) {
#endif
            Node* new_node = new Node(std::move(data));
            ++mLength;
            if (index != 0 && index < mLength) {
                if (index < mLength / 2) {
                    size_t _index = 1;
                    Node* cur_node = head;
                    Node* last_node;
                    while (true) {
                        last_node = cur_node;
                        cur_node = cur_node->next;
                        if (_index == index) {
                            last_node->next = new_node;
                            new_node->last = last_node;
                            new_node->next = cur_node;
                            cur_node->last = new_node;
                            return;
                        }
                        ++_index;
                    }
                }
                else {
                    size_t _index = mLength-1;
                    Node* cur_node = tail;
                    Node* next_node;
                    while (true) {
                        next_node = cur_node;
                        cur_node = cur_node->last;
                        if (_index == index) {
                            next_node->last = new_node;
                            new_node->next = next_node;
                            new_node->last = cur_node;
                            cur_node->next = new_node;
                            return;
                        }
                        --_index;
                    }
                }
            }
            if (index == 0) {
                new_node->next = head;
                head->last = new_node;
                head = new_node;
                return;
            }
            if (index == mLength) {
                tail->next = new_node;
                new_node->last = tail;
                tail = new_node;
                return;
            }
#ifdef DEBUG
        }
        if (index > mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::invalid_argument("Linked list is empty and uninitialised, use append instead");
#endif
    }

    void push_front(const T& data) {
        insert(data, 0);
    }

    void push_front(T&& data) {
        insert(std::move(data), 0);
    }

    std::vector<T> contents() const {
#ifdef DEBUG
        if (mLength) {
#endif
            std::vector<T> elems(mLength);
            Node* cur_node = head;
            for (int i = 0; i < mLength; ++i) {
                elems[i] = cur_node->data;
                cur_node = cur_node->next;
            }
            return elems;
#ifdef DEBUG
        }
        throw std::runtime_error("Error: Linked list is empty");
#endif
    }

    int find(const T& data) const {
#ifdef DEBUG
        if (mLength) {
#endif
            int index = 0;
            Node* cur_node = head;
            while (cur_node) {
                if (cur_node->data == data)
                    return index;
                cur_node = cur_node->next;
                ++index;
            }
            return -1;
#ifdef DEBUG
        }
        throw std::runtime_error("Error: Linked list is empty, there is no content to search");
#endif
    }

    void display() const {
#ifdef DEBUG
        if (mLength) {
#endif
            std::vector<T> vals = contents();
            for (const T& i: vals)
                std::cout << i << "\t";
            std::cout << "\n";
#ifdef DEBUG
        }
        else throw std::runtime_error("Error: Linked list is empty");
#endif
    }

    size_t length() const noexcept {
        return mLength;
    }

    bool empty() const noexcept {
        return mLength == 0;
    }

    explicit operator bool() const noexcept {
        return (bool) mLength;
    }

    bool operator==(const DoublyLinkedList<T>& other) const {
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
#ifdef DEBUG
        if (mLength && index < mLength) {
#endif
            if (index != 0) {
                if (index < mLength / 2) {
                    size_t cur_index = 1;
                    Node* cur_node = head;
                    while (true) {
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
                else {
                    size_t cur_index = mLength - 1;
                    Node* cur_node = tail;
                    while (true) {
                        Node* next_node = cur_node;
                        cur_node = cur_node->last;
                        if (cur_index == index) {
                            next_node->last = cur_node->last;
                            if (next_node->last == nullptr) {
                                head = next_node;
                            }
                            delete cur_node;
                            --mLength;
                            return;
                        }
                        --cur_index;
                    }
                }
            }
            else {
                Node* head_cpy = head;
                head = head->next;
                delete head_cpy;
                --mLength;
                return;
            }
#ifdef DEBUG
        }
        if (index >= mLength)
            throw std::invalid_argument("Invalid index, out of range");
        throw std::runtime_error("Error: Linked list is empty, there is nothing to erase");
#endif
    }

    void clear() {
#ifdef DEBUG
        if (mLength) {
#endif
            Node* cur_node = head;
            while (cur_node) {
                cur_node = cur_node->next;
                delete head;
                head = cur_node;
            }
            head = nullptr;
            tail = head;
            mLength = 0;
#ifdef DEBUG
        }
        else throw std::runtime_error("Error: linked list is empty and so cannot be cleared.");
#endif
    }

    T get(const size_t& index) const {
#ifdef DEBUG
        if (index < mLength) {
#endif
            if (index == 0)
                return head->data;
            if (index == mLength - 1)
                return tail->data;
            if (index < mLength / 2) {
                size_t cur_index = 1;
                Node* cur_node = head;
                while (true) {
                    cur_node = cur_node->next;
                    if (cur_index == index)
                        return cur_node->data;
                    ++cur_index;
                }
            }
            else {
                size_t cur_index = mLength - 1;
                Node* cur_node = tail;
                while (true) {
                    cur_node = cur_node->last;
                    if (cur_index == index)
                        return cur_node->data;
                    --cur_index;
                }
            }
#ifdef DEBUG
        }
        throw std::invalid_argument("Invalid index, out of range");
#endif
    }

    T front() const {
#ifdef DEBUG
        if (mLength)
#endif
            return head->data;
        throw std::runtime_error("List is empty, there is nothing at front");
    }

    T back() const {
#ifdef DEBUG
        if (mLength)
#endif
            return tail->data;
        throw std::runtime_error("List is empty, there is nothing at back");
    }

    void pop_front() {
#ifdef DEBUG
        if (mLength) {
#endif
            Node* temp = head;
            head = head->next;
            if (head)
                head->last = nullptr;
            delete temp;
            --mLength;
#ifdef DEBUG
        }
        else throw std::runtime_error("List is empty, there is nothing to pop front");
#endif
    }

    void pop_back() {
#ifdef DEBUG
        if (mLength) {
#endif
            Node* temp = tail;
            tail = tail->last;
            if (tail)
                tail->next = nullptr;
            delete temp;
            --mLength;
#ifdef DEBUG
        }
        else throw std::runtime_error("List is empty, there is nothing to pop back");
#endif
    }

    void reverse_order() {
#ifdef DEBUG
        if (mLength) {
#endif
            Node* cur_node = head->next;
            Node* last = head;
            tail = head;
            tail->last = cur_node;
            Node* next;
            while (cur_node) {
                next = cur_node->next;
                cur_node->next = last;
                last->last = cur_node;
                last = cur_node;
                cur_node = next;
            }
            head = last;
#ifdef DEBUG
        }
        else throw std::runtime_error("Error: linked list is empty and so cannot be reversed");
#endif
    }

    T operator[](const size_t& index) const {
#ifdef DEBUG
        if (index < mLength)
#endif
            return get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    T& operator[](const size_t index) {
#ifdef DEBUG
        if (index < mLength)
#endif
            return ref_get(index);
        throw std::invalid_argument("Invalid index, out of range");
    }

    DoublyLinkedList<T> operator+(DoublyLinkedList<T>& right) {
        if (right.mLength) {
            std::vector<T> data = contents();
            std::vector<T> right_data = right.contents();
            for (T& i: right_data)
                data.push_back(i);
            DoublyLinkedList<T>* res = new DoublyLinkedList<T>();
            for (const T& i: data)
                res->append(i);
            return *res;
        }
        return *this;
    }

    virtual ~DoublyLinkedList() {
        if (mLength)
            clear();
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

    T& ref_get(const size_t& index)  {
#ifdef DEBUG
        if (index < mLength) {
#endif
            if (index == 0)
                return head->data;
            if (index == mLength - 1)
                return tail->data;
            if (index < mLength / 2) {
                size_t cur_index = 1;
                Node* cur_node = head;
                while (true) {
                    cur_node = cur_node->next;
                    if (cur_index == index)
                        return cur_node->data;
                    ++cur_index;
                }
            }
            else {
                size_t cur_index = mLength - 1;
                Node* cur_node = tail;
                while (true) {
                    cur_node = cur_node->last;
                    if (cur_index == index)
                        return cur_node->data;
                    --cur_index;
                }
            }
#ifdef DEBUG
        }
        throw std::invalid_argument("Invalid index, out of range");
#endif
    }
};