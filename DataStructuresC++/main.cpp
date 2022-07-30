#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "BinaryTree.h"
#include "Graph.h"
#include "Map.h"
#include "BinarySearchTree.h"
#include "Tree.h"
#include "Vector.h"
#include "SortingAlgorithms.h"

template<typename T>
void printvec(std::vector<T> data) {
    for (T& i: data) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    try {
        LinkedList<float> list(12.31f);
        list.append(3.13f);
        list.append(5.87f);
        list.append(65.55f);
        list.append(1.90f);
        Algorithm::insertion_sort(list, [](float x, float y) { return x < y; });
        list.display();
        std::cout << list.length() << std::endl;
        list.erase(2);
        list.display();
        std::cout << list[0] << "\n\n";

        auto it = list.begin();
        std::cout << *it++ << "\n";
        std::cout << *it++ << "\n";
        std::cout << *it << "\n";
        std::cout << "\n\n";

        list.reverse_order();
        list.display();
        std::cout << "\n";

        LinkedList<float> list2;
        list2.append(1.90f);
        list2.append(2.90f);
        list2.append(3.90f);
        list2.display();
        std::cout << "\n";

        LinkedList<float> list3 = list + list2;
        list3.display();
        std::cout << "\n\n";

        LinkedList<float> list4;
        list4 = list3;
        list4.display();
        std::cout << "\n\n";

        Queue<std::string> queue;
        queue.enqueue("Hello ");
        queue.enqueue("my ");
        queue.enqueue("name ");
        queue.enqueue("is ");
        std::cout << "Peeking Queue: " << queue.peek() << "\n";
        queue.enqueue("Bart");
        queue.dequeue();
        std::cout << "length = " << queue.length() << "\n";
        std::cout << "Peeking Queue: " << queue.peek() << "\n";
        queue.display();
        std::cout << queue.contains("is ") << "\n";
        std::cout << queue.length() << "\n\n";

        Stack<char> stack;
        stack.push('a');
        stack.push('n');
        stack.push('d');
        stack.push('k');
        stack.display();
        stack.pop();
        std::cout << stack.peek() << "\n";
        std::cout << stack.length() << "\n\n";

        BT::BinaryTree<int> Btree = BT::BinaryTree<int>(5);
        Btree.new_left(10);
        Btree.new_right(15);
        Btree.advance_left();
        Btree.new_left(20);
        Btree.new_right(25);
        Btree.goto_root();
        Btree.advance_right();
        Btree.new_left(30);
        Btree.new_right(35);
        std::vector<int> data = Btree.contents_InOrder();
        printvec(data);
        data = Btree.contents_PostOrder();
        printvec(data);
        data = Btree.contents_PreOrder();
        printvec(data);
        std::cout << Btree.max_height() << std::endl;

        Btree.clear();
        Btree.change_data(10);
        std::cout << "\nTree height after clearing: " << Btree.max_height();
        std::cout << "\n\n";

        GP::Graph<int> graph(5);
        graph.add_node(10);
        graph.add_node(15);
        graph.add_node(20);
        graph.add_node(30);
        graph.add_edge('A', 'B');
        graph.add_edge('B', 'C');
        graph.add_edge('A', 'D');
        graph.add_edge('A', 'C');
        graph.add_edge('C', 'D');
        graph.add_edge('B', 'D');
        graph.add_edge('A', 'E');
        graph.add_edge('E', 'B');
        graph.print();
        std::cout << "\n";
        std::cout << graph.find_edge('C', 'A');
        std::cout << "\n";
        graph.remove('C');
        graph.print();
        std::cout << "\n\n";

        Queue<int> PQueue(3, 1);
        PQueue.enqueue(15);
        PQueue.enqueue(12);
        PQueue.enqueue(20);
        PQueue.enqueue(7);
        PQueue.display();
        std::cout << "\n\n";

        Map::Map<int, const char*> map;
        map.add("A", 15);
        map.add("B", 25);
        map.add("C", 6);
        map.add("D", 53);
        map.add("E", 152);
        map["F"] = 196;
        map.print();
        std::cout << "\n\n";

        BST::BinarySearchTree<double> bst(2.5);
        bst.add(1.9);
        bst.add(5.6);
        bst.add(8.3);
        bst.add(1.2);
        bst.add(12.9);
        bst.remove(5.6);
        std::vector<double> res = bst.contents_InOrder();
        printvec(res);
        std::cout << "\n\n";


        Tree::Tree<char> tree('A', true);
        tree.add_child('C');
        tree.add_child('B');
        tree.add_child('G');
        tree.add_child('M');
        tree.goto_child(tree.find_child('G'));
        tree.add_child('K');
        tree.add_child('P');
        tree.goto_root();
        tree.goto_child(tree.find_child('B'));
        tree.add_child('L');
        tree.add_child('Z');
        tree.goto_root();
        tree.goto_child(tree.find_child('M'));
        tree.add_child('X');
        tree.add_child('Q');
        tree.goto_child(tree.find_child('X'));
        tree.add_child('F');
        tree.add_child('O');
        std::cout << "Max tree height: " << tree.max_height() << std::endl;
        std::vector<char> t_res = tree.contents_InOrder();
        printvec(t_res);
        std::cout << "\n\n";

        Vector::Vector<std::vector<int>> vector(3);
        vector.push_back({2, 3, 4});
        vector.push_back({4});
        vector.push_back({6, 9});
        for (size_t i = 0; i < vector.size(); ++i) {
            printvec(vector[i]);
        }
        //Vector::Vector<int> vector;
        //vector.push_back(2);
        //vector.push_back(4);
        //vector.push_back(8);
        //vector.push_back(9);
        //for (int i = 0; i < vector.size(); ++i)
        //	std::cout << vector[i] << "\t";
        std::cout << "\n\n";

    }

    catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}