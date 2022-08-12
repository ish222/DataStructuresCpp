#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <deque>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

namespace custom {
	/**
	 * A template implementation of a graph data structure. Each node element has an ID with the type `ID_Type`
	 * and data with the type `T`. Each node also has connections with other nodes, called edges. This is an undirected
	 * graph meaning each edge has a two-way connection to and from each of the two connecting nodes. Utility functions
	 * and algorithm functions such as depth-first search and breadth-first search are also implemented. This graph
	 * implementation uses an adjacency list to store the nodes and their connections.
	 * @tparam T - the type of the data of each node in the graph.
	 * @tparam ID_Type - the type of the ID used to identify each node in the graph.
	 * @see <a href="https://en.wikipedia.org/wiki/Graph_(abstract_data_type)">Graph data structure</a>
	 */
	template<typename T, typename ID_Type>
	class Graph {
	public:
		/**
		 * Default Graph constructor which initializes an empty adjacency list, node list and sets the node number to 0.
		 */
		Graph() noexcept: adj_list({}), node_list({}), node_num(0) {}

		/**
		 * Overloaded Graph constructor which allocates memory for a node with the ID and data provided and adds
		 * the node to the adjacency and node lists.
		 * @param data - data of type `T` to be copied into the node.
		 * @param id - ID of type `ID_Type`, to be copied into the node and used to identify the node.
		 */
		explicit Graph(const T& data, const ID_Type& id) noexcept: node_num(1) {
			Node* new_node = new Node(data, id);
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*> {new_node});
		}

