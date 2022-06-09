#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <type_traits>

namespace BST {
	template<typename T>
	struct Node {
		T data;
		Node<T>* left = nullptr;
		Node<T>* right = nullptr;

		Node(T data) : data(data) {}
	};

	template<typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
	class BinarySearchTree {
	public:
		BinarySearchTree(const T& data) {
			root = new Node<T>(data);
			current_head = nullptr;
			left = false;
		}

		void add(const T& data) {
			Node<T>* change = find_node(data, root);
			if (change == nullptr && current_head != nullptr) {
				change = new Node(data);
				if (left)
					current_head->left = change;
				else current_head->right = change;
				left = false;
			}
			else throw std::invalid_argument("This value already exists in the tree");
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
		Node<T>* root;
		Node<T>* current_head;
		bool left;

		Node<T>* find_node(const T& data, Node<T>* node) {
			if (node == nullptr) {
				return node;
			}
			if (node->data > data) {
				current_head = node;
				left = true;
				return find_node(data, node->left);
			}
			else if (node->data < data) {
				current_head = node;
				left = false;
				return find_node(data, node->right);
			}
			else return node;
		}

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

		void delete_tree(Node<T>* node) {
			if (node->left != nullptr)
				delete_tree(node->left);
			if (node->right != nullptr)
				delete_tree(node->right);
			delete node;
		}
	};
};