#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

namespace GP {

	template <typename T, typename U>
	struct Node {
		T data;
		U id;
	};

	template <typename T, typename U = char>
	class Graph {
	public:
		Graph(const int& data, bool directed = true) {
			Node<T, U>* new_node = new Node<T, U>();
			new_node->data = data;
			node_num = 0;
			new_node->id = 65+node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node<T, U>*>{new_node});
			this->directed = directed;
		}

		void add_node(const int& data) {
			Node<T, U>* new_node = new Node<T, U>();
			new_node->data = data;
			node_num++;
			new_node->id = 65+node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node<T, U>*>{new_node});
		}

		void add_edge(const char& last, const char& next) {
			Node<T, U>* last_node = nullptr;
			Node<T, U>* next_node = nullptr;
			for (Node<T, U>* node : node_list) {
				if (node->id == last)
					last_node = node;
				else if (node->id == next)
					next_node = node;
			}
			if (last_node != nullptr && next_node != nullptr) {
				adj_list[(int)last - 65].push_back(next_node);
				if (!directed) {
					adj_list[(int)next - 65].push_back(last_node);
				}
			}
			else {
				throw std::runtime_error("Invalid node IDs");
			}
		}

		void change(const U& id, const T& data) {
			bool found = false;
			for (Node<T, U>*& node : node_list) {
				if (node->id == id) {
					node->data = data;
					found = true;
				}	
			}
			if (found == false)
				throw std::runtime_error("Invalid node ID");
		}

		int num_node() const {
			return node_list.size();
		}

		bool empty() const {
			return node_list.size() == 0;
		}

		operator bool() const {
			return (empty() != 0);
		}

		bool find(const char& id) const {
			for (Node<T, U>*& node : node_list) {
				if (node->id == id)
					return true;
			}
			return false;
		}

		bool find_edge(const char& last, const char& next) const {
			int last_index = (int)last - 65;
			int next_index = (int)next - 65;
			bool found = false;
			for (int i = 0; i < adj_list[last_index].size(); i++) {
				if (adj_list[last_index][i]->id == next) {
					found = true;
					break;
				}
			}
			return found;
		}

		std::vector<int> contents() const {
			std::vector<int> contents = {};
			for (Node<T, U>*& node : node_list) {
				contents.push_back(node->data);
			}
			return contents;
		}

		void print() const {
			for (std::vector<Node<T, U>*> links : adj_list) {
				for (Node<T, U>*& node : links) {
					std::cout << node->id << " : " << node->data << "\t->\t";
				}
				std::cout << "END\n";
			}
		}

		void remove(const char& id) {
			Node<T, U>* node = nullptr;
			for (int i = 0; i < node_list.size(); i++) {
				if (node_list[i]->id == id) {
					node = node_list[i];
					adj_list.erase(adj_list.begin() + i);
					node_list.erase(node_list.begin() + i);
					break;
				}
			}
			for (std::vector<Node<T, U>*>& links : adj_list) {
				for (int i = 0; i < links.size(); i++) {
					if (links[i]->id == id) {
						links.erase(links.begin() + i);
					}
				}
			}
			delete node;
		}

		void clear() {
			for (Node<T, U>*& node : node_list) {
				delete node;
			}
			node_list.clear();
			adj_list.clear();
		}

		~Graph() {
			if (node_list.empty() == false)
				clear();
		}

	private:
		std::vector<std::vector<Node<T, U>*>> adj_list;
		std::vector<Node<T, U>*> node_list;
		int node_num;
		bool directed;
	};

}