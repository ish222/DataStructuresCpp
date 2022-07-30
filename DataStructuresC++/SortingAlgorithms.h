#pragma once

#include "LinkedList.h"

namespace Algorithm {
    template<typename T>
    inline bool ascending(T a, T b) { return a > b; }

    template<typename ListType>
    void bubble_sort(ListType& list,
                     bool (* comparison)(typename ListType::ValueType, typename ListType::ValueType) = &ascending) {
        size_t list_size = list.length();
        for (size_t i = 0; i < list_size - 1; ++i) {
            for (size_t j = 0; j < list_size - i - 1; ++j) {
                if (comparison(list[j], list[j + 1])) {
                    auto temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }
    }

    template<typename ListType>
    void selection_sort(ListType& list,
                        bool (* comparison)(typename ListType::ValueType, typename ListType::ValueType) = &ascending) {
        size_t list_size = list.length();
        for (size_t i = 0; i < list_size - 1; ++i) {
            size_t min = i;
            for (size_t j = i + 1; j < list_size; ++j) {
                if (comparison(list[min], list[j]))
                    min = j;
            }
            if (min != i) {
                auto temp = list[i];
                list[i] = list[min];
                list[min] = temp;
            }
        }
    }

    template<typename ListType>
    void insertion_sort(ListType& list,
                        bool (* comparison)(typename ListType::ValueType, typename ListType::ValueType) = &ascending) {
        int list_size = static_cast<int>(list.length());
        for (int i = 1; i < list_size; ++i) {
            auto temp = list[i];
            int j = i - 1;
            while (j >= 0 && comparison(list[j], temp)) {
                list[j + 1] = list[j];
                --j;
            }
            list[j + 1] = temp;
        }
    }

    template<typename ListType>
    void merge_sort(ListType& list,
                    bool (* comparison)(typename ListType::ValueType, typename ListType::ValueType) = &ascending) {

    }
}
