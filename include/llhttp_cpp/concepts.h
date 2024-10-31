//
// Created by Renatus Madrigal on 10/29/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H

#include <functional>
#include <type_traits>

#include "llhttp.h"

namespace llhttp {
    template<typename T>
    concept Functor = requires(T t) {
        { &T::operator() };
    };

    namespace detail {
        template<typename T>
        struct FunctionTraitsHelper;

        template<typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret (*)(Args...)> {
            using Pointer = Ret (*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
            static constexpr auto ArgCount = sizeof...(Args);
        };

        template<typename F, typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret (F::*)(Args...)> {
            using Pointer = Ret (*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
            static constexpr auto ArgCount = sizeof...(Args);
        };

        template<typename F, typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret (F::*)(Args...) const> {
            using Pointer = Ret (*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
            static constexpr auto ArgCount = sizeof...(Args);
        };

        template<typename Ret, typename... Args>
        struct FunctionTraitsHelper<Ret(Args...)> {
            using Pointer = Ret (*)(Args...);
            using ReturnType = Ret;
            using FunctionType = Ret(Args...);
            static constexpr auto ArgCount = sizeof...(Args);
        };

        template<typename T>
            requires Functor<T>
        struct FunctionTraitsHelper<T> {
            using Pointer = typename FunctionTraitsHelper<decltype(&T::operator())>::Pointer;
            using ReturnType = typename FunctionTraitsHelper<Pointer>::ReturnType;
            using FunctionType = typename FunctionTraitsHelper<Pointer>::FunctionType;
            static constexpr auto ArgCount = FunctionTraitsHelper<Pointer>::ArgCount;
        };
    } // namespace detail

    template<typename T>
    concept Callable = Functor<T> || std::is_function_v<T> ||
                       (std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>);

    template<typename T>
        requires Callable<T>
    using FunctionTraits = detail::FunctionTraitsHelper<std::remove_all_extents_t<T>>;

    template<typename T>
        requires Callable<T>
    using FunctionPointer = typename FunctionTraits<T>::Pointer;

    template<typename T>
    static constexpr auto IsFunctionPointer = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>;

    template<typename T>
        requires Callable<T>
    static constexpr auto ConvertibleToFunctionPointer = std::is_convertible_v<T, FunctionPointer<T>>;

    namespace detail {
        template<typename T>
        struct ArgTypeTupleHelper;

        template<typename Ret, typename... Args>
        struct ArgTypeTupleHelper<Ret(Args...)> {
            using type = std::tuple<Args...>;
        };

        template<typename T, typename Func>
        struct PrependArgTypeHelper;

        template<typename T, typename Ret>
        struct PrependArgTypeHelper<T, Ret(void)> {
            using type = Ret(T);
        };

        template<typename T, typename Ret, typename... Args>
        struct PrependArgTypeHelper<T, Ret(Args...)> {
            using type = Ret(T, Args...);
        };
    } // namespace detail

    template<size_t N, typename F>
        requires Callable<F>
    using NthArgType = typename std::tuple_element_t<
            N, typename detail::ArgTypeTupleHelper<typename FunctionTraits<F>::FunctionType>::type>;

    template<typename F>
    concept Callback = Callable<F> && std::is_convertible_v<FunctionPointer<F>, llhttp_cb>;

    template<typename F>
    concept DataCallback = Callable<F> && std::is_convertible_v<FunctionPointer<F>, llhttp_data_cb>;

    template<typename F>
    concept CallbackWithoutParser =
            Callable<F> && std::is_convertible_v<FunctionPointer<typename detail::PrependArgTypeHelper<
                                                         llhttp_t *, typename FunctionTraits<F>::FunctionType>::type>,
                                                 llhttp_cb>;

    template<typename F>
    concept DataCallbackWithoutParser =
            Callable<F> && std::is_convertible_v<FunctionPointer<typename detail::PrependArgTypeHelper<
                                                         llhttp_t *, typename FunctionTraits<F>::FunctionType>::type>,
                                                 llhttp_data_cb>;
} // namespace llhttp

#endif // LLHTTP_CPP_INCLUDE_LLHTTP_CPP_CONCEPTS_H
