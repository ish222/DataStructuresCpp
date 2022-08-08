#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace custom {
	template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
	class BinarySearchTree {
	public:
		BinarySearchTree() noexcept : root(nullptr), current_head(nullptr), left(false) {}

		explicit BinarySearchTree(const T& data) noexcept : left(false) {
			root = new Node(data);
			current_head = root;
		}

		explicit BinarySearchTree(T&& data) noexcept : left(false) {
			root = new Node(std::move(data));
			current_head = root;
		}

		BinarySearchTree(std::initializer_list<T> init) noexcept : left(false) {
			for (auto it = init.begin(); it != init.end(); ++it)
				add(std::move(*it));
		}

		void add(const T& data) noexcept {
			Node* change = find_node(data, root);
			if (change == nullptr && current_head != nullptr) {
				change = new Node(data);
				if (left)
					current_head->left = change;
				else
					current_head->right = change;
				left = false;
			} else
				throw std::invalid_argument("This value already exists in the tree");
		}

		void add(T&& data) {
			Node* change = find_node(data, root);
			if (change == nullptr && current_head != nullptr) {
				change = new Node(std::move(data));
				if (left)
					current_head->left = change;
				else
					current_head->right = change;
				left = false;
			} else
				throw std::invalid_argument("This value already exists in the tree");
		}

		[[nodiscard]] int height() const noexcept {
			if (root != nullptr)
				return calc_max_height(root);
			return -1;
		}

		[[nodiscard]] bool is_empty() const noexcept {
			return height() == 0;
		}

		explicit operator bool() const noexcept {
			return (is_empty() != 0);
		}

		[[nodiscard]] std::vector<T> contents_PreOrder() const noexcept {
			std::vector<T> temp = {};
			return PreOrder(root, temp);
		}

		[[nodiscard]] std::vector<T> contents_InOrder() const noexcept {
			std::vector<T> temp = {};
			return InOrder(root, temp);
		}

		[[nodiscard]] std::vector<T> contents_PostOrder() const noexcept {
			std::vector<T> temp = {};
			return PostOrder(root, temp);
		}

		void remove(const T& val) {
			Node* node = find_node(val, root);
			if (node == nullptr) {
				throw std::runtime_error("Error: value not found, so cannot be deleted");
			} else if (node->left == nullptr && node->right == nullptr) {
				delete node;
				if (left)
					current_head->left = nullptr;
				else
					current_head->right = nullptr;
				return;
			} else if (node->left == nullptr) {
				Node* node_cpy = node;
				node = node->right;
				delete node_cpy;
				if (left)
					current_head->left = node;
				else
					current_head->right = node;
				return;
			} else if (node->right == nullptr) {
				Node* node_cpy = node;
				node = node->left;
				delete node_cpy;
				if (left)
					current_head->left = node;
				else
					current_head->right = node;
				return;
			} else {
				Node* replace = min_value(node->right);
				node->data = replace->data;
				delete replace;
				if (left)
					current_head->left = nullptr;
				else
					current_head->right = nullptr;
			}
		}

		void clear() noexcept {
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

			explicit Node(const T& data) noexcept : data(data) {}

			explicit Node(T&& data) noexcept : data(std::move(data)) {}
		};

		Node* root;
		Node* current_head;
		bool left;

		Node* find_node(const T& data, Node* node) noexcept {
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
			} else
				return node;
		}

		Node* min_value(Node* node) noexcept {
			Node* cur = node;
			while (cur->left != nullptr) {
				current_head = cur;
				cur = cur->left;
			}
			return cur;
		}

		std::vector<T>& PreOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				data.push_back(node->data);
				PreOrder(node->left, data);
				PreOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& InOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				InOrder(node->left, data);
				data.push_back(node->data);
				InOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& PostOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				PostOrder(node->left, data);
				PostOrder(node->right, data);
				data.push_back(node->data);
			}
			return data;
		}

		int calc_max_height(Node* node) const noexcept {
			if (node == nullptr)
				return -1;
			int l_height = calc_max_height(node->left);
			int r_height = calc_max_height(node->right);
			if (l_height > r_height)
				return l_height + 1;
			else
				return r_height + 1;
		}

		void delete_tree(Node*& node) noexcept {
			if (node->left != nullptr)
				delete_tree(node->left);
			if (node->right != nullptr)
				delete_tree(node->right);
			delete node;
		}
	};
}// namespace custom

#endif//BINARY_SEARCH_TREE_H