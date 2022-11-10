#pragma once
#include <stack>
#include <stdexcept>

template<class T>
class Tree {
public:
    class iterator;
    friend class iterator;

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

    int operator[] (const int a) {
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
        clear(node->right);
        clear(node->left);
        delete node;
    }

public:
    class iterator {
    private:
        Node* node = nullptr;
        bool end = false;

    public:

        explicit iterator(Node* node, bool isEnd = false)  {
            if(isEnd && node != nullptr) {
                while(node->left != nullptr || node->right != nullptr) {
                    if(node->right != nullptr)
                        node = node->right;
                    else
                        node = node->left;
                }
            }
            this->node = node;
            this->end = isEnd;
        }

        iterator(iterator const& it) {
            this->node = it.node;
            this->end = it.end;
        }

        iterator(iterator const * const it) {
            this->node = it->node;
            this->end = it->end;
        }

        T getValue() {
            return node->value;
        }

        bool next() {
            if(end)
                return false;
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

            if(node->right == nullptr || node->right == lastNode) {
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
            if (isEnd()) {
                end = false;
                return true;
            }

            if (node->top == nullptr) {
                while(true) {
                    if(node->right != nullptr) {
                        node = node->right;
                    } else {
                        if (node->left != nullptr) {
                            node = node->left;
                        } else {
                            break;
                        }
                    }
                }
                return true;
            }

            if(node->top->left == node) {
                node = node->top;
                return true;
            }
            if(node->top->right == node && node->top->left == nullptr) {
                node = node->top;
                return true;
            }
            Node* lastNode = node;
            node = node->top;
            while(true) {
                if(node->right != nullptr && node->right != lastNode) {
                    lastNode = node;
                    node = node->right;
                } else {
                    if (node->left != nullptr && node->left != lastNode) {
                        lastNode = node;
                        node = node->left;
                    } else {
                        break;
                    }
                }
            }
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

        iterator operator++ (int){
            if(isEnd())
                throw std::out_of_range("out of range");
            iterator it(this);
            next();
            return it;
        }

        iterator operator-- (int){
            iterator it(this);
            if(this->back()) {
                return it;
            } else {
                throw std::out_of_range("out of range");
            }
        }

        T operator* (){
            return this->getValue();
        }

    };

};

