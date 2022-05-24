#include <iostream>
#include <vector>
#include <stdexcept>

#include "LinkedList.h"

int main() {
	LinkedList list;
	list.append(2);
	list.append(3);
	list.append(5);
	list.append(15);
	list.append(45);
	list.print_contents();
	std::cout << list.length() << std::endl;
	list.erase(2);
	list.print_contents();
	std::cout << list[0] <<"\n";

	LinkedList list2;
	list2.append(190);
	list2.append(290);
	list2.append(390);
	list2.print_contents();

	LinkedList list3 = list + list2;
	list3.print_contents();

}