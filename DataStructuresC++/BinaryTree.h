#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

namespace BT {

    template<typename T>
    struct Node {
        T data;
        Node<T>* left = nullptr;
        Node<T>* right = nullptr;

        Node(T data) : data(data) {}
    };

    template<typename T>
    class BinaryTree {
    public:
        BinaryTree(const T& data) {
            root = new Node<T>(data);
            current_head = root;
        }

        void new_left(const T& data) {
            if (current_head->left == nullptr) {
                Node<T>* new_node = new Node<T>(data);
                current_head->left = new_node;
            }
            else
                throw std::runtime_error(
                        "Left node is already initialised, use change_left function to change left node.");
        }

        void new_right(const T& data) {
            if (current_head->right == nullptr) {
                Node<T>* new_node = new Node<T>(data);
                current_head->right = new_node;
            }
            else
                throw std::runtime_error(
                        "Right node is already initialised, use change_right function to change right node.");
        }

        void change_data(const T& data) {
            if (current_head != nullptr)
                current_head->data = data;
            else if (current_head == root && root == nullptr) {
                Node<T>* new_node = new Node<T>(data);
                root = new_node;
            }
            else throw std::runtime_error("Current node is uninitialised, there is no value to change.");
        }

        void change_left(const T& data) {
            if (current_head->left != nullptr)
                current_head->left->data = data;
            else throw std::runtime_error("Left node is uninitialised, use new_left function to add a left node.");
        }

        void change_right(const T& data) {
            if (current_head->right != nullptr)
                current_head->right->data = data;
            else throw std::runtime_error("Right node is uninitialised, use new_right function to add a right node.");
        }

        void advance_left() {
            if (current_head->left != nullptr)
                current_head = current_head->left;
            else throw std::runtime_error("Left node is uninitialised.");
        }

        void advance_right() {
            if (current_head->right != nullptr)
                current_head = current_head->right;
            else throw std::runtime_error("Right node is uninitialised.");
        }

        void goto_root() {
            current_head = root;
        }

        Node<T>* get_root() const {
            return root;
        }

        T get_data() const {
            return current_head->data;
        }

        static T get_data(BinaryTree<T>* node) {
            if (node->current_head != nullptr)
                return node->current_head->data;
            else {
                throw std::runtime_error("Error: Binary tree object provided is empty");
                return T();
            }
        }

        int max_height() {
            if (root == nullptr)
                return -1;
            return calc_max_height(root);
        }

        int height() {
            return calc_max_height(current_head);
        }

        bool is_empty() {
            return max_height() == 0;
        }

        operator bool() const {
            return (is_empty() != 0);
        }

        T show_left() const {
            if (current_head->left != nullptr)
                return current_head->left->data;
            else {
                throw std::runtime_error("Error: left node is empty");
                return T();
            }
        }

        T show_right() const {
            if (current_head->right != nullptr)
                return current_head->right->data;
            else {
                throw std::runtime_error("Error: right node is empty");
                return T();
            }
        }

        std::vector<T> contents_PreOrder() {
            if (max_height() == 0) {
                throw std::runtime_error("Error: Binary tree is empty, there is no content to return");
                return std::vector<T>();
            }
            std::vector<T> temp = {};
            return PreOrder(root, temp);
        }

        std::vector<T> contents_InOrder() {
            if (max_height() == 0) {
                throw std::runtime_error("Error: Binary tree is empty, there is no content to return");
                return std::vector<T>();
            }
            std::vector<T> temp = {};
            return InOrder(root, temp);
        }

        std::vector<T> contents_PostOrder() {
            if (max_height() == 0) {
                throw std::runtime_error("Error: Binary tree is empty, there is no content to return");
                return std::vector<T>();
            }
            std::vector<T> temp = {};
            return PostOrder(root, temp);
        }

        void remove_left() {
            if (current_head->left != nullptr)
                delete_tree(current_head->left);
            else {
                throw std::runtime_error("Error: Left node is empty, there is nothing to remove");
            }
        }

        void remove_right() {
            if (current_head->right != nullptr)
                delete_tree(current_head->right);
            else {
                throw std::runtime_error("Error: Right node is empty, there is nothing to remove");
            }
        }

        void clear() {
            if (max_height() == -1) {
                throw std::runtime_error("Error: Binary tree is empty, there is nothing to remove");
                return;
            }
            delete_tree(root);
            root = nullptr;
            current_head = root;
        }

        ~BinaryTree() {
            if (root != nullptr)
                clear();
        }

    private:
        Node<T>* root;
        Node<T>* current_head;

        std::vector<T>& PreOrder(Node<T>* node, std::vector<T>& data) {
            if (node != nullptr) {
                data.push_back(node->data);
                PreOrder(node->left, data);
                PreOrder(node->right, data);
            }
            return data;
        }

        std::vector<T>& InOrder(Node<T>* node, std::vector<T>& data) {
            if (node != nullptr) {
                InOrder(node->left, data);
                data.push_back(node->data);
                InOrder(node->right, data);
            }
            return data;
        }

        std::vector<T>& PostOrder(Node<T>* node, std::vector<T>& data) {
            if (node != nullptr) {
                PostOrder(node->left, data);
                PostOrder(node->right, data);
                data.push_back(node->data);
            }
            return data;
        }

        int calc_max_height(Node<T>* node) {
            if (node == nullptr)
                return -1;
            int l_height = calc_max_height(node->left);
            int r_height = calc_max_height(node->right);
            if (l_height > r_height)
                return l_height + 1;
            else return r_height + 1;
        }

        void delete_tree(Node<T>*& node) {
            if (node->left != nullptr)
                delete_tree(node->left);
            if (node->right != nullptr)
                delete_tree(node->right);
            delete node;
        }
    };

}