#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace custom {
	/**
	 * A template implementation of a tree data structure. Each node in the tree has a member data of type `T` and a
	 * vector of node pointers containing all its children nodes.
	 *
	 * There is an option of having the children nodes of each parent node ordered, only if the data type `T` is arithmetic.
	 *
	 * @tparam T - the type of the data of each node in the tree.
	 * @see <a href="https://en.wikipedia.org/wiki/Tree_(data_structure)">Tree (Data Structure)</a>
	 */
	template<typename T>
	class Tree {
	public:
		/**
		 * Default Tree constructor which sets the root and current head node to `nullptr` and sets the `ordered` status
		 * to `false`.
		 */
		Tree() noexcept: root(nullptr), current_head(nullptr), ordered(false) {}

		/**
		 * Overloaded Tree constructor which sets the root node to contain the data of type `T` provided. It sets the
		 * current head node to the root node and sets the `ordered` status to the boolean value provided, which has
		 * the default argument value of `false`.
		 *
		 * \note
		 * If `ordered` is set to `true`, the data type `T` must be arithmetic.
		 *
		 * @param data - data of type `T` to be copied into the root node.
		 * @param ordered - boolean value to set the `ordered` status of the children nodes, set by default to `false`.
		 */
		explicit Tree(const T& data, bool ordered = false) noexcept: ordered(ordered) {
			root = new Node(data);
			current_head = root;
			if (ordered)
				static_assert(std::is_arithmetic<T>::value,
				              "Ordered trees require arithmetic data types");  // Ensures that the data type is arithmetic if the children are to be ordered
		}

		/**
		 * Overloaded Tree constructor which sets the root node to contain the data of type `T` provided. It sets the
		 * current head node to the root node and sets the `ordered` status to the boolean value provided, which has
		 * the default argument value of `false`.
		 *
		 * \note
		 * If `ordered` is set to `true`, the data type `T` must be arithmetic.
		 *
		 * @param data - a *r-value reference* to data of type `T` to be moved into the root node.
		 * @param ordered - boolean value to set the `ordered` status of the children nodes, set by default to `false`.
		 */
		explicit Tree(T&& data, bool ordered = false) noexcept: ordered(ordered) {
			root = new Node(std::move(data));
			current_head = root;
			if (ordered)
				static_assert(std::is_arithmetic<T>::value, "Ordered trees require arithmetic data types");
		}

		/**
		 * Adds a child node to the list of children of the current head node. If the `ordered` status is `true`, the
		 * child node is added to the list of children to fit in ascending order.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)* if unordered, otherwise *O(n)* where n is the number of children nodes.
		 *
		 * @param data - data of type `T` to be copied into the new child node.
		 */
		void add_child(const T& data) {
			if (current_head) {
				Node* new_node = new Node(data);
				if (!current_head->children.empty() && ordered) {
					for (size_t i = 0; i < current_head->children.size(); ++i) {
						if (new_node->data < current_head->children[i]->data) {
							current_head->children.insert(current_head->children.begin() + i, new_node);
							return;
						}
					}
				}
				current_head->children.push_back(new_node);
				return;
			}
			throw std::runtime_error("Current node is uninitialised, cannot add child");
		}

		/**
		 * Adds a child node to the list of children of the current head node. If the `ordered` status is `true`, the
		 * child node is added to the list of children to fit in ascending order.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)* if unordered, otherwise *O(n)* where n is the number of children nodes.
		 *
		 * @param data - data of type `T` to be moved into the new child node.
		 */
		void add_child(T&& data) {
			if (current_head) {
				Node* new_node = new Node(std::move(data));
				if (!current_head->children.empty() && ordered) {
					for (size_t i = 0; i < current_head->children.size(); ++i) {
						if (new_node->data < current_head->children[i]->data) {
							current_head->children.insert(current_head->children.begin() + i, new_node);
							return;
						}
					}
				}
				current_head->children.push_back(new_node);
				return;
			}
			throw std::runtime_error("Current node is uninitialised, cannot add child");
		}

		/**
		 * Adds nodes with data from an initialiser list of type `T` to the children list of the current head node.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(n)* where n is the number elements in the initialiser list.
		 *
		 * @param list - the initialiser list whose data is to be added to the nodes and then the children list of the
		 * current head.
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/initializer_list">std::initializer_list</a>
		 */
		void add_child(std::initializer_list<T> list) {
			if (current_head) {
				for (auto it = list.begin(); it != list.end(); ++it)
					add_child(std::move(*it));
			} else
				throw std::runtime_error("Current node is uninitialised, cannot add child");
		}

		/**
		 * Obtains the data values in order, of type `T`, of all the children nodes of the current head node.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of children nodes.
		 *
		 * @return - a `std::vector` of type `T` containing the data values of the children nodes.
		 */
		[[nodiscard]] std::vector<T> children_data() const {
			if (!current_head->children.empty()) {
				std::vector<T> ret;
				for (const Node*& node: current_head->children) {
					ret.push_back(node->data);
				}
				return ret;
			}
			throw std::runtime_error("Current node has no children");
		}

		/**
		 * Finds the index of the child node, with the specified data value, in the children list of the current head
		 * node. If multiple children nodes with the same data exist, the index of the first child is returned. If a
		 * child node with the data value is not found, **-1** is returned.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of children nodes.
		 *
		 * @param data - data of type `T` to search for in the children nodes.
		 * @return - an integer specifying the index of the child node or **-1** if no child node with the data value
		 * specified is found.
		 */
		[[nodiscard]] int find_child(const T& data) const {
			if (!current_head->children.empty()) {
				int index = 0;
				for (Node* node: current_head->children) {
					if (node->data == data)
						return index;
					++index;
				}
				return -1;
			}
			throw std::runtime_error("Current node has no children");
		}

		/**
		 * Changes the context of the current head to a child node of the current head at the index specified.
		 *
		 * If an index out of the range of the children nodes list is provided, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param index - an integer value specifying the index of a child node to change the current head node to.
		 */
		void goto_child(const int& index) {
			if (index < current_head->children.size() && index > -1)
				current_head = current_head->children[index];
			else
				throw std::invalid_argument("Index out of range.");
		}

		/**
		 * Changes the context of the current head node to the root of the tree.
		 *
		 * **Time Complexity** = *O(1)*.
		 */
		void goto_root() noexcept {
			current_head = root;
		}

		/**
		 * Obtains the height of the current head node from the furthest descendant leaf node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number nodes in the sub-tree originating from the current head.
		 *
		 * @return - an integer value representing the height of the current head node.
		 */
		[[nodiscard]] int current_height() const noexcept {
			return get_depth(current_head);
		}

		/**
		 * Obtains the maximum height of the tree, that is the number of nodes from the root node to the furthest leaf
		 * node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - an integer value representing the maximum height of the tree.
		 */
		[[nodiscard]] int max_height() const noexcept {
			return get_depth(root);
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
		 * Adds the contents of the whole tree, in order, to a `std::vector` of type `T` and returns it.
		 *
		 * If the current head node is uninitialised, i.e. `nullptr`, a `runtime_error` exception is thrown.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 *
		 * @return - a `std::vector` of type `T` containing the contents of the whole tree, in order.
		 */
		[[nodiscard]] std::vector<T> contents_InOrder() const {
			if (root) {
				std::vector<T> temp;
				return InOrder(root, temp);
			}
			throw std::runtime_error("Error: Tree is empty, there is no content to return");
		}

		/**
		 * Removes a child from the children nodes list at a given index.
		 *
		 * If the index provided is out of the range of the children nodes list, an `invalid_argument` exception is thrown.
		 *
		 * **Time Complexity** = *O(1)*.
		 *
		 * @param index - an integer specifying the index of the child node to remove.
		 */
		void remove_child(const int& index) {
			if (index < current_head->children.size() && index > -1)
				current_head->children.erase(current_head->children.begin() + index);
			else
				throw std::invalid_argument("Index for remove_child is out of range");
		}

		/**
		 * Clears the sub-tree with the current head as the root and sets the current head to `nullptr`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number nodes in the sub-tree originating from the current head.
		 */
		void delete_subtree() noexcept {
			delete_tree(current_head);
			current_head = nullptr;
		}

		/**
		 * Clears the whole tree, starting from the root and sets the root and current head to `nullptr`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		void clear() noexcept {
			delete_tree(root);
			root = nullptr;
			current_head = root;
		}

		/**
		 * Tree destructor which calls clear() if the tree is initialised, clearing its contents and freeing memory.
		 *
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the tree.
		 */
		~Tree() {
			if (root)
				clear();
		}

	private:
		/**
		 * A node structure to contain the data, of type `T` for each node in the tree and a `std::vector` of Node
		 * pointers containing the children nodes of this specific node.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each node. */
			std::vector<Node*> children;  /**< A `std::vector` of type `Node*` specifying the children nodes of this node. */

			/**
			 * Constructor which copies the data provided into the node object and initialises an empty children list.
			 * @param data - data of type `T` to copy into the node object.
			 */
			explicit Node(const T& data) noexcept: data(data), children({}) {}

			/**
			 * Constructor which moves the data provided into the node object and initialises an empty children list.
			 * @param data - a *r-value reference* to data of type `T` to move into the node object.
			 */
			explicit Node(T&& data) noexcept: data(std::move(data)), children({}) {}
		};

		Node* root;  /**< A pointer to the root node of the tree. */
		Node* current_head;  /**< A pointer to a node in the tree currently in context. */
		bool ordered;  /**< A boolean value which indicates whether the children nodes are ordered in ascending order. */

		/**
		 * Private helper function which traverses the tree recursively, in order and appends the data at each node
		 * to a `std::vector` of type `T`.
		 *
		 * **Time Complexity** = *O(n)* where n is the number nodes in the sub-tree originating from the node provided.
		 *
		 * @param node - a pointer to a node to traverse.
		 * @param data - a reference to a `std::vector` of type `T` to append the data of each node to.
		 * @return - a reference to the same vector passed in as `data`.
		 */
		std::vector<T>& InOrder(Node* node, std::vector<T>& data) const {
			if (!node) return data;
			int child_count = node->children.size();
			for (int i = 0; i < child_count - 1; ++i)
				InOrder(node->children[i], data);
			data.push_back(node->data);
			if (child_count)
				InOrder(node->children[child_count - 1], data);
			return data;
		}

		/**
		 * Private helper function which helps calculate the depth of a given node in the tree.
		 *
		 * **Time Complexity** = *O(n)* where n is the number nodes in the sub-tree originating from the node provided.
		 *
		 * @param node - a pointer to a node to calculate the depth of.
		 * @return - an intger value specifying the depth of the specified node.
		 */
		int get_depth(Node* node) const noexcept {
			if (!node) return 0;
			int max_depth = 0;
			for (Node* child: node->children) {
				max_depth = std::max(max_depth, get_depth(child));
			}
			return ++max_depth;
		}

		/**
		 * Private helper function which deallocates memory and clears a sub-tree with the root of a specified node.
		 *
		 * **Time Complexity** = *O(n)* where n is the number nodes in the sub-tree originating from the node provided.
		 *
		 * @param node - a pointer to a node which is to be the root of the cleared tree.
		 */
		void delete_tree(Node*& node) noexcept {
			if (!node) return;
			if (node->children.empty()) {
				delete node;
				return;
			}
			for (Node*& child: node->children) {
				delete_tree(child);
			}
		}
	};
}// namespace custom

#endif// TREE_H