#pragma once
#include <stack>

template <typename T>
class Tree {
public:
    class TreeIterator;
    friend class TreeIterator;
    Tree();
    ~Tree();
private:
    struct Node {
        Node* left;
        Node* right;
        T value;
    };
    Node* tree = nullptr;
    void clear(Node* node);
public:
    class TreeIterator {
    private:
        std::stack<Node*> stack;
    };
    T getValue();
    TreeIterator next();
};

