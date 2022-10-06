#include <gtest/gtest.h>
#include "tree.h"

Tree<int> general_tree = Tree<int>(1);

TEST(BasicTest, WhileCycleTest) {
    Tree<int> tree = Tree<int>(2);
    int size = 0;
    auto iterator = tree.begin();
    do {
        EXPECT_EQ(iterator.getValue(), 2);
        size++;
    } while(iterator.next());
    EXPECT_EQ(size, 1);
}

TEST(BasicTest, ForCycleTest) {
    Tree<int> tree = Tree<int>(2);
    int size = 0;
    for(auto iterator = tree.begin(); iterator != tree.end(); ++iterator) {
        EXPECT_EQ(iterator.getValue(), 2) << "Wrong value in tree";
        size++;
    }
    EXPECT_EQ(size, 1);
}

TEST(GenaralTest, CreateTree) {
    auto it = general_tree.begin();
    it.addLeft(2);
    it.addRight(9);
    EXPECT_NO_THROW( ++it) << "move on  2";
    EXPECT_EQ(it.getValue(), 2);
    EXPECT_EQ(it.isEnd(), false);
    it.addLeft(3);
    it.addRight(5);
    EXPECT_NO_THROW(++it) << "move on  3";
    EXPECT_EQ(it.getValue(), 3);
    EXPECT_EQ(it.isEnd(), false);
    it.addLeft(4);
    EXPECT_NO_THROW(++it) << "move on  4";
    EXPECT_EQ(it.getValue(), 4);
    EXPECT_EQ(it.isEnd(), false);
    EXPECT_NO_THROW(++it) << "move on  5";
    EXPECT_EQ(it.getValue(), 5);
    EXPECT_EQ(it.isEnd(), false);
    it.addRight(6);
    EXPECT_NO_THROW(++it) << "move on  6";
    EXPECT_EQ(it.getValue(), 6);
    EXPECT_EQ(it.isEnd(), false);
    it.addLeft(7);
    it.addRight(8);
    EXPECT_NO_THROW(++it) << "move on  7";
    EXPECT_EQ(it.getValue(), 7);
    EXPECT_EQ(it.isEnd(), false);
    EXPECT_NO_THROW(++it) << "move on  8";
    EXPECT_EQ(it.getValue(), 8);
    EXPECT_EQ(it.isEnd(), false);
    EXPECT_NO_THROW(++it) << "move on  9";
    EXPECT_EQ(it.getValue(), 9);
    EXPECT_EQ(it.isEnd(), false);
    it.addLeft(10);
    it.addRight(12);
    EXPECT_NO_THROW(++it) << "move on  10";
    EXPECT_EQ(it.getValue(), 10);
    EXPECT_EQ(it.isEnd(), false);
    it.addRight(11);
    EXPECT_NO_THROW(++it) << "move on  11";
    EXPECT_EQ(it.getValue(), 11);
    EXPECT_EQ(it.isEnd(), false);
    EXPECT_NO_THROW(++it) << "move on  12";
    EXPECT_EQ(it.getValue(), 12);
    EXPECT_EQ(it.isEnd(), false);
    it.addLeft(13);
    it.addRight(14);
}

TEST(GenaralTest, IteratorTest) {
    int size = 0;
    for(auto iterator = general_tree.begin(); iterator != general_tree.end(); ++iterator) {
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "Wrong value in tree";
    }
}
