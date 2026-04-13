#ifndef AIZO_PROJECT1_BINARYTREE_H
#define AIZO_PROJECT1_BINARYTREE_H

#include <cstddef>

template<typename T>
class BinaryTree {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        explicit Node(const T& v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* root_;
    std::size_t size_;

    Node* node_by_heap_index(std::size_t heap_index) const {
        if (root_ == nullptr || heap_index == 0) {
            return nullptr;
        }

        if (heap_index == 1) {
            return root_;
        }

        std::size_t mask = 1;
        while ((mask << 1) <= heap_index) {
            mask <<= 1;
        }

        mask >>= 1;

        Node* current = root_;
        while (mask > 0 && current != nullptr) {
            if (heap_index & mask) {
                current = current->right;
            } else {
                current = current->left;
            }
            mask >>= 1;
        }

        return current;
    }

    void clear_recursive(Node* node) {
        if (node == nullptr) {
            return;
        }

        clear_recursive(node->left);
        clear_recursive(node->right);
        delete node;
    }

public:
    BinaryTree() : root_(nullptr), size_(0) {}

    ~BinaryTree() {
        clear();
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        const std::size_t new_index = size_ + 1;

        if (new_index == 1) {
            root_ = node;
            size_ = 1;
            return;
        }

        Node* parent = node_by_heap_index(new_index / 2);

        if ((new_index % 2) == 0) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        ++size_;
    }

    void clear() {
        clear_recursive(root_);
        root_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const {
        return size_;
    }

    T& at(std::size_t index) {
        return node_by_heap_index(index + 1)->value;
    }

    const T& at(std::size_t index) const {
        return node_by_heap_index(index + 1)->value;
    }
};

#endif //AIZO_PROJECT1_BINARYTREE_H
