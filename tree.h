#pragma once

template<class T>
class Tree {
public:
    class iterator;
    friend class iterator;

    ~Tree() {
        clear(tree);
    }

    explicit Tree(T value) {
        tree = new Node(value);
    }

    iterator begin() {
        return iterator(tree);
    }

    iterator end() {
        return iterator(tree, true);
    }

    int operator[] (const int a) {
        iterator it = iterator(tree); //  iterator it = begin() ?
        for(int i = 0; i < a; i++) {
            ++it;
        }
        if(it.isEnd()) {
            throw std::out_of_range("out of range");
        }
        return it.getValue();
    }

private:

    class Node {
    public:
        Node(T value, Node* top, Node* right, Node* left) : value(value), top(top), right(right), left(left) {}
        explicit Node(T value) : value(value), top(nullptr), right(nullptr), left(nullptr) {}
        Node* left;
        Node* right;
        Node* top;
        T value;
    };

    Node* tree = nullptr;

    void clear(Node* node) {
        if(node == nullptr)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    class iterator {
    private:
        Node* node = nullptr;
        bool end = false;

    public:

        explicit iterator(Node* node, bool isEnd = false)  {
            if(isEnd) {
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

        explicit iterator(iterator const * const it) {
            this->node = it->node;
            this->end = it->end;
        }

        T getValue() {
            return node->value;
        }

        bool next() {
            if(end) // if this is end return false
                return false;
            if(node->left != nullptr) { // step into tree
                node = node->left;
                return true;
            }
            if(node->right != nullptr) {
                node = node->right;
                return true;
            }

            Node* lastNode = nullptr;
            while((node->right == nullptr || node->right == lastNode) && node->top != nullptr) { // if this hanging vertex step up
                lastNode = node;
                node = node->top;
            }

            if(node->right == nullptr || node->right == lastNode) { // if this is ended
                while(node->left != nullptr || node->right != nullptr) { // go back
                    if(node->right != nullptr)
                        node = node->right;
                    else
                        node = node->left;
                }
                end = true;
                return true;
            } else {
                node = node->right;
            }
            return true;
        }

        bool back() {
            if (end) {
                end = false;
                return true;
            }

            if (node->top == nullptr) { // is begun
                return false;
            }

            if(node->top->left == node) { // if came from left node
                node = node->top;
                return true;
            }
            if(node->top->left == nullptr) { // if came from right node and not have left node
                node = node->top;
                return true;
            }
            Node* lastNode = node;
            node = node->top;
            while(true) { // or go down
                if(node->right != nullptr && node->right != lastNode) { // if right node exists and not last node
                    lastNode = node;
                    node = node->right;
                } else {
                    if (node->left != nullptr) {
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
            if(!next())
                throw std::out_of_range("out of range");
            return *this;
        }

        iterator& operator-- (){
            if(!back())
                throw std::out_of_range("out of range");
            return *this;
        }

        const iterator operator++ (int){
            iterator it(this);
            if(!next())
                throw std::out_of_range("out of range");
            return it;

        }

        const iterator operator-- (int){
            iterator it(this);
            if(!back())
                throw std::out_of_range("out of range");
            return it;
        }

        T operator* (){
            return this->getValue();
        }

    };

};

