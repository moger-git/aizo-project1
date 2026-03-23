#include <filesystem>
#include <iostream>
#include "include/structures/DynamicArray.h"
#include "include/structures/DoublyLinkedList.h"
#include "include/io/FileLoader.h"
#include "include/structures/SinglyLinkedList.h"

int main() {
    std::cout << "Current path: " << std::filesystem::current_path() << "\n";
    DynamicArray<int> data;
    // SinglyLinkedList<int> data;
    // DoublyLinkedList<int> data;

    const std::string path = "../data/data.txt";
    bool ok = FileLoader<int, DynamicArray<int>>::load(path, data);

    if (!ok) {
        std::cout << "Error: cannot load file: " << path << "\n";
        return 1;
    }


    std::cout << "Loaded " << data.size() << " elements:\n";
    data.print();

    return 0;
}
