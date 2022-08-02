#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <deque>

namespace custom {
    template<typename T, typename ID_Type>
    class Graph {
    public:
        Graph() : adj_list({}), node_list({}), node_num(0) {}

        explicit Graph(const T& data, const ID_Type& id) : node_num(1) {
            Node* new_node = new Node(data, id);
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
        }

        explicit Graph(T&& data, ID_Type&& id) : node_num(1) {
            Node* new_node = new Node(std::move(data), std::move(id));
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
        }

        Graph(const Graph<T, ID_Type>& other) {
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

        Graph<T, ID_Type>& operator=(const Graph<T, ID_Type>& other) {
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

        Graph(Graph<T, ID_Type>&& other) noexcept {
            node_list = other.node_list;
            adj_list = other.adj_list;
            node_num = other.node_num;
            other.node_num = 0;
            other.node_list.clear();
            other.adj_list.clear();
        }

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

        void add_node(const T& data, const ID_Type& id) {
            Node* new_node = new Node(data, id);
            ++node_num;
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
        }

        void add_node(T&& data, ID_Type&& id) {
            Node* new_node = new Node(std::move(data), std::move(id));
            ++node_num;
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
        }

        virtual void add_edge(const ID_Type& last, const ID_Type& next) {
            Node* last_node = nullptr;
            Node* next_node = nullptr;
            int last_index;
            int next_index;
            for (int i = 0; i < node_list.size(); ++i) {
                if (node_list[i]->id == last) {
                    last_node = node_list[i];
                    last_index = i;
                }
                else if (node_list[i]->id == next) {
                    next_node = node_list[i];
                    next_index = i;
                }
            }
            if (last_node && next_node) {
                adj_list[last_index].push_back(next_node);
                adj_list[next_index].push_back(last_node);
            }
            else throw std::runtime_error("Invalid node IDs, cannot add edge");
        }

        void change(const ID_Type& id, const T& data) {
            for (Node*& node: node_list) {
                if (node->id == id) {
                    node->data = data;
                    return;
                }
            }
            throw std::runtime_error("Invalid node ID");
        }

        int size() const noexcept {
            return node_num;
        }

        bool empty() const noexcept {
            return node_num == 0;
        }

        explicit operator bool() const noexcept {
            return (bool) node_num;
        }

        bool find(const ID_Type& id) const {
            for (Node*& node: node_list) {
                if (node->id == id)
                    return true;
            }
            return false;
        }

        bool find_edge(const ID_Type& last, const ID_Type& next) const {
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

        std::vector<T> contents() const {
            if (node_num) {
                std::vector<T> contents = {};
                for (Node*& node: node_list) {
                    contents.push_back(node->data);
                }
                return contents;
            }
            throw std::runtime_error("Graph is empty, there are no contents");
        }

        std::vector<ID_Type> dfs(const ID_Type& id) const {
            std::vector<ID_Type> ret;
            std::unordered_map<Node*, bool> visited;
            int index = find_node_index(id);
            if (index == -1)
                throw std::invalid_argument("Node with id provided does not exist");
            dfs_helper(node_list[index], ret, visited);
            return ret;
        }

        std::vector<ID_Type> bfs(const ID_Type& id) const {
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

        void print() const {
            if (node_num) {
                for (std::vector<Node*> links: adj_list) {
                    for (Node*& node: links) {
                        std::cout << node->id << " : " << node->data << "\t->\t";
                    }
                    std::cout << "END\n";
                }
                std::cout << std::endl;
            }
            else throw std::runtime_error("Graph is empty, there is nothing to print");
        }

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
            }
            else throw std::runtime_error("Graph is empty, there is nothing to remove");
        }

        void clear() {
            if (node_num) {
                for (Node*& node: node_list) {
                    delete node;
                }
                node_list.clear();
                adj_list.clear();
                node_num = 0;
            }
            else throw std::runtime_error("Graph is empty, there is nothing to clear");
        }

        virtual ~Graph() {
            if (node_num)
                clear();
        }

    protected:
        struct Node {
            T data;
            ID_Type id;

            Node(const T& data, ID_Type id) : data(data), id(id) {}

            Node(T&& data, ID_Type&& id) : data(std::move(data)), id(std::move(id)) {}
        };

        std::vector<std::vector<Node*>> adj_list;
        std::vector<Node*> node_list;
        size_t node_num;

        int find_node_index(const ID_Type& id) const {
            for (int i = 0; i < node_list.size(); ++i) {
                if (node_list[i]->id == id)
                    return i;
            }
            return -1;
        }

        void dfs_helper(Node* node, std::vector<ID_Type>& vec, std::unordered_map<Node*, bool>& visited) const {
            visited[node] = true;
            vec.push_back(node->id);
            int node_index = find_node_index(node->id);
            for (Node* neighbour: adj_list[node_index]) {
                if (!visited[neighbour]) {
                    dfs_helper(neighbour, vec, visited);
                }
            }
        }
    };

    template<typename T, typename ID_Type>
    class DirectedGraph : public Graph<T, ID_Type> {
    public:
        DirectedGraph() : Graph<T, ID_Type>() {}

        explicit DirectedGraph(const T& data, const ID_Type& id) : Graph<T, ID_Type>(data, id) {}

        explicit DirectedGraph(T&& data, ID_Type&& id) : Graph<T, ID_Type>(std::move(data), std::move(id)) {}

        DirectedGraph(const DirectedGraph<T, ID_Type>& other) {
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

        DirectedGraph<T, ID_Type>& operator=(const DirectedGraph<T, ID_Type>& other) {
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

        DirectedGraph(DirectedGraph<T, ID_Type>&& other) noexcept: Graph<T, ID_Type>(std::move(other)) {}

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

        void add_edge(const ID_Type& last, const ID_Type& next) override {
            Node* last_node = nullptr;
            Node* next_node = nullptr;
            int last_index = -1;
            for (int i = 0; i < node_list.size(); ++i) {
                if (node_list[i]->id == last) {
                    last_node = node_list[i];
                    last_index = i;
                }
                else if (node_list[i]->id == next) {
                    next_node = node_list[i];
                }
            }
            if (last_node && next_node) {
                adj_list[last_index].push_back(next_node);
            }
            else throw std::runtime_error("Invalid node IDs, cannot add edge");
        }

        virtual ~DirectedGraph() {
            if (node_num)
                clear();
        }

    private:
        using typename Graph<T, ID_Type>::Node;
        using Graph<T, ID_Type>::adj_list;
        using Graph<T, ID_Type>::node_list;
        using Graph<T, ID_Type>::node_num;
        using Graph<T, ID_Type>::clear;
    };
}

#endif // GRAPH_H