#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	template<typename T>
	class BinaryTree {
	public:
		BinaryTree() noexcept : root(nullptr), current_head(nullptr) {}

		explicit BinaryTree(const T& data) noexcept {
			root = new Node(data);
			current_head = root;
		}

		explicit BinaryTree(T&& data) noexcept {
			root = new Node(std::move(data));
			current_head = root;
		}

		void new_left(const T& data) {
			if (current_head->left == nullptr) {
				Node* new_node = new Node(data);
				current_head->left = new_node;
			} else
				throw std::runtime_error(
				        "Left node is already initialised, use change_left function to change left node.");
		}

		void new_left(T&& data) {
			if (current_head->left == nullptr) {
				Node* new_node = new Node(std::move(data));
				current_head->left = new_node;
			} else
				throw std::runtime_error(
				        "Left node is already initialised, use change_left function to change left node.");
		}

		void new_right(const T& data) {
			if (current_head->right == nullptr) {
				Node* new_node = new Node(data);
				current_head->right = new_node;
			} else
				throw std::runtime_error(
				        "Right node is already initialised, use change_right function to change right node.");
		}

		void new_right(T&& data) {
			if (current_head->right == nullptr) {
				Node* new_node = new Node(std::move(data));
				current_head->right = new_node;
			} else
				throw std::runtime_error(
				        "Right node is already initialised, use change_right function to change right node.");
		}

		void change_data(const T& data) {
			if (current_head)
				current_head->data = data;
			else if (current_head == root && root == nullptr) {
				Node* new_node = new Node(data);
				root = new_node;
			} else
				throw std::runtime_error("Current node is uninitialised, there is no value to change.");
		}

		void change_data(T&& data) {
			if (current_head)
				current_head->data = std::move(data);
			else if (current_head == root && root == nullptr) {
				Node* new_node = new Node(std::move(data));
				root = new_node;
			} else
				throw std::runtime_error("Current node is uninitialised, there is no value to change.");
		}

		void change_left(const T& data) {
			if (current_head->left)
				current_head->left->data = data;
			else
				throw std::runtime_error("Left node is uninitialised, use new_left function to add a left node.");
		}

		void change_left(T&& data) {
			if (current_head->left)
				current_head->left->data = std::move(data);
			else
				throw std::runtime_error("Left node is uninitialised, use new_left function to add a left node.");
		}

		void change_right(const T& data) {
			if (current_head->right)
				current_head->right->data = data;
			else
				throw std::runtime_error("Right node is uninitialised, use new_right function to add a right node.");
		}

		void change_right(T&& data) {
			if (current_head->right)
				current_head->right->data = std::move(data);
			else
				throw std::runtime_error("Right node is uninitialised, use new_right function to add a right node.");
		}

		void advance_left() {
			if (current_head->left)
				current_head = current_head->left;
			else
				throw std::runtime_error("Left node is uninitialised.");
		}

		void advance_right() {
			if (current_head->right)
				current_head = current_head->right;
			else
				throw std::runtime_error("Right node is uninitialised.");
		}

		void goto_root() noexcept {
			current_head = root;
		}

		const T& get_data() const noexcept {
			return current_head->data;
		}

		[[nodiscard]] static T get_data(BinaryTree<T>* node) {
			if (node->current_head)
				return node->current_head->data;
			throw std::runtime_error("Error: Binary tree object provided is empty");
		}

		[[nodiscard]] int max_height() const noexcept {
			if (root == nullptr)
				return -1;
			return calc_max_height(root);
		}

		[[nodiscard]] int height() const noexcept {
			return calc_max_height(current_head);
		}

		[[nodiscard]] bool empty() const noexcept {
			return max_height() == 0;
		}

		explicit operator bool() const noexcept {
			return (empty() != 0);
		}

		const T& show_left() const {
			if (current_head->left)
				return current_head->left->data;
			throw std::runtime_error("Error: left node is empty");
		}

		const T& show_right() const {
			if (current_head->right)
				return current_head->right->data;
			throw std::runtime_error("Error: right node is empty");
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

		void remove_left() {
			if (current_head->left)
				delete_tree(current_head->left);
			else
				throw std::runtime_error("Error: Left node is uninitialised, there is nothing to remove");
		}

		void remove_right() {
			if (current_head->right)
				delete_tree(current_head->right);
			else
				throw std::runtime_error("Error: Right node is uninitialised, there is nothing to remove");
		}

		void clear() noexcept {
			delete_tree(root);
			root = nullptr;
			current_head = root;
		}

		~BinaryTree() {
			if (root)
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

#endif// BINARY_TREE_H