		/**
		 * Overloaded Graph constructor which allocates memory for a node with the ID and data provided and adds
		 * the node to the adjacency and node lists.
		 * @param data - an *r-value reference* to the data of type `T` to be moved into the node.
		 * @param id - an *r-value reference* to the ID of type `ID_Type`, to be moved into the node and used to identify the node.
		 */
		explicit Graph(T&& data, ID_Type&& id) noexcept: node_num(1) {
			Node* new_node = new Node(std::move(data), std::move(id));
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*> {new_node});
		}

		/**
		 * Copy constructor for a Graph which will perform a deep copy, element-wise, of another Graph
		 * object of the same types `T` and `ID_Type`, including its edges.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the other graph.
		 * @param other - another Graph object of the same types `T` and `ID_Type` to be copied.
		 */
		Graph(const Graph<T, ID_Type>& other) noexcept {
			node_list = {};
			adj_list = {};
			node_num = 0;
			for (Node*& node: other.node_list)
				add_node(node->data);
			for (std::vector<Node*> link: other.adj_list) {
				ID_Type first = link[0]->id;
				for (int i = 1; i < link.size(); ++i) {
					Node* last_node = nullptr;
					Node* next_node = nullptr;
					for (Node* node: node_list) {
						if (node->id == first)
							last_node = node;
						else if (node->id == link[i]->id)
							next_node = node;
					}
					if (last_node && next_node) {
						size_t first_index = std::find(adj_list.begin(), adj_list.end(), first);
						adj_list[first_index].push_back(next_node);
						size_t last_index = std::find(adj_list.begin(), adj_list.end(), last_node->id);
						adj_list[last_index].push_back(last_node);
					}
				}
			}
		}

		/**
		 * Copy assignment operator for the Graph which will copy another Graph object of the same types
		 * `T` and `ID_Type` into the current object, including its edges.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the other graph + the number of nodes
		 * in the current graph.
		 * @param other - another Graph object of the same types `T` and `ID_Type` to be copied.
		 * @return - a reference to the current object.
		 */
		Graph<T, ID_Type>& operator=(const Graph<T, ID_Type>& other) noexcept {
			if (this != &other) {
				if (node_num)
					clear();
				node_list = {};
				adj_list = {};
				node_num = 0;
				for (Node*& node: other.node_list)
					add_node(node->data);
				for (std::vector<Node*> link: other.adj_list) {
					ID_Type first = link[0]->id;
					for (int i = 1; i < link.size(); ++i)
						add_edge(first, link[i]->id);
				}
			}
			return *this;
		}

		/**
		 * Move constructor for a Graph which will take the data from another Graph object of the same types
		 * `T` and `ID_Type` and set the other object to its default state of not have any data.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Graph object of types `T` and `ID_Type` to be moved.
		 */
		Graph(Graph<T, ID_Type>&& other) noexcept {
			node_list = other.node_list;
			adj_list = other.adj_list;
			node_num = other.node_num;
			other.node_num = 0;
			other.node_list.clear();
			other.adj_list.clear();
		}

		/**
		 * Move assignment operator for the Graph which will move another Graph object of types `T` and `ID_Type` into
		 * the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Graph object of types `T` and `ID_Type` to be moved.
		 * @return - a reference to the current object.
		 */
		Graph<T, ID_Type>& operator=(Graph<T, ID_Type>&& other) noexcept {
			if (this != &other) {
				if (node_num)
					clear();
				node_list = other.node_list;
				adj_list = other.adj_list;
				node_num = other.node_num;
				other.node_num = 0;
				other.node_list.clear();
				other.adj_list.clear();
			}
			return *this;
		}

		/**
		 * Allocates memory for and adds a new new node to the graph.
		 * **Time Complexity** = *O(1)*.
		 * @param data - data of type `T` to be copied into the node.
		 * @param id - ID of type `ID_Type`, to be copied into the node and used to identify the node.
		 */
		void add_node(const T& data, const ID_Type& id) noexcept {
			Node* new_node = new Node(data, id);
			++node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*> {new_node});
		}

		/**
		 * Allocates memory for and adds a new new node to the graph.
		 * **Time Complexity** = *O(1)*.
		 * @param data -  an *r-value reference* to the data of type `T` to be moved into the node.
		 * @param id - an *r-value reference* to the ID of type `ID_Type`, to be moved into the node and used to identify the node.
		 */
		void add_node(T&& data, ID_Type&& id) noexcept {
			Node* new_node = new Node(std::move(data), std::move(id));
			++node_num;
			node_list.push_back(new_node);
			adj_list.push_back(std::vector<Node*> {new_node});
		}

		/**
		 * Adds a connection (edge) between two specified nodes. If the node IDs provided are invalid, a `runtime_error`
		 * exception is thrown. As this is an undirected graph, the connection between the two nodes is two-way.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 * @param last - the ID of type `ID_Type` of the first node.
		 * @param next - the ID of type `ID_Type` of the second node.
		 */
		virtual void add_edge(const ID_Type& last, const ID_Type& next) {
			Node* last_node = nullptr;
			Node* next_node = nullptr;
			int last_index;
			int next_index;
			for (int i = 0; i < node_list.size(); ++i) {
				if (node_list[i]->id == last) {
					last_node = node_list[i];
					last_index = i;
				} else if (node_list[i]->id == next) {
					next_node = node_list[i];
					next_index = i;
				}
			}
			if (last_node && next_node) {
				adj_list[last_index].push_back(next_node);
				adj_list[next_index].push_back(last_node);
			} else
				throw std::runtime_error("Invalid node IDs, cannot add edge");
		}

		/**
		 * Changes the data of a specified node. If a node with the ID specified is not found, a `runtime_error`
		 * exception is thrown.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 * @param id - the ID of type `ID_Type` of the node whose data is to be changed.
		 * @param data - the data of type `T` to change the node's data to.
		 */
		void change(const ID_Type& id, const T& data) {
			for (Node*& node: node_list) {
				if (node->id == id) {
					node->data = data;
					return;
				}
			}
			throw std::runtime_error("Invalid node ID");
		}

		/**
		 * Provides a value for the number of nodes in the graph.
		 * **Time Complexity** = *O(1)*.
		 * @return - an unsigned integer representing the number of nodes in the graph.
		 */
		[[nodiscard]] size_t size() const noexcept {
			return node_num;
		}

		/**
		 * Provides a boolean value that indicates whether the graph contains any nodes.
		 * **Time Complexity** = *O(1)*.
		 * @return - a boolean value that indicates whether the graph is empty or not.
		 */
		[[nodiscard]] bool empty() const noexcept {
			return node_num == 0;
		}

		/**
		 * Conversion operator for boolean type. Evaluates to `true` if the number of nodes in the graph is not 0, otherwise
		 * it evaluates to `false`.
		 * **Time Complexity** = *O(1)*.
		 * @return - the boolean value of whether the number of nodes in the graph is 0.
		 */
		explicit operator bool() const noexcept {
			return (bool)node_num;
		}

		/**
		 * Checks if a node with the ID provided exists in the graph.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 * @param id - an ID of type `ID_Type` to check in the graph for.
		 * @return - a boolean value indicating whether a node with the ID provided exists in the graph.
		 */
		[[nodiscard]] bool contains(const ID_Type& id) const noexcept {
			for (Node*& node: node_list) {
				if (node->id == id)
					return true;
			}
			return false;
		}

		/**
		 * Checks if a connection (edge) between the two nodes, with the IDs provided, exists.
		 * **Time Complexity** = *O(n)* where n the number of edges in the graph.
		 * @param last - the ID of type `ID_Type` of the first node.
		 * @param next - the ID of type `ID_Type` of the second node.
		 * @return - a boolean value indicating whether an edge exists between the two nodes.
		 */
		[[nodiscard]] bool find_edge(const ID_Type& last, const ID_Type& next) const noexcept {
			int last_index = -1;
			for (int i = 0; i < node_list.size(); ++i) {
				if (node_list[i]->id == last) {
					last_index = i;
					break;
				}
			}
			if (last_index == -1)
				return false;
			for (int i = 0; i < adj_list[last_index].size(); i++) {
				if (adj_list[last_index][i]->id == next)
					return true;
			}
			return false;
		}

		/**
		 * Adds the edges of the graph, with each edge represented by an `std::pair` of type `ID_Type` and `T`, into
		 * a `std::vector` of the pair type specified.
		 * **Time Complexity** = *O(n^2)* (worst-case - if there is an edge between all nodes) where n is the number of
		 * nodes in the graph.
		 * @return - a `std::vector` of type `std::pair<ID_Type, T>` containing each edge in the graph.
		 * @see <a href="https://en.cppreference.com/w/cpp/container/vector">std::vector</a>
		 * @see <a href="https://en.cppreference.com/w/cpp/utility/pair">std::pair</a>
		 */
		[[nodiscard]] std::vector<std::pair<ID_Type, T>> contents() const noexcept {
			std::vector<std::pair<ID_Type, T>> contents = {};
			for (Node*& node: node_list) {
				contents.push_back({node->id, node->data});
			}
			return contents;
		}

		/**
		 * Performs depth-first traversal of the graph and adds the ID of each node connected to the source node
		 * specified into a `std::vector`. Visited nodes are marked so as to prevent infinite cycles.
		 * If a node with the ID provided is not found, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n^2)* (worst-case - if there is an edge between all nodes) where n is the number of
		 * nodes in the graph.
		 * @param id - the ID of type `ID_Type` of the source node to traverse from.
		 * @return - a `std::vector` of type `ID_Type` containing all the connected nodes in the order of depth-first.
		 * @see <a href="https://en.wikipedia.org/wiki/Depth-first_search">Depth-first search</a>
		 */
		[[nodiscard]] std::vector<ID_Type> dfs(const ID_Type& id) const {
			std::vector<ID_Type> ret;
			std::unordered_map<Node*, bool> visited;
			std::stack<Node*> stack;
			int index = find_node_index(id);
			if (index == -1)
				throw std::invalid_argument("Node with id provided does not exist");
			stack.push(node_list[index]);
			while (!stack.empty()) {
				Node* top = stack.top();
				int cur_index = find_node_index(top->id);
				if (!visited[top]) {
					ret.push_back(top->id);
					visited[top] = true;
				}
				stack.pop();
				for (Node* neighbour: adj_list[cur_index]) {
					if (!visited[neighbour]) {
						stack.push(neighbour);
					}
				}
			}
			return ret;
		}

		/**
		 * Performs breadth-first traversal of the graph and adds the ID of each node connected to the source node
		 * specified into a `std::vector`. Visited nodes are marked so as to prevent infinite cycles.
		 * If a node with the ID provided is not found, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n^2)* (worst-case - if there is an edge between all nodes) where n is the number of
		 * nodes in the graph.
		 * @param id - the ID of type `ID_Type` of the source node to traverse from.
		 * @return - a `std::vector` of type `ID_Type` containing all the connected nodes in the order of breadth-first.
		 * @see <a href="https://en.wikipedia.org/wiki/Breadth-first_search">Breadth-first search</a>
		 */
		[[nodiscard]] std::vector<ID_Type> bfs(const ID_Type& id) const {
			std::vector<ID_Type> ret;
			std::unordered_map<Node*, bool> visited;
			std::deque<Node*> queue;
			int index = find_node_index(id);
			if (index == -1)
				throw std::invalid_argument("Node with id provided does not exist");
			visited[node_list[index]] = true;
			queue.push_back(node_list[index]);
			while (!queue.empty()) {
				Node* front = queue.front();
				ret.push_back(front->id);
				queue.pop_front();
				int cur_index = find_node_index(front->id);
				for (Node* neighbour: adj_list[cur_index]) {
					if (!visited[neighbour]) {
						visited[neighbour] = true;
						queue.push_back(neighbour);
					}
				}
			}
			return ret;
		}

		/**
		 * Checks if there is a path between two specified nodes in the graph. If a node for each of the IDs provided
		 * is not found, an `invalid_argument` exception will be thrown.
		 * **Time Complexity** = *O(n^2)* where n is the number of nodes in the graph.
		 * @param last - the ID of type `ID_Type` of the source node.
		 * @param next - the ID of type `ID_Type` of the destination node.
		 * @param use_dfs - a boolean value indicating whether to use depth-first search, set by default to true.
		 * @return - a boolean value indicating whether a path between two specified nodes exists.
		 */
		[[nodiscard]] bool has_path(const ID_Type& last, const ID_Type& next, bool use_dfs = true) {
			int last_index = find_node_index(last);
			int next_index = find_node_index(next);
			if (last_index == -1 || next_index == -1)
				throw std::invalid_argument("Invalid node ids provided for has_path");
			if (use_dfs)
				return dfs_path(node_list[last_index], node_list[next_index]);
			return bfs_path(node_list[last_index], node_list[next_index]);
		}

		/**
		 * Iterates through each node in the graph and prints the node data and its connections to the console,
		 * using `std::cout`. If the graph is uninitialized, i.e. the node list is empty, a `runtime_error` exception
		 * is thrown.
		 * \note
		 * The types `T` and `ID_Type` must both be compatible with `std::cout`.
		 * **Time Complexity** = *O(n^2)* where n is the number of elements in the graph.
		 * @see <a href="https://en.cppreference.com/w/cpp/io/cout">std::cout</a>
		 */
		void print() const {
			if (node_num) {
				for (std::vector<Node*> links: adj_list) {
					for (Node*& node: links) {
						std::cout << node->id << " : " << node->data << "\t->\t";
					}
					std::cout << "END\n";
				}
				std::cout << std::endl;
			} else
				throw std::runtime_error("Graph is empty, there is nothing to print");
		}

		/**
		 * Removes the node and all of its edges from the graph. If a node with the specified ID is not found, an
		 * `invalid_argument` exception is thrown. If the graph is uninitialized, i.e. the node list is empty, a
		 * `runtime_error` exception is thrown.
		 * **Time Complexity** = *O(n^2)* where n is the number of elements in the graph.
		 * @param id - the ID of type `ID_Type` of the node to be removed.
		 */
		void remove(const ID_Type& id) {
			if (node_num) {
				Node* node = nullptr;
				for (int i = 0; i < node_list.size(); ++i) {
					if (node_list[i]->id == id) {
						node = node_list[i];
						adj_list.erase(adj_list.begin() + i);
						node_list.erase(node_list.begin() + i);
						--node_num;
						break;
					}
				}
				if (!node)
					throw std::invalid_argument("Invalid id, this id does not exist");
				for (std::vector<Node*>& links: adj_list) {
					for (int i = 0; i < links.size(); i++) {
						if (links[i]->id == id) {
							links.erase(links.begin() + i);
						}
					}
				}
				delete node;
			} else
				throw std::runtime_error("Graph is empty, there is nothing to remove");
		}

		/**
		 * Erases all nodes from the graph and deallocates its memory. Empties the adjacency list and the node list
		 * vectors.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 */
		void clear() noexcept {
			for (Node*& node: node_list) {
				delete node;
			}
			node_list.clear();
			adj_list.clear();
			node_num = 0;
		}

		/**
		 * Graph destructor which clears the graph and frees any allocated memory.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 */
		virtual ~Graph() {
			if (node_num)
				clear();
		}

	protected:
		/**
		 * A node structure to contain the data and ID of each node in the graph.
		 */
		struct Node {
			T data;  /**< The data of type `T` of each node. */
			ID_Type id;  /**< The ID of type `ID_Type` of each node. */

			/**
			 * Constructor which copies the data and ID provided into the node object.
			 * @param data - the data of type `T` to be copied into the node object.
			 * @param id - the ID of type `ID_Type` to be copied into the node object and used to identify this node.
			 */
			Node(const T& data, ID_Type id) noexcept: data(data), id(id) {}

			/**
			 * Constructor which moves the data and ID provided into the node object.
			 * @param data - a *r-value reference* to the data of type `T` to be moved into the node object.
			 * @param id - a *r-value reference* to the ID of type `ID_Type` to be moved into the node object and used to identify this node.
			 */
			Node(T&& data, ID_Type&& id) noexcept: data(std::move(data)), id(std::move(id)) {}
		};

		std::vector<std::vector<Node*>> adj_list;  /**< An adjacency list comprised of a `std::vector` of `std::vector` of node pointers, specifying the edges of each node. */
		std::vector<Node*> node_list;  /**< A `std::vector` of nodes containing pointers to the nodes in the graph. */
		size_t node_num;  /**< An unsigned integer specifying the number of nodes in the graph. */

		/**
		 * Protected helper function to find the index, in the node list, of a given node ID. If a node with the ID
		 * provided is not found, a value of **-1** is returned.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 * @param id - the node ID of type `ID_Type` to search for.
		 * @return - the index of the node in the node list, or `-1` if not found.
		 */
		[[nodiscard]] int find_node_index(const ID_Type& id) const noexcept {
			for (int i = 0; i < node_list.size(); ++i) {
				if (node_list[i]->id == id)
					return i;
			}
			return -1;
		}

		/**
		 * Protected helper function for has_path() for the case of depth-first search. If the nodes with the two IDs
		 * provided are not found, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n^2)* where n is the number of nodes in the graph.
		 * @param last - the ID of type `ID_Type` of the source node.
		 * @param next - the ID of type `ID_Type` of the destination node.
		 * @return - a boolean value indicating whether a path between the nodes is found using depth-first search.
		 */
		[[nodiscard]] bool dfs_path(Node* last, Node* next) const {
			std::unordered_map<Node*, bool> visited;
			std::stack<Node*> stack;
			int index = find_node_index(last->id);
			if (index == -1)
				throw std::invalid_argument("Node with id provided does not exist");
			stack.push(node_list[index]);
			while (!stack.empty()) {
				Node* top = stack.top();
				int cur_index = find_node_index(top->id);
				if (!visited[top]) {
					if (top == next)
						return true;
					visited[top] = true;
				}
				stack.pop();
				for (Node* neighbour: adj_list[cur_index]) {
					if (!visited[neighbour]) {
						stack.push(neighbour);
					}
				}
			}
			return false;
		}

		/**
		 * Protected helper function for has_path() for the case of breadth-first search. If the nodes with the two IDs
		 * provided are not found, an `invalid_argument` exception is thrown.
		 * **Time Complexity** = *O(n^2)* where n is the number of nodes in the graph.
		 * @param last - the ID of type `ID_Type` of the source node.
		 * @param next - the ID of type `ID_Type` of the destination node.
		 * @return - a boolean value indicating whether a path between the nodes is found using breadth-first search.
		 */
		[[nodiscard]] bool bfs_path(Node* last, Node* next) const {
			std::unordered_map<Node*, bool> visited;
			std::deque<Node*> queue;
			int index = find_node_index(last->id);
			if (index == -1)
				throw std::invalid_argument("Node with id provided does not exist");
			visited[node_list[index]] = true;
			queue.push_back(node_list[index]);
			while (!queue.empty()) {
				Node* front = queue.front();
				if (front == next)
					return true;
				queue.pop_front();
				int cur_index = find_node_index(front->id);
				for (Node* neighbour: adj_list[cur_index]) {
					if (!visited[neighbour]) {
						visited[neighbour] = true;
						queue.push_back(neighbour);
					}
				}
			}
			return false;
		}
	};

	/**
	 * A specialised version of the Graph class where the edges between nodes are directed. This class inherits all
	 * the members from the base Graph class, while override some of the member functions to meet the requirements
	 * stated above.
	 * @tparam T - the type of data to be stored in each node of the directed graph.
	 * @tparam ID_Type - the type of the ID used to identify each node in the directed graph.
	 */
	template<typename T, typename ID_Type>
	class DirectedGraph : public Graph<T, ID_Type> {
	public:
		/**
		 * Default DirectedGraph constructor calls the base Graph constructor.
		 */
		DirectedGraph() noexcept: Graph<T, ID_Type>() {}

		/**
		 * Overloaded DirectedGraph constructor which calls the complementary base Graph constructor, passing and
		 * copying the data and ID provided.
		 * @param data - data of type `T` to be copied into the node.
		 * @param id - ID of type `ID_Type`, to be copied into the node and used to identify the node.
		 */
		explicit DirectedGraph(const T& data, const ID_Type& id) noexcept: Graph<T, ID_Type>(data, id) {}

		/**
		 * Overloaded DirectedGraph constructor which calls the complementary base Graph constructor, passing and
		 * moving the data and ID provided.
		 * an *r-value reference* to the data of type `T` to be moved into the node.
		 * @param id - an *r-value reference* to the ID of type `ID_Type`, to be moved into the node and used to identify the node.
		 */
		explicit DirectedGraph(T&& data, ID_Type&& id) noexcept: Graph<T, ID_Type>(std::move(data), std::move(id)) {}

		/**
		 * Copy constructor for the DirectedGraph class which will perform a deep copy, element-wise, of another DirectedGraph
		 * object of the same types `T` and `ID_Type`, including its edges.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the other graph.
		 * @param other - another DirectedGraph object of the same types `T` and `ID_Type` to be copied.
		 */
		DirectedGraph(const DirectedGraph<T, ID_Type>& other) noexcept {
			node_list = {};
			adj_list = {};
			node_num = 0;
			for (Node*& node: other.node_list)
				add_node(node->data);
			for (std::vector<Node*> link: other.adj_list) {
				ID_Type first = link[0]->id;
				for (int i = 1; i < link.size(); ++i)
					add_edge(first, link[i]->id);
			}
		}

		/**
		 * Copy assignment operator for the DirectedGraph class which will copy another DirectedGraph object of the same
		 * types`T` and `ID_Type` into the current object, including its edges.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the other graph + the number of nodes
		 * in the current graph.
		 * @param other - another Graph object of the same types `T` and `ID_Type` to be copied.
		 * @return - a reference to the current object.
		 */
		DirectedGraph<T, ID_Type>& operator=(const DirectedGraph<T, ID_Type>& other) noexcept {
			if (this != &other) {
				if (node_num)
					clear();
				node_list = {};
				adj_list = {};
				node_num = 0;
				for (Node*& node: other.node_list)
					add_node(node->data);
				for (std::vector<Node*> link: other.adj_list) {
					ID_Type first = link[0]->id;
					for (int i = 1; i < link.size(); ++i)
						add_edge(first, link[i]->id);
				}
			}
			return *this;
		}

		/**
		 * Move constructor for the DirectedGraph class which will call the base Graph class move constructor, passing
		 * an *r-value reference* of a  DirectedGraph object of type `T` and `ID_Type`.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Graph object of types `T` and `ID_Type` to be moved.
		 */
		DirectedGraph(DirectedGraph<T, ID_Type>&& other) noexcept: Graph<T, ID_Type>(std::move(other)) {}

		/**
		 * Move assignment operator for the DirectedGraph class which will move another DirectedGraph object of
		 * types `T` and `ID_Type` into the current object.
		 * \note
		 * If the current object, that is being copied into, is not empty, **it will be cleared**.
		 * **Time Complexity** = *O(1)*.
		 * @param other - an *r-value reference* to a Graph object of types `T` and `ID_Type` to be moved.
		 * @return - a reference to the current object.
		 */
		DirectedGraph<T, ID_Type>& operator=(DirectedGraph<T, ID_Type>&& other) noexcept {
			if (this != &other) {
				if (node_num)
					clear();
				node_list = other.node_list;
				adj_list = other.adj_list;
				node_num = other.node_num;
				other.node_num = 0;
				other.node_list.clear();
				other.adj_list.clear();
			}
			return *this;
		}

		/**
		 * This is an override of the base Graph class add_edge() method where in this case only one edge, from the given
		 * source node to the destination node, is added. If the node IDs provided are invalid, a `runtime_error`
		 * exception is thrown. As this is an undirected graph, the connection between the two nodes is two-way.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 * @param last - the ID of type `ID_Type` of the source node.
		 * @param next - the ID of type `ID_Type` of the destination node.
		 */
		void add_edge(const ID_Type& last, const ID_Type& next) override {
			Node* last_node = nullptr;
			Node* next_node = nullptr;
			int last_index = -1;
			for (int i = 0; i < node_list.size(); ++i) {
				if (node_list[i]->id == last) {
					last_node = node_list[i];
					last_index = i;
				} else if (node_list[i]->id == next) {
					next_node = node_list[i];
				}
			}
			if (last_node && next_node) {
				adj_list[last_index].push_back(next_node);
			} else
				throw std::runtime_error("Invalid node IDs, cannot add edge");
		}

		/**
		 * DirectedGraph destructor which calls the base Graph class clear() method to free any allocated memory
		 * and clear the adjacent list and node list vectors.
		 * **Time Complexity** = *O(n)* where n is the number of nodes in the graph.
		 */
		virtual ~DirectedGraph() {
			if (node_num)
				clear();
		}

	private:
		using typename Graph<T, ID_Type>::Node;  /**< An alias used to easily access the Node structure in the base class. */
		using Graph<T, ID_Type>::adj_list;  /**< An alias used to easily access the adj_list member in the base class. */
		using Graph<T, ID_Type>::node_list;  /**< An alias used to easily access the node_list member in the base class. */
		using Graph<T, ID_Type>::node_num;  /**< An alias used to easily access the node_num member in the base class. */
		using Graph<T, ID_Type>::clear;  /**< An alias used to easily access the clear() method in the base class. */
	};
}// namespace custom

#endif// GRAPH_H