#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

namespace BT {

	struct Node {
		int data;
		Node* left = NULL;
		Node* right = NULL;
	};

	class BinaryTree {
	public:
		BinaryTree(const int& data) {
			root = new Node();
			root->data = data;
			current_head = root;
			_height = 1;
		}

		void new_left(const int& data) {
			Node* new_node = new Node();
			new_node->data = data;
			current_head->left = new_node;
			if (current_head->right == NULL) _height++;
		}

		void new_right(const int& data) {
			Node* new_node = new Node();
			new_node->data = data;
			current_head->right = new_node;
			if (current_head->left == NULL) _height++;
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

		Node* get_root() {
			return root;
		}

		int get_data() {
			return current_head->data;
		}

		int get_data(BinaryTree* node) {
			return node->current_head->data;
		}

		int height() {
			return _height;
		}

		int show_left() {
			return current_head->left->data;
		}

		int show_right() {
			return current_head->right->data;
		}

		std::vector<int>& PreOrder(Node* node, std::vector<int>& data) {
			if (node != NULL) {
				data.push_back(node->data);
				PreOrder(node->left, data);
				PreOrder(node->right, data);
			}
			return data;
		}

		std::vector<int>& InOrder(Node* node, std::vector<int>& data) {
			if (node != NULL) {
				InOrder(node->left, data);
				data.push_back(node->data);
				InOrder(node->right, data);
			}
			return data;
		}

		std::vector<int>& PostOrder(Node* node, std::vector<int>& data) {
			if (node != NULL) {
				PostOrder(node->left, data);
				PostOrder(node->right, data);
				data.push_back(node->data);
			}
			return data;
		}

		std::vector<int> contents_PreOrder() {
			std::vector<int> temp = {};
			return PreOrder(root, temp);
		}

		std::vector<int> contents_InOrder() {
			std::vector<int> temp = {};
			return InOrder(root, temp);
		}

		std::vector<int> contents_PostOrder() {
			std::vector<int> temp = {};
			return PostOrder(root, temp);
		}

	private:
		Node* root;
		Node* current_head;
		int _height;
	};

}