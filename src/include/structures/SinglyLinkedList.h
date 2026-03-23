#ifndef AIZO_PROJECT1_SINGLYLINKEDLIST_H
#define AIZO_PROJECT1_SINGLYLINKEDLIST_H

#include <cstddef>
#include <iostream>

template<typename T>
class SinglyLinkedList {
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

    Node* node_at(std::size_t index) const {
        Node* current = head_;

        for (std::size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current;
    }

public:
    SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node* node = new Node(value);

        if (tail_ == nullptr) {
            head_ = node;
            tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }

        ++size_;
    }

    void clear() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }

        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    T& at(std::size_t index) {
        return node_at(index)->value;
    }

    const T& at(std::size_t index) const {
        return node_at(index)->value;
    }

    void print() const {
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << "\n";
    }
};

#endif //AIZO_PROJECT1_SINGLYLINKEDLIST_H