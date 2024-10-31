//
// Created by Renatus Madrigal on 10/29/2024.
//

#include <gtest/gtest.h>

#include "llhttp_cpp/functional.h"

// TODO: Check if these tests can be made static
TEST(FunctionalTest, ConceptTest) {
    constexpr auto lambda1 = []() {};
    if constexpr (std::is_same_v<void (*)(), llhttp::FunctionPointer<decltype(lambda1)>>) {
        EXPECT_TRUE(true);
    } else {
        EXPECT_TRUE(false);
    }
    constexpr auto lambda2 = [=](int a) -> bool { return a > 3; };
    if constexpr (std::is_same_v<bool (*)(int), llhttp::FunctionPointer<decltype(lambda2)>>) {
        EXPECT_TRUE(true);
    } else {
        EXPECT_TRUE(false);
    }
}

TEST(FunctionalTest, LambdaToFunctionPointerTest) {
    auto func = [](int a, int b) { return a + b; };
    auto func_ptr = llhttp::toFunctionPointer(func);
    EXPECT_EQ(func_ptr(1, 2), 3);
    auto func_ptr_ptr = llhttp::toFunctionPointer(func_ptr);
    EXPECT_EQ(func_ptr_ptr(1, 2), 3);
    auto x = 1;
    auto func2 = [&x](int a) { return x + a; };
    auto func2_ptr = llhttp::toFunctionPointer(func2);
    EXPECT_EQ(func2_ptr(2), 3);
    EXPECT_EQ(func2(2), 3);
    x = 2;
    EXPECT_EQ(func2_ptr(2), 4);
    auto func3_ptr = llhttp::toFunctionPointer([&x](int a) { return a - x; });
    EXPECT_EQ(func3_ptr(2), 0);
}

TEST(FunctionalTest, FunctorToFunctionPointerTest) {
    std::function<int(int, int)> func = [](int a, int b) { return a + b; };
    auto func_ptr = llhttp::toFunctionPointer(std::move(func));
    EXPECT_EQ(func_ptr(1, 2), 3);
    struct TestFunctor {
        int val;
        bool operator()(int a, int b) const { return a + b > val; };
    };
    auto functor_ptr = llhttp::toFunctionPointer(TestFunctor{3});
    EXPECT_EQ(functor_ptr(1, 3), true);
}
