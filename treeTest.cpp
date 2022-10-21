#include <gtest/gtest.h>
#include <crtdbg.h>
#include "tree.h"

class MemoryLeakDetector {
public:
    MemoryLeakDetector() {
        _CrtMemCheckpoint(&memState_);
    }

    ~MemoryLeakDetector() {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
            reportFailure(stateDiff);
    }
private:
    void reportFailure(_CrtMemState stateDiff) {
        FAIL() << "Memory leak \n"
        << stateDiff.lSizes[0] << " bytes in " <<stateDiff.lCounts[0] << " Free Blocks." << std::endl
        << stateDiff.lSizes[1] << " bytes in " <<stateDiff.lCounts[1] << " Normal Blocks." << std::endl
        << stateDiff.lSizes[2] << " bytes in " <<stateDiff.lCounts[2] << " CRT Blocks." << std::endl
        << stateDiff.lSizes[3] << " bytes in " <<stateDiff.lCounts[4] << " Ignore Blocks." << std::endl
        << stateDiff.lSizes[4] << " bytes in " <<stateDiff.lCounts[4] << " Client Blocks." << std::endl
        << "Largest number used: " << stateDiff.lHighWaterCount << " bytes." << std::endl
        << "Total allocations: " << stateDiff.lTotalCount << " bytes." << std::endl;
    }
    _CrtMemState memState_;
};

class TestWithMemoryLeakDetector : public ::testing::Test {
protected:

    MemoryLeakDetector leakDetector;

};

class BasicTest : public TestWithMemoryLeakDetector {
protected:

    Tree<int> tree = Tree<int>(2);
};

TEST_F(BasicTest, WhileCycleTest) {
    int size = 0;
    auto iterator = tree.begin();
    do {
        EXPECT_EQ(iterator.getValue(), 2);
        size++;
    } while (iterator.next());
    EXPECT_EQ(size, 1);
}

TEST_F(BasicTest, ForCycleTest)  {
    int size = 0;
    for(auto iterator = tree.begin(); iterator != tree.end(); ++iterator) {
        EXPECT_EQ(iterator.getValue(), 2) << "Wrong value in tree";
        size++;
    }
    EXPECT_EQ(size, 1);
}

class GeneralTest1 : public TestWithMemoryLeakDetector {
protected:

    void SetUp() override {
        auto it = tree.begin();
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

    Tree<int> tree = Tree<int>(1);
};

TEST_F(GeneralTest1, ForItearation) {
    int size = 0;
    for(auto iterator = tree.begin(); iterator != tree.end(); ++iterator) {
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        EXPECT_EQ(iterator.getValue(), *iterator);
    }
}

TEST_F(GeneralTest1, ForEachItearation) {
    MemoryLeakDetector leakDetector;
    int size = 0;
    for(int a : tree) {
        size++;
        EXPECT_EQ(a, size);
    }
    EXPECT_EQ(size, 14);
}

TEST_F(GeneralTest1, IndexingTest) {
    MemoryLeakDetector leakDetector;
    for(int i = 0; i < 14; i++) {
        EXPECT_NO_THROW(tree[i]) << "index: " << i;
        EXPECT_EQ(tree[i], i + 1) << "index: " << i;
    }
}

TEST_F(GeneralTest1, DecrementTest1) {
    MemoryLeakDetector leakDetector;
    Tree<int>::iterator iterator = tree.begin();
    int size = 0;
    while(true) {
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        EXPECT_NO_THROW(++iterator);
        if(iterator == tree.end())
            break;
        EXPECT_EQ(iterator.getValue(), size + 1) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        EXPECT_NO_THROW(--iterator);
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        EXPECT_NO_THROW(++iterator);
        EXPECT_EQ(iterator.getValue(), size + 1) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size >= 15)
            FAIL();
    }
}

TEST_F(GeneralTest1, DecremetTest2){
    MemoryLeakDetector leakDetector;
    int size = 15;
    auto iterator = tree.end();
    do {
        EXPECT_NO_THROW(--iterator);
        size--;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size < 1 )
            FAIL();
    } while(iterator != tree.begin());
}

