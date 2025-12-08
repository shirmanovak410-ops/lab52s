//
// Created by Ксения on 09.10.2025.
//
#include <iostream>
#include <vector>
#include "lab3.h"
int main() {
    std::vector<int> arr = {1, 2, -3, 4, 5, 6, -7, 8, 9, 10};
    int removed_element=0;
    std::cout << remove_first_negative_element(arr, removed_element) << " " << removed_element;
    return 0;
}
bool remove_first_negative_element(std::vector<int>& vec, int& removed_element) {
    for (auto i = vec.begin(); i != vec.end(); i++) {
        if (*i < 0) {
            removed_element = *i;
            i = vec.erase(i);
            return true;
        }
    }
    return false;
}