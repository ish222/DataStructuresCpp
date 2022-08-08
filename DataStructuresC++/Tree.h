#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace custom {
	template<typename T>
	class Tree {
	public:
		Tree() noexcept : root(nullptr), current_head(nullptr), ordered(false) {}

		explicit Tree(const T& data, bool ordered = false) noexcept : ordered(ordered) {
			root = new Node(data);
			current_head = root;
			if (ordered)
				static_assert(std::is_arithmetic<T>::value, "Ordered trees require arithmetic data types");
		}

		explicit Tree(T&& data, bool ordered = false) noexcept : ordered(ordered) {
			root = new Node(std::move(data));
			current_head = root;
			if (ordered)
				static_assert(std::is_arithmetic<T>::value, "Ordered trees require arithmetic data types");
		}

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

		void add_child(std::initializer_list<T> list) {
			if (current_head) {
				for (auto it = list.begin(); it != list.end(); ++it)
					add_child(std::move(*it));
			} else
				throw std::runtime_error("Current node is uninitialised, cannot add child");
		}

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

		void goto_child(const int& index) {
			if (index < current_head->children.size() && index > -1)
				current_head = current_head->children[index];
			else
				throw std::invalid_argument("Index out of range.");
		}

		void goto_root() noexcept {
			current_head = root;
		}

		[[nodiscard]] int current_height() const noexcept {
			return get_depth(current_head);
		}

		[[nodiscard]] int max_height() const noexcept {
			return get_depth(root);
		}

		[[nodiscard]] bool empty() const noexcept {
			return root == nullptr;
		}

		explicit operator bool() const noexcept {
			return current_head != nullptr;
		}

		[[nodiscard]] std::vector<T> contents_InOrder() const {
			if (root) {
				std::vector<T> temp;
				return InOrder(root, temp);
			}
			throw std::runtime_error("Error: Tree is empty, there is no content to return");
		}

		void remove_child(const int& index) {
			if (index < current_head->children.size() && index > -1)
				current_head->children.erase(current_head->children.begin() + index);
			else
				throw std::invalid_argument("Index for remove_child is out of range");
		}

		void delete_subtree() noexcept {
			delete_tree(current_head);
			current_head = nullptr;
		}

		void clear() noexcept {
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

			explicit Node(const T& data) noexcept : data(data), children({}) {}

			explicit Node(T&& data) noexcept : data(std::move(data)), children({}) {}
		};

		Node* root;
		Node* current_head;
		bool ordered;// Orders children in ascending values

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

		int get_depth(Node* node) const noexcept {
			if (!node) return 0;
			int max_depth = 0;
			for (Node* child: node->children) {
				max_depth = std::max(max_depth, get_depth(child));
			}
			return ++max_depth;
		}

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