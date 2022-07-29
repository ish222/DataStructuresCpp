CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT(DATA_STRUCTURES_CPP)

SET(CMAKE_CXX_STANDARD 14)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")
add_compile_definitions("RELEASE=$<CONFIG:Release>")
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp BinarySearchTree.h BinaryTree.h Graph.h LinkedList.h Map.h Queue.h Stack.h Tree.h Vector.h SortingAlgorithms.h)