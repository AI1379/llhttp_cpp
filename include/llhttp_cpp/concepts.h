//
// Created by Renatus Madrigal on 10/29/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H

#include <functional>
#include <type_traits>

#include "concepts.h"

namespace llhttp {
    template<typename T>
    concept Functor = requires(T t)
    {
        { &T::operator() };
    };

    namespace detail {
        template<typename T>
        struct FunctionTraitsHelper;

        template<typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret(*)(Args...)> {
            using Pointer = Ret(*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
        };

        template<typename F, typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret(F::*)(Args...)> {
            using Pointer = Ret(*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
        };

        template<typename F, typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret(F::*)(Args...) const> {
            using Pointer = Ret(*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
        };

        template<typename T>
            requires Functor<T>
        struct FunctionTraitsHelper<T> {
            using Pointer = typename FunctionTraitsHelper<decltype(&T::operator())>::Pointer;
            using ReturnType = typename FunctionTraitsHelper<Pointer>::ReturnType;
            using FunctionType = typename FunctionTraitsHelper<Pointer>::FunctionType;
        };
    }

    template<typename T>
    concept Callable = Functor<T> || std::is_function_v<T> || (
                           std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T> >);

    template<typename T>
        requires Callable<T>
    using FunctionTraits = detail::FunctionTraitsHelper<std::remove_all_extents_t<T> >;

    template<typename T>
        requires Callable<T>
    using FunctionPointer = typename FunctionTraits<T>::Pointer;

    template<typename T>
    static constexpr auto IsFunctionPointer = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T> >;

    template<typename T>
        requires Callable<T>
    static constexpr auto ConvertibleToFunctionPointer = std::is_convertible_v<T, FunctionPointer<T> >;
}

#endif //LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H
