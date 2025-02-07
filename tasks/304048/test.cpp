#include <gtest/gtest.h>
#include "solution.cpp"

TEST(StackTest, MaxSizeTest)
{
    IntStack stack(5);

    stack.push(1);
    stack.push(2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    stack.push(4);
    stack.push(5);
    EXPECT_THROW(stack.push(6), std::runtime_error);
}
TEST(StackTest, PopEmptyStackTest)
{
    IntStack stack(5);
    EXPECT_THROW(stack.pop(), std::runtime_error);
}
TEST(StackTest, TopEmptyStackTest)
{
    IntStack stack(10);
    EXPECT_THROW(stack.top(), std::runtime_error);
}
TEST(StackTest, PositiveStackTest)
{
    IntStack stack(5);

    stack.push(1);
    stack.push(10);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    std::vector<int> result = stack.monotonicIncreasing();
    std::vector<int> expected = {1, 2, 3, 4};
    EXPECT_EQ(result, expected);
}
TEST(StackTest, NegativeStackTest)
{
    IntStack stack(5);

    stack.push(-1);
    stack.push(-10);
    stack.push(-4);
    stack.push(-3);
    stack.push(-2);
    std::vector<int> result = stack.monotonicIncreasing();
    std::vector<int> expected = {-10, -4, -3, -2};
    EXPECT_EQ(result, expected);
}
TEST(StackTest, MixedStackTest)
{
    IntStack stack(5);

    stack.push(1);
    stack.push(-10);
    stack.push(2);
    stack.push(3);
    stack.push(-4);
    std::vector<int> result = stack.monotonicIncreasing();
    std::vector<int> expected = {-10, -4};
    EXPECT_EQ(result, expected);
}
TEST(StackTest, NegativeMaxSizeTest)
{
    EXPECT_THROW(IntStack stack(-1), std::runtime_error);
}
TEST(StackTest, EmptyStackTest)
{
    IntStack stack(5);
    std::vector<int> result = stack.monotonicIncreasing();
    std::vector<int> expected;
    EXPECT_EQ(result, expected);
}