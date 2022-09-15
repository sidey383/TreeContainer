#include "Tree.h"
#include <stack>

Tree::Tree() {}
Tree::~Tree() {
    clear(tree);
}

T Tree::TreeIterator::getValue() {
    if(stack.top() == nullptr)
        return nullptr;
    else
        return stack.top()->value;
}