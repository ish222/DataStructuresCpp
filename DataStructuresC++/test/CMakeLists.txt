project(Google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run LinkedLists_Tests.cpp DoublyLinkedList_Tests.cpp Queue_Tests.cpp Stack_Tests.cpp Vector_Tests.cpp Array_Tests.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)