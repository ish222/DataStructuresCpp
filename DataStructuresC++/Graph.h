#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

namespace custom {
    template<typename T>
    class Graph {
    public:
        Graph() : adj_list({}), node_list({}), node_num(0), directed(true) {}

        Graph(const int& data, bool directed = true) {
            Node* new_node = new Node(data, 'A');
            node_num = 1;
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
            this->directed = directed;
        }

        Graph(const Graph& other) {
            directed = other.directed;
            node_list = {};
            adj_list = {};
            node_num = 0;
            for (Node*& node: other.node_list)
                add_node(node->data);
            for (std::vector<Node*> link: other.adj_list) {
                char first = link[0]->id;
                for (int i = 1; i < link.size(); ++i)
                    add_edge(first, link[i]->id);
            }
        }

        Graph& operator=(const Graph& other) {
            if (this != &other) {
                if (node_num)
                    clear();
                directed = other.directed;
                node_list = {};
                adj_list = {};
                node_num = 0;
                for (Node*& node: other.node_list)
                    add_node(node->data);
                for (std::vector<Node*> link: other.adj_list) {
                    char first = link[0]->id;
                    for (int i = 1; i < link.size(); ++i)
                        add_edge(first, link[i]->id);
                }
            }
            return *this;
        }

        Graph(Graph&& other) noexcept {
            directed = other.directed;
            node_list = other.node_list;
            adj_list = other.adj_list;
            node_num = other.node_num;
            other.node_num = 0;
            other.node_list.clear();
            other.adj_list.clear();
        }

        Graph& operator=(Graph&& other) noexcept {
            if (this != &other) {
                if (node_num)
                    clear();
                directed = other.directed;
                node_list = other.node_list;
                adj_list = other.adj_list;
                node_num = other.node_num;
                other.node_num = 0;
                other.node_list.clear();
                other.adj_list.clear();
            }
            return *this;
        }

        void add_node(const int& data) {
            Node* new_node = new Node(data, 'A' + node_num);
            ++node_num;
            node_list.push_back(new_node);
            adj_list.push_back(std::vector<Node*>{new_node});
        }

        void add_edge(const char& last, const char& next) {
            Node* last_node = nullptr;
            Node* next_node = nullptr;
            for (Node* node: node_list) {
                if (node->id == last)
                    last_node = node;
                else if (node->id == next)
                    next_node = node;
            }
            if (last_node && next_node) {
                adj_list[(int) (last - 'A')].push_back(next_node);
                if (!directed) {
                    adj_list[(int) (next - 'A')].push_back(last_node);
                }
            }
            else throw std::runtime_error("Invalid node IDs");
        }

        void change(const char& id, const T& data) {
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

        operator bool() const noexcept {
            return (bool) node_num;
        }

        bool find(const char& id) const {
            for (Node*& node: node_list) {
                if (node->id == id)
                    return true;
            }
            return false;
        }

        bool find_edge(const char& last, const char& next) const {
            int last_index = (int) last - 65;
            for (int i = 0; i < adj_list[last_index].size(); i++) {
                if (adj_list[last_index][i]->id == next)
                    return true;
            }
            return false;
        }

        std::vector<int> contents() const {
            if (node_num) {
                std::vector<int> contents = {};
                for (Node*& node: node_list) {
                    contents.push_back(node->data);
                }
                return contents;
            }
            throw std::runtime_error("Graph is empty, there are no contents");
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

        void remove(const char& id) {
            if (node_num) {
                Node* node = nullptr;
                for (int i = 0; i < node_list.size(); i++) {
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

        ~Graph() {
            if (node_num)
                clear();
        }

    private:
        struct Node {
            T data;
            char id;

            Node(T data, char id) : data(data), id(id) {}
        };

        std::vector<std::vector<Node*>> adj_list;
        std::vector<Node*> node_list;
        size_t node_num;
        bool directed;
    };
}

#endif // GRAPH_H