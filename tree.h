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
        bool end = false;
    public:

        iterator(Node* node) : node(node), end(false) {}

        iterator(Node* node, bool isEnd) : node(node), end(isEnd)  {
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
            if(node == nullptr)
                throw std::range_error("tree is empty");
            if(end)
                return !end;
            if(node->left != nullptr) {
                node = node->left;
                end = false;
                return !end;
            }
            if(node->right != nullptr) {
                node = node->right;
                return !end;
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
                end = true;
                return !end;
            } else {
                node = node->right;
            }
            return !end;
        }

        bool back() {
            if(node == nullptr)
                throw std::range_error("tree is empty");
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
            end = false;
            return true;
        }

        bool isEnd() {
            return end;
        }

        void setValue(T value) {
            node->value = value;
        }

        void addLeft(T value) {
            Node* newNode = new Node(value, node, nullptr, node->left);
            if(newNode->left != nullptr)
                newNode->left->top = newNode;
            node->left = newNode;
        }

        void addRight(T value) {
            Node* newNode = new Node(value, node, node->right, nullptr);
            if(newNode->right != nullptr)
                newNode->right->top = newNode;
            node->right = newNode;
        }

        bool operator== (iterator const &it){
            return it.node == this->node && it.end == this->end;
        }

        bool operator!= (iterator const &it){
            return it.node != this->node || it.end != this->end;
        }

        iterator& operator++ (){
            if(isEnd())
                throw std::out_of_range("out of range");
            next();
            return *this;
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

