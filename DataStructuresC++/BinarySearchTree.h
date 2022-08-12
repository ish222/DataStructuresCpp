#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace custom {
	/**
	 * A template implementation of a specialised tree data structure where each node can have at most two children
	 * nodes and the children nodes are ordered based on value, where the left child of each node will have a lower
	 * value and the right child of the node will have a higher value than the parent node. Each node has a member
	 * data of type `T` and pointers to the left and right children nodes.
	 *
	 * \note
	 * In order for the nodes to be ordered based on data values, the data type `T` must be arithmetic.
	 *
	 * @tparam T - the type of the data of each node in the tree.
	 * @see <a href="https://en.wikipedia.org/wiki/Binary_search_tree">Binary search tree</a>
	 */
	template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
	class BinarySearchTree {
	public:
		/**
		 * Default BinarySearchTree constructor which sets the root and current head node pointers to `nullptr` and
		 * initialises the private helper variable `left` to false.
		 */
		BinarySearchTree() noexcept: root(nullptr), current_head(nullptr), left(false) {}

		/**
		 * Overloaded BinarySearchTree constructor which takes a value of type `T` and constructs a new node with
		 * the data provided. The root and current head nodes are set to this new node.
		 * @param data - data of type `T` to be copied into the root node.
		 */
		explicit BinarySearchTree(const T& data) noexcept: left(false) {
			root = new Node(data);
			current_head = root;
		}

		/**
		 * Overloaded BinarySearchTree constructor which takes a value of type `T` and constructs a new node with the
		 * data provided, setting it to the root and current head of the tree.
		 * @param data -  - a *r-value reference* to data of type `T` to be moved into the root node.
		 */
		explicit BinarySearchTree(T&& data) noexcept: left(false) {
			root = new Node(std::move(data));
			current_head = root;
		}

		/**
		 * Overloaded BinarySearchTree constructor which takes an argument of an initialiser list of type `T` and
		 * adds its arguments to the tree. The elements in the list are each added to their correct positions in the tree.
		 *
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list + the number of nodes
		 * in the tree.
		 *
		 * @param init - an initialiser list of type `T` whose contents will be added to the tree.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		BinarySearchTree(std::initializer_list<T> init) noexcept: left(false) {
			for (auto it = init.begin(); it != init.end(); ++it)
				add(std::move(*it));
		}

		/**
		 * Adds a node with the data, of type `T`, provided into its corresponding ordered position in the tree.
		 *
		 * If a node with the value provided already exists in the tree, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(log(n))* where n is the number of nodes in the tree.
		 *
		 * @param data - data of type `T` to be copied into the new node.
		 */
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

		/**
		 * Adds a node with the data, of type `T`, provided into its corresponding ordered position in the tree.
		 *
		 * If a node with the value provided already exists in the tree, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(log(n))* where n is the number of nodes in the tree.
		 *
		 * @param data - a *r-value reference* of the data of type `T` to be moved into the new node.
		 */
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

		/**
		 * Obtain the maximum height of the tree, the distance from the root node to its furthest leaf node. If the tree
		 * is uninitialized, the value of **-1** is returned.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - an integer value representing the maximum height of the tree, or **-1** if the tree is uninitialized.
		 */
		[[nodiscard]] int height() const noexcept {
			if (root)
				return calc_max_height(root);
			return -1;
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
		 * Conversion operator for boolean type. Evaluates to true if the root node of the tree is **not**
		 * `nullptr`.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @return - a boolean value indicating whether the root node is `nullptr`.
		 */
		explicit operator bool() const noexcept {
			return root != nullptr;
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
		 * Removes the node, with the data value specified, from the tree and reorders the tree while maintaining the
		 * deleted node's children nodes.
		 *
		 * If a node, with the data value specified, is not found in the tree, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(log(n))* where n is the number of nodes in the tree.
		 *
		 * @param val - the value of the node to be removed.
		 */
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

		/**
		 * Clears all the node elements in the tree and deallocates the memory of all nodes by calling delete_tree() and
		 * passing the root as its argument. Sets the root node pointer to `nullptr`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		void clear() noexcept {
			delete_tree(root);
			root = nullptr;
		}

		/**
		 * BinarySearchTree destructor which calls clear() to clear all the node elements in the tree if the tree is
		 * initialized.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		~BinarySearchTree() {
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
			Node* left = nullptr;  /**< Pointer to the left child node of this node, which will have a lesser value. */
			Node* right = nullptr;  /**< Pointer to the right child node of this node, which will have a greater value. */

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
		Node* current_head;  /**< A pointer to a node in the tree currently in context, which in this class is mainly used to utility use. */
		bool left;  /**< A private helper member which is used by tree-altering functions. */

		/**
		 * Private helper function which find a node with the data specified, starting from the root node provided.
		 *
		 * **Time Complexity** = *O(log(n))* where n is the number of nodes in the tree.
		 *
		 * @param data - the data to search in a node for.
		 * @param node - a pointer to a node to act as a root of a sub-tree to search the node for.
		 * @return - a pointer to the node with the data specified or `nullptr` if a node with the data specified is not found.
		 */
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

		/**
		 * Retrieves a pointer to the node with the lowest value in the tree.
		 *
		 * **Time Complexity** = *O(n)* where n is the maximum height of the tree.
		 *
		 * @param node - a pointer to the root node to initiate the search from.
		 * @return - a pointer to the node with the lowest value in the tree.
		 */
		Node* min_value(Node* node) noexcept {
			Node* cur = node;
			while (cur->left) {
				current_head = cur;
				cur = cur->left;
			}
			return cur;
		}

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
			if (node) {
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
			if (node) {
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
			if (node) {
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
			if (node->left)
				delete_tree(node->left);
			if (node->right)
				delete_tree(node->right);
			delete node;
		}
	};
}// namespace custom

#endif//BINARY_SEARCH_TREE_H