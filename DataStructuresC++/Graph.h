#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

namespace GP {

	struct Node {
		int data;
		char id;
	};

	class Graph {
	public:
		Graph(const int& data) {
			Node* new_node = new Node();
			new_node->data = data;
			node_num = 0;
			new_node->id = 65+node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*>{new_node});
		}

		void add_node(const int& data) {
			Node* new_node = new Node();
			new_node->data = data;
			node_num++;
			new_node->id = 65+node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*>{new_node});
		}

		void add_edge(char last, char next) {
			Node* last_node = NULL;
			Node* next_node = NULL;
			for (Node* node : node_list) {
				if (node->id == last)
					last_node = node;
				else if (node->id == next)
					next_node = node;
			}
			if (last_node != NULL && next_node != NULL) {
				adj_list[(int)last - 65].push_back(next_node);
			}
		}

		std::vector<int> contents() {
			std::vector<int> contents = {};
			for (Node* node : node_list) {
				contents.push_back(node->data);
			}
			return contents;
		}

		void print() {
			for (std::vector<Node*> links : adj_list) {
				for (Node* node : links) {
					std::cout << node->id << " : " << node->data << "\t->\t";
				}
				std::cout << "\n";
			}
		}

	private:
		std::vector<std::vector<Node*>> adj_list;
		std::vector<Node*> node_list;
		int node_num;
	};

}