TEST_F(GeneralTest1, ForwardAndBackward){
    MemoryLeakDetector leakDetector;
    Tree<int>::iterator iterator;
    int size = 0;
    for(iterator = tree.begin(); iterator != tree.end(); ++iterator) {
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size >= 15)
            FAIL();
    }
    do {
        EXPECT_NO_THROW(--iterator);
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size < 1)
            FAIL();
        size--;
    } while (iterator != tree.begin());

}

TEST_F(GeneralTest1, RotateValueTest1) {
    MemoryLeakDetector leakDetector;
    int size = 0;
    for(auto it = tree.begin(); it != tree.end(); ++it) {
        size++;
        EXPECT_EQ(it.getValue(), size) << "iterator node: " << *it << " isEnd: " << it.isEnd() << std::endl;
        EXPECT_NO_THROW(it.setValue(15 - it.getValue()));
        if(size >= 15)
            FAIL();
    }

    size = 0;
    auto iterator = tree.end();
    do {
        EXPECT_NO_THROW(--iterator);
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size < 1)
            FAIL();
    } while(iterator != tree.begin());
}

TEST_F(GeneralTest1, RotateValueTest2) {
    MemoryLeakDetector leakDetector;
    auto bIterator = tree.begin();
    auto eIterator = tree.end();
    EXPECT_EQ(eIterator == bIterator, false);
    EXPECT_NO_THROW(--eIterator);
    while(true) {
        int v1, v2;
        EXPECT_NO_THROW(v1 = bIterator.getValue());
        EXPECT_NO_THROW(v2 = eIterator.getValue());
        EXPECT_NO_THROW(eIterator.setValue(v1));
        EXPECT_NO_THROW(bIterator.setValue(v2));
        EXPECT_NO_THROW(--eIterator);
        if(eIterator == bIterator) {
            break;
        }
        EXPECT_NO_THROW(++bIterator);
        if(eIterator == bIterator) {
            break;
        }
    }
    int size = 0;
    auto iterator = tree.end();
    do {
        EXPECT_NO_THROW(--iterator);
        size++;
        EXPECT_EQ(iterator.getValue(), size) << "iterator node: " << *iterator << " isEnd: " << iterator.isEnd() << std::endl;
        if(size < 1)
            FAIL();
    } while(iterator != tree.begin());
}

TEST_F(GeneralTest1, PostfixOperatorsTest) {
    auto iterator = tree.begin();
    int i = 0;
    while(iterator != tree.end()) {
        i++;
        Tree<int>::iterator postfixIterator;
        EXPECT_NO_THROW(postfixIterator = iterator++);
        EXPECT_EQ(*postfixIterator, i);
        if(iterator.isEnd())
            EXPECT_EQ(*iterator, i);
        else
            EXPECT_EQ(*iterator, i+1);
    }
    i = 14;
    iterator = tree.end();
    EXPECT_EQ(*iterator, i);
    EXPECT_EQ(iterator.isEnd(), true);
    Tree<int>::iterator postfixIterator;
    EXPECT_NO_THROW(postfixIterator = iterator--);
    EXPECT_EQ(*postfixIterator, i);
    EXPECT_EQ(postfixIterator.isEnd(), true);
    EXPECT_EQ(*iterator, i);
    EXPECT_EQ(iterator.isEnd(), false);
    while(iterator != tree.begin()) {
        EXPECT_NO_THROW(postfixIterator = iterator--);
        EXPECT_EQ(*postfixIterator, i);
        EXPECT_EQ(postfixIterator.isEnd(), false);
        EXPECT_EQ(*iterator, i-1);
        EXPECT_EQ(iterator.isEnd(), false);
        i--;
    }
}

class VoidTreeTest : public TestWithMemoryLeakDetector {
protected:

    Tree<int> tree = Tree<int>();
};

TEST_F(VoidTreeTest, IteratorExceptions) {
    auto iter = tree.begin();
    EXPECT_ANY_THROW(iter.getValue());
    EXPECT_ANY_THROW(*iter);
    EXPECT_ANY_THROW(iter++);
    EXPECT_ANY_THROW(iter--);
    EXPECT_ANY_THROW(++iter);
    EXPECT_ANY_THROW(--iter);
}
