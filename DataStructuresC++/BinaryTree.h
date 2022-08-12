#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace custom {
	template<typename T>
	/**
	 * A template implementation of a specialised tree data structure where each node can have at most two children
	 * nodes. Each node has a member data of type `T` and pointers to the left and right children nodes.
	 *
	 * @tparam T - the type of the data of each node in the tree.
	 * @see <a href="https://en.wikipedia.org/wiki/Binary_tree">Binary tree</a>
	 */
	class BinaryTree {
	public:
		/**
		 * Default BinaryTree constructor which sets the root and current head members to `nullptr`.
		 */
		BinaryTree() noexcept: root(nullptr), current_head(nullptr) {}

		/**
		 * Overloaded BinaryTree constructor which takes a value of type `T` and constructs a new node with the data
		 * provided, setting it to the root and current head of the tree.
		 * @param data - data of type `T` to be copied into the root node.
		 */
		explicit BinaryTree(const T& data) noexcept {
			root = new Node(data);
			current_head = root;
		}

		/**
		 * Overloaded BinaryTree constructor which takes a value of type `T` and constructs a new node with the data
		 * provided, setting it to the root and current head of the tree.
		 * @param data -  - a *r-value reference* to data of type `T` to be moved into the root node.
		 */
		explicit BinaryTree(T&& data) noexcept {
			root = new Node(std::move(data));
			current_head = root;
		}

		/**
		 * Constructs a new node with the data provided and sets the current head node's left child to the new node.
		 *
		 * If the current head node is uninitialized or its left child is already initialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - data of type `T` to be copied into the new node.
		 */
		void new_left(const T& data) {
			if (current_head && current_head->left == nullptr) {
				Node* new_node = new Node(data);
				current_head->left = new_node;
			} else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot add left node.");
			} else
				throw std::runtime_error(
						"Left node is already initialised, use change_left function to change left node.");
		}

		/**
		 * Constructs a new node with the data provided and sets the current head node's left child to the new node.
		 *
		 * If the current head node is uninitialized or its left child is already initialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the new node.
		 */
		void new_left(T&& data) {
			if (current_head && current_head->left == nullptr) {
				Node* new_node = new Node(std::move(data));
				current_head->left = new_node;
			} else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot add left node.");
			} else
				throw std::runtime_error(
						"Left node is already initialised, use change_left function to change left node.");
		}

		/**
		 * Constructs a new node with the data provided and sets the current head node's right child to the new node.
		 *
		 * If the current head node is uninitialized or its right child is already initialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - data of type `T` to be copied into the new node.
		 */
		void new_right(const T& data) {
			if (current_head && current_head->right == nullptr) {
				Node* new_node = new Node(data);
				current_head->right = new_node;
			} else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot add right node.");
			} else
				throw std::runtime_error(
						"Right node is already initialised, use change_right function to change right node.");
		}

		/**
		 * Constructs a new node with the data provided and sets the current head node's right child to the new node.
		 *
		 * If the current head node is uninitialised or its right child is already initialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the new node.
		 */
		void new_right(T&& data) {
			if (current_head && current_head->right == nullptr) {
				Node* new_node = new Node(std::move(data));
				current_head->right = new_node;
			} else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot add right node.");
			} else
				throw std::runtime_error(
						"Right node is already initialised, use change_right function to change right node.");
		}

		/**
		 * Changes the data member of the current head node.
		 *
		 * If the current head node is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - data of type `T` to be copied into the current head node.
		 */
		void change_data(const T& data) {
			if (current_head)
				current_head->data = data;
			else if (current_head == root && root == nullptr) {
				Node* new_node = new Node(data);
				root = new_node;
			} else
				throw std::runtime_error("Current node is uninitialised, there is no value to change.");
		}

		/**
		 * Changes the data member of the current head node.
		 *
		 * If the current head node is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the current head node.
		 */
		void change_data(T&& data) {
			if (current_head)
				current_head->data = std::move(data);
			else if (current_head == root && root == nullptr) {
				Node* new_node = new Node(std::move(data));
				root = new_node;
			} else
				throw std::runtime_error("Current node is uninitialised, there is no value to change.");
		}

		/**
		 * Changes the data member of the left child node of the current head node.
		 *
		 * If the current head node is uninitialized or its left child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - data of type `T` to be copied into the current head node's left child node.
		 */
		void change_left(const T& data) {
			if (current_head && current_head->left)
				current_head->left->data = data;
			else
				throw std::runtime_error("Left node is uninitialised, use new_left function to add a left node.");
		}

		/**
		 * Changes the data member of the left child node of the current head node.
		 *
		 * If the current head node is uninitialized or its left child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the current head node's left child node.
		 */
		void change_left(T&& data) {
			if (current_head && current_head->left)
				current_head->left->data = std::move(data);
			else
				throw std::runtime_error("Left node is uninitialised, use new_left function to add a left node.");
		}

		/**
		 * Changes the data member of the right child node of the current head node.
		 *
		 * If the current head node is uninitialized or its right child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - data of type `T` to be copied into the current head node's right child node.
		 */
		void change_right(const T& data) {
			if (current_head && current_head->right)
				current_head->right->data = data;
			else
				throw std::runtime_error("Right node is uninitialised, use new_right function to add a right node.");
		}

		/**
		 * Changes the data member of the right child node of the current head node.
		 *
		 * If the current head node is uninitialized or its right child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the current head node's right child node.
		 */
		void change_right(T&& data) {
			if (current_head && current_head->right)
				current_head->right->data = std::move(data);
			else
				throw std::runtime_error("Right node is uninitialised, use new_right function to add a right node.");
		}

		/**
		 * Advances the current head node to its left child node.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * If the current head node or its left child node is uninitialized, a `runtime_error` exception is thrown.
		 */
		void advance_left() {
			if (current_head && current_head->left)
				current_head = current_head->left;
			else
				throw std::runtime_error("Left node is uninitialised.");
		}

		/**
		 * Advances the current head node to its right child node.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * If the current head node or its right child node is uninitialized, a `runtime_error` exception is thrown.
		 */
		void advance_right() {
			if (current_head && current_head->right)
				current_head = current_head->right;
			else
				throw std::runtime_error("Right node is uninitialised.");
		}

		/**
		 * Sets the current head node of the tree to the root node.
		 *
		 * **Time Complexity** = *O(1)*.
		 */
		void goto_root() noexcept {
			current_head = root;
		}

		/**
		 * Returns the data, of type `T`, of the current head node.
		 *
		 * If the current head node is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a const reference to the value of the data member of the current head node.
		 */
		const T& get_data() const {
			if (current_head)
				return current_head->data;
			throw std::runtime_error("Current head node is uninitialised, no data to return.");
		}

		/**
		 * Obtains the value of the maximum height of the tree, from the root to its furthest leaf node. If the tree
		 * is uninitialized, **-1** is returned.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - an integer specifying the maximum height of the tree, **-1** if the tree is uninitialized.
		 */
		[[nodiscard]] int max_height() const noexcept {
			if (root == nullptr)
				return -1;
			return calc_max_height(root);
		}

		/**
		 * Calculates the maximum height of the sub-tree with the current head node as the root node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - an integer value representing the maximum height of the sub-tree with the current head node.
		 */
		[[nodiscard]] int height() const noexcept {
			return calc_max_height(current_head);
		}

		/**
		 * Provides a boolean value that indicates whether the tree is empty and uninitialised.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a boolean value that indicates whether the root node is `nullptr`.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return root == nullptr;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to true if the current head node of the tree is **not**
		 * `nullptr`.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a boolean value indicating whether the current head node is `nullptr`.
		 */
		explicit operator bool() const noexcept {
			return current_head != nullptr;
		}

		/**
		 * Returns a const reference to the data member of the left child node of the current head node.
		 *
		 * If the current head node and its left child node are uninitialised, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a const reference to the data member of the left child node of the current head node.
		 */
		const T& show_left() const {
			if (current_head && current_head->left)
				return current_head->left->data;
			throw std::runtime_error("Error: left node is empty");
		}

		/**
		 * Returns a const reference to the data member of the right child node of the current head node.
		 *
		 * If the current head node and its left child node are uninitialised, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a const reference to the data member of the right child node of the current head node.
		 */
		const T& show_right() const {
			if (current_head && current_head->right)
				return current_head->right->data;
			throw std::runtime_error("Error: right node is empty");
		}

		/**
		 * Iterates through the tree in pre-order traversal and appends the value of each node to a `std::vector` of
		 * type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - a `std::vector` of type `T` containing the value of each node in the tree after pre-order traversal.
		 */
		[[nodiscard]] std::vector<T> contents_PreOrder() const noexcept {
			std::vector<T> temp = {};
			return PreOrder(root, temp);
		}

		/**
		 * Iterates through the tree in in-order traversal and appends the value of each node to a `std::vector` of
		 * type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - a `std::vector` of type `T` containing the value of each node in the tree after in-order traversal.
		 */
		[[nodiscard]] std::vector<T> contents_InOrder() const noexcept {
			std::vector<T> temp = {};
			return InOrder(root, temp);
		}

		/**
		 * Iterates through the tree in post-order traversal and appends the value of each node to a `std::vector` of
		 * type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - a `std::vector` of type `T` containing the value of each node in the tree after post-order traversal.
		 */
		[[nodiscard]] std::vector<T> contents_PostOrder() const noexcept {
			std::vector<T> temp = {};
			return PostOrder(root, temp);
		}

		/**
		 * Removes the left child node of the current head node, including its sub-tree, and sets its pointer to `nullptr`.
		 *
		 * If the current head or its left child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 */
		void remove_left() {
			if (current_head && current_head->left)
				delete_tree(current_head->left);
			else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot remove left node.");
			} else
				throw std::runtime_error("Error: Left node is uninitialised, there is nothing to remove");
		}

		/**
		 * Removes the right child node of the current head node, including its sub-tree, and sets its pointer to `nullptr`.
		 *
		 * If the current head or its right child is uninitialized, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 */
		void remove_right() {
			if (current_head && current_head->right)
				delete_tree(current_head->right);
			else if (!current_head) {
				throw std::runtime_error("Current head node is not initialized, cannot remove right node.");
			} else
				throw std::runtime_error("Error: Right node is uninitialised, there is nothing to remove");
		}

		/**
		 * Clears the whole tree, starting from the root and deallocates all the memory for each node. Sets the root
		 * and current head to `nullptr`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		void clear() noexcept {
			delete_tree(root);
			root = nullptr;
			current_head = root;
		}

		/**
		 * BinaryTree destructor which will call clear() to clear the tree from the root if it is initialized.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		~BinaryTree() {
			if (root)
				clear();
		}

	private:
		/**
		 * A node structure to contain the data, of type `T` for each node in the tree and Node pointers for the
		 * left and right children nodes.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each node. */
			Node* left = nullptr;  /**< Pointer to the left child node of this node. */
			Node* right = nullptr;  /**< Pointer to the right child node of this node. */

			/**
			 * Constructor which copies the data provided into the node object and initialises the left and right
			 * child Node pointers to `nullptr`.
			 * @param data - data of type `T` to copy into the node object.
			 */
			explicit Node(const T& data) noexcept: data(data) {}

			/**
			 * Constructor which copies the data provided into the node object and initialises the left and right
			 * child Node pointers to `nullptr`.
			 * @param data - a *r-value reference* to data of type `T` to move into the node object.
			 */
			explicit Node(T&& data) noexcept: data(std::move(data)) {}
		};

		Node* root;  /**< Pointer to the root node of the tree. */
		Node* current_head;  /**< A pointer to a node in the tree currently in context. */

		/**
		 * Private helper function to help recursively traverse the tree pre-order and add each node's data to
		 * a `std::vector` of type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @param node - a pointer to the current node being traversed.
		 * @param data - a reference to the `std::vector` of type `T` containing the data of each node.
		 * @return - a reference to the `std::vector` of type `T` containing the data.
		 */
		std::vector<T>& PreOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				data.push_back(node->data);
				PreOrder(node->left, data);
				PreOrder(node->right, data);
			}
			return data;
		}

		/**
		 * Private helper function to help recursively traverse the tree in-order and add each node's data to
		 * a `std::vector` of type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @param node - a pointer to the current node being traversed.
		 * @param data - a reference to the `std::vector` of type `T` containing the data of each node.
		 * @return - a reference to the `std::vector` of type `T` containing the data.
		 */
		std::vector<T>& InOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				InOrder(node->left, data);
				data.push_back(node->data);
				InOrder(node->right, data);
			}
			return data;
		}

		/**
		 * Private helper function to help recursively traverse the tree post-order and add each node's data to
		 * a `std::vector` of type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @param node - a pointer to the current node being traversed.
		 * @param data - a reference to the `std::vector` of type `T` containing the data of each node.
		 * @return - a reference to the `std::vector` of type `T` containing the data.
		 */
		std::vector<T>& PostOrder(Node* node, std::vector<T>& data) const noexcept {
			if (node != nullptr) {
				PostOrder(node->left, data);
				PostOrder(node->right, data);
				data.push_back(node->data);
			}
			return data;
		}

		/**
		 * Private helper function to help calculate the maximum height of a sub-tree at a specified node to its
		 * furthest leaf node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @param node - a pointer to the node to calculate the maximum height of.
		 * @return - an integer value representing the maximum height of the specified node.
		 */
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

		/**
		 * Private helper function to help clear and deallocate memory for a sub-tree originating from a specified node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @param node - a reference to the pointer to a node.
		 */
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