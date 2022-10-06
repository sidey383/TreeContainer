#pragma once
#include <stack>
#include <stdexcept>

template<class T>
class Tree {
public:
    class iterator;
    friend class iterator;

    Tree() {}

    ~Tree() {
        clear(tree);
    }

    Tree(T value) {
        tree = new Node(value);
    }

    iterator begin() {
        return iterator(tree);
    }

    iterator end() {
        return iterator(tree, true);
    }

    bool operator[] (const int a) {
        iterator it = begin();
        for(int i = 0; i < a; i++) {
            if(!it.next()) {
                throw std::out_of_range("out of range");
            }
        }
        return it.getValue();

    }

private:

    class Node {
    public:
        Node(T value, Node* top, Node* right, Node* left) : value(value), top(top), right(right), left(left) {}
        Node(T value) : value(value), top(nullptr), right(nullptr), left(nullptr) {}
        Node* left;
        Node* right;
        Node* top;
        T value;
    };

    Node* tree = nullptr;

    void clear(Node* node) {
        if(node == nullptr)
            return;
        free(node->right);
        free(node->left);
    }

public:
    class iterator {
    private:
        Node* node;

    public:

        iterator(Node* node) : node(node) {}

        iterator(Node* node, bool isEnd) : node(node) {
            if(isEnd && node != nullptr) {
                while(node->left != nullptr || node->right != nullptr) {
                    if(node->left != nullptr)
                        node = node->left;
                    else
                        node = node->right;
                }
            }
        }

        iterator(iterator const& it) {
            this->node = it.node;
        }

        T getValue() {
            if(node == nullptr)
                throw std::range_error("tree is empty");
            return node->value;
        }

        bool next() {
            if(node->left != nullptr) {
                node = node->left;
                return true;
            }
            if(node->right != nullptr) {
                node = node->right;
                return true;
            }

            Node* lastNode = nullptr;
            while((node->right == nullptr || node->right == lastNode) && node->top != nullptr) {
                lastNode = node;
                node = node->top;
            }

            if(node->top == nullptr) {
                while(node->left != nullptr || node->right != nullptr) {
                    if(node->right != nullptr)
                        node = node->right;
                    else
                        node = node->left;
                }
                return false;
            } else {
                node = node->right;
            }
            return true;
        }

        bool back() {
            if(node->top == nullptr) {
                return false;
            }
            Node* lastNode = node;
            node = node->top;
            while((node->left != nullptr && node->left != lastNode) || (node->right != nullptr && node->right != lastNode && node->left != lastNode)) {
                lastNode = node;
                if(node->right != nullptr && node->right != lastNode && node->left != lastNode) {
                    node = node->right;
                } else {
                    node = node->left;
                }
            }
            return true;
        }

        bool operator== (iterator const &it){
            return it.node == this->node;
        }

        bool operator!= (iterator const &it){
            return it.node != this->node;
        }

        iterator& operator++ (){
            if(this->next()) {
                return *this;
            } else {
                throw std::out_of_range("out of range");
            }
        }

        iterator& operator-- (){
            if(this->back()) {
                return *this;
            } else {
                throw std::out_of_range("out of range");
            }
        }

        T operator* (){
            return this->getValue();
        }

    };

};

