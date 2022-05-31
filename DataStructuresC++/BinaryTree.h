#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

namespace BT {

	template<typename T>
	struct Node {
		T data;
		Node<T>* left = NULL;
		Node<T>* right = NULL;
	};

	template<typename T>
	class BinaryTree {
	public:
		BinaryTree(const T& data) {
			root = new Node<T>();
			root->data = data;
			current_head = root;
		}

		void new_left(const T& data) {
			Node<T>* new_node = new Node<T>();
			new_node->data = data;
			current_head->left = new_node;
		}

		void new_right(const T& data) {
			Node<T>* new_node = new Node<T>();
			new_node->data = data;
			current_head->right = new_node;
		}

		void advance_left() {
			if (current_head->left != NULL)
				current_head = current_head->left;
			else throw std::invalid_argument("Left node is uninitialised.");
		}

		void advance_right() {
			if (current_head->right != NULL)
				current_head = current_head->right;
			else throw std::invalid_argument("Right node is uninitialised.");
		}

		void goto_root() {
			current_head = root;
		}

		Node<T>* get_root() {
			return root;
		}

		T get_data() {
			return current_head->data;
		}

		T get_data(BinaryTree<T>* node) {
			return node->current_head->data;
		}

		int max_height(Node<T>* node) {
			if (node == NULL) {
				return -1;
			}
			
			int l_height = max_height(node->left);
			int r_height = max_height(node->right);

			if (l_height > r_height) {
				return l_height + 1;
			}
			else return r_height + 1;
		}

		int height() {
			return max_height(root);
		}

		T show_left() {
			return current_head->left->data;
		}

		T show_right() {
			return current_head->right->data;
		}

		std::vector<T>& PreOrder(Node<T>* node, std::vector<T>& data) {
			if (node != NULL) {
				data.push_back(node->data);
				PreOrder(node->left, data);
				PreOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& InOrder(Node<T>* node, std::vector<T>& data) {
			if (node != NULL) {
				InOrder(node->left, data);
				data.push_back(node->data);
				InOrder(node->right, data);
			}
			return data;
		}

		std::vector<T>& PostOrder(Node<T>* node, std::vector<T>& data) {
			if (node != NULL) {
				PostOrder(node->left, data);
				PostOrder(node->right, data);
				data.push_back(node->data);
			}
			return data;
		}

		std::vector<T> contents_PreOrder() {
			std::vector<T> temp = {};
			return PreOrder(root, temp);
		}

		std::vector<T> contents_InOrder() {
			std::vector<T> temp = {};
			return InOrder(root, temp);
		}

		std::vector<T> contents_PostOrder() {
			std::vector<T> temp = {};
			return PostOrder(root, temp);
		}

	private:
		Node<T>* root;
		Node<T>* current_head;
	};

}