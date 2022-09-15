#include <iostream>
#include <stack>

template <typename T>
class Tree_container {
public:
    class Iterator;
    friend class Iterator;
private:
    struct Node {
        Node* left;
        Node* right;
        T value;
    };

    Node* tree = nullptr;

    void clear(Node* node) {
        if(node == nullptr)
            return;
        clear(node->left);
        clear(node->right);
        free(node);
    }
public:

    Tree_container() {}

    ~Tree_container() {
        clear(tree);
    }

    class Iterator {
        std::stack<Node*> stack;

    public:

        T getValue() {
            if(stack.top() == nullptr)
                return nullptr;
            else
                return stack.top()->value;
        }

        Iterator next() {
            //TODO: realize next for iterator
            return this;
        }
    };

};

int main() {
    std::cout << "Hello, World!" << std::endl;
    Tree_container<int> tree;
    return 0;
}
