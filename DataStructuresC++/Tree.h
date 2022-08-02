#ifndef TREE_H
#define TREE_H

#include <vector>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

namespace custom {
    template<typename T>
    class Tree {
    public:
        Tree(const T& data, const bool& ordered = false) {
            Node* new_node = new Node(data);
            root = new_node;
            current_head = root;
            this->ordered = ordered;  // Orders children in ascending values
            if (ordered) {
                static_assert(std::is_arithmetic<T>::value, "Ordered trees require arithmetic data objects");
            }
        }

        void add_child(const T& data) {
            if (current_head == nullptr) {
                throw std::runtime_error("Current node is uninitialised, cannot add child.");
                return;
            }
            Node* new_node = new Node(data);
            if (ordered) {
                int index = 0;
                for (Node*& node : current_head->children) {
                    if (data > node->data)
                        ++index;
                    else break;
                }
                current_head->children.insert(current_head->children.begin() + index, new_node);
            } else current_head->children.push_back(new_node);
        }

        std::vector<T> children_data() const {
            if (current_head->children.empty()) {
                throw std::runtime_error("Current node has no children.");
                return {};
            }
            std::vector<T> ret = {};
            for (const Node*& node : current_head->children) {
                ret.push_back(node->data);
            }
            return ret;
        }

        int find_child(const T& data) {
            if (current_head->children.empty()) {
                throw std::runtime_error("Current node has no children.");
                return -1;
            }
            int index = 0;
            for (Node* node : current_head->children) {
                if (node->data == data)
                    return index;
                ++index;
            }
            return -1;
        }

        void goto_child(const int& index) {
            if (current_head->children.size() - 1 < index || index < 0) {
                throw std::invalid_argument("Index out of range.");
                return;
            }
            current_head = current_head->children[index];
        }

        void goto_root() {
            current_head = root;
        }

        int height() {
            return get_depth(current_head);
        }

        int max_height() {
            return get_depth(root);
        }

        operator bool() const {
            return current_head != nullptr;
        }

        std::vector<T> contents_InOrder() {
            if (max_height() == 0) {
                throw std::runtime_error("Error: Tree is empty, there is no content to return");
                return std::vector<T>();
            }
            std::vector<T> temp = {};
            return InOrder(root, temp);
        }

        void remove_child(const int& index) {
            if (current_head->children.size() - 1 < index || index < 0) {
                throw std::invalid_argument("Index out of range.");
                return;
            }
            current_head->children.erase(current_head->children.begin() + index);
        }

        void delete_subtree() {
            delete_tree(current_head);
        }

        void clear() {
            delete_tree(root);
            root = nullptr;
            current_head = root;
        }

        ~Tree() {
            if (root)
                clear();
        }

    private:
        struct Node {
            T data;
            std::vector<Node*> children;

            Node(const T& data = 0) : data(data), children({}) {}
        };

        Node* root;
        Node* current_head;
        bool ordered;

        std::vector<T>& InOrder(Node* node, std::vector<T>& data) {
            // TODO: FIX this function
            if (!node) return data;
            data.push_back(node->data);
            for (Node*& child : node->children) {
                data.push_back(child->data);
                InOrder(child, data);
            }
            return data;
        }

        int get_depth(Node* node) {
            if (!node) return 0;
            int max_depth = 0;
            for (Node* child : node->children) {
                max_depth = std::max(max_depth, get_depth(child));
            }
            return ++max_depth;
        }

        void delete_tree(Node*& node) {
            if (!node) return;
            if (node->children.empty()) {
                delete node;
                return;
            }
            for (Node*& child : node->children) {
                delete_tree(child);
            }
        }
    };
}

#endif // TREE_H