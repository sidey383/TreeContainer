#include <gtest/gtest.h>
#include "tree.h"

TEST(CreateTest, BasicCreate) {
    Tree<int> tree = Tree<int>(2);
    int size = 0;
    for(auto iterator = tree.begin(); iterator != tree.end(); iterator = ++iterator) {
        EXPECT_EQ(iterator.getValue(), 2);
        size++;
    }
    EXPECT_EQ(size, 1);
}
