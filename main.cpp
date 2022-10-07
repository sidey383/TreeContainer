#include "tree.h"
#include <assert.h>

int main() {
    Tree<int> general_tree = Tree<int>(1);
    auto it = general_tree.begin();
    it.addLeft(2);
    it.addRight(9);
    ++it;
    assert(it.getValue() == 2);
    assert(!it.isEnd());
    it.addLeft(3);
    it.addRight(5);
    ++it;
    assert(it.getValue() == 3);
    assert(!it.isEnd());
    it.addLeft(4);
    ++it;
    assert(it.getValue() == 4);
    assert(!it.isEnd());
    ++it;
    assert(it.getValue() == 5);
    assert(!it.isEnd());
    it.addRight(6);
    ++it;
    assert(it.getValue() == 6);
    assert(!it.isEnd());
    it.addLeft(7);
    it.addRight(8);
    ++it;
    assert(it.getValue() == 7);
    assert(!it.isEnd());
    ++it;
    assert(it.getValue() == 8);
    assert(!it.isEnd());
    ++it;
    assert(it.getValue() == 9);
    assert(!it.isEnd());
    it.addLeft(10);
    it.addRight(12);
    ++it;
    assert(it.getValue() == 10);
    assert(!it.isEnd());
    it.addRight(11);
    ++it;
    assert(it.getValue() == 11);
    assert(!it.isEnd());
    ++it;
    assert(it.getValue() == 12);
    assert(!it.isEnd());
    it.addLeft(13);
    it.addRight(14);
}