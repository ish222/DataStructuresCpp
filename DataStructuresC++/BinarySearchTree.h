#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <type_traits>

namespace custom {
	template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
	class BinarySearchTree {
	public:
		explicit BinarySearchTree(const T& data) {
			root = new Node(data);
			current_head = root;
			left = false;
		}

		explicit BinarySearchTree(T&& data) {
			root = new Node(std::move(data));
			current_head = root;
			left = false;
		}

		void add(const T& data) {
			Node* change = find_node(data, root);
			if (change == nullptr && current_head != nullptr) {
				change = new Node(data);
				if (left)
					current_head->left = change;
				else current_head->right = change;
				left = false;
			} else throw std::invalid_argument("This value already exists in the tree");
		}

		int height() {
			if (root == nullptr)
				return -1;
			return calc_max_height(root);
		}

		bool is_empty() {
			return height() == 0;
		}

		operator bool() const {
			return (is_empty() != 0);
		}

		std::vector<T> contents_PreOrder() {
			if (height() == 0) {
				throw std::runtime_error("Error: Binary search tree is empty, there is no content to return");
				return std::vector<T>();
			}
			std::vector<T> temp = {};
			return PreOrder(root, temp);
		}

		std::vector<T> contents_InOrder() {
			if (height() == 0) {
				throw std::runtime_error("Error: Binary search tree is empty, there is no content to return");
				return std::vector<T>();
			}
			std::vector<T> temp = {};
			return InOrder(root, temp);
		}

		std::vector<T> contents_PostOrder() {
			if (height() == 0) {
				throw std::runtime_error("Error: Binary search tree is empty, there is no content to return");
				return std::vector<T>();
			}
			std::vector<T> temp = {};
			return PostOrder(root, temp);
		}

		void remove(const T& val) {
			Node* node = find_node(val, root);
			if (node == nullptr) {
				throw std::runtime_error("Error: value not found, so cannot be deleted");
				return;
			} else if (node->left == nullptr && node->right == nullptr) {
				delete node;
				if (left)
					current_head->left = nullptr;
				else current_head->right = nullptr;
				return;
			} else if (node->left == nullptr) {
				Node* node_cpy = node;
				node = node->right;
				delete node_cpy;
				if (left)
					current_head->left = node;
				else current_head->right = node;
				return;
			} else if (node->right == nullptr) {
				Node* node_cpy = node;
				node = node->left;
				delete node;
				if (left)
					current_head->left = node;
				else current_head->right = node;
				return;
			} else {
				Node* replace = min_value(node->right);
				node->data = replace->data;
				delete replace;
				if (left)
					current_head->left = nullptr;
				else current_head->right = nullptr;
			}

		}

		void clear() {
			if (height() == -1) {
				throw std::runtime_error("Error: Binary search tree is empty, there is nothing to remove");
				return;
			}
			delete_tree(root);
			root = nullptr;
		}

		~BinarySearchTree() {
			if (root != nullptr)
				clear();
		}

	private:
		struct Node {
			T data;
			Node* left = nullptr;
			Node* right = nullptr;

			explicit Node(const T& data) : data(data) {}

			explicit Node(T&& data) : data(std::move(data)) {}
		};

		Node* root;
		Node* current_head;
		bool left;

		Node* find_node(const T& data, Node* node) {
			if (node == nullptr) {
				return node;
			}
			if (node->data > data) {
				current_head = node;
				left = true;
				return find_node(data, node->left);
			} else if (node->data < data) {
				current_head = node;
				left = false;
				return find_node(data, node->right);
			} else return node;
		}

		Node* min_value(Node* node) {
			Node* cur = node;
			while (cur->left != nullptr) {
				current_head = cur;
				cur = cur->left;
			}
			return cur;
		}

		std::vector<T>& PreOrder(Node* node, std::vector<T>& data) {
			if (node != nullptr) {
				data.push_back(node->data);
				PreOrder(node->left, data);
				PreOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& InOrder(Node* node, std::vector<T>& data) {
			if (node != nullptr) {
				InOrder(node->left, data);
				data.push_back(node->data);
				InOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& PostOrder(Node* node, std::vector<T>& data) {
			if (node != nullptr) {
				PostOrder(node->left, data);
				PostOrder(node->right, data);
				data.push_back(node->data);
			}
			return data;
		}

		int calc_max_height(Node* node) {
			if (node == nullptr)
				return -1;
			int l_height = calc_max_height(node->left);
			int r_height = calc_max_height(node->right);
			if (l_height > r_height)
				return l_height + 1;
			else return r_height + 1;
		}

		void delete_tree(Node*& node) {
			if (node->left != nullptr)
				delete_tree(node->left);
			if (node->right != nullptr)
				delete_tree(node->right);
			delete node;
		}
	};
};

#endif //BINARY_SEARCH_TREE_H