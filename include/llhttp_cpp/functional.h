//
// Created by Renatus Madrigal on 10/29/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_FUNCTIONAL_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_FUNCTIONAL_H

#include "llhttp_cpp/concepts.h"

namespace llhttp {
    namespace detail {
        // https://stackoverflow.com/questions/28746744/passing-capturing-lambda-as-function-pointer
        template<int, typename Func, typename Ret, typename... Args>
        auto functionPointerImpl(Func &&func, Ret (*)(Args...)) {
            static std::decay_t<Func> storage = std::forward<Func>(func);
            static bool used = false;
            if (used) {
                using type = decltype(storage);
                storage.~type();
                new(&storage) type(std::forward<Func>(func));
            }
            used = true;

            return +[](Args... args) -> Ret {
                auto &c = *std::launder(&storage);
                return Ret(c(std::forward<Args>(args)...));
            };
        }
    }

    template<typename Func, int UniqueId = 0>
        requires Callable<Func>
    FunctionPointer<Func> toFunctionPointer(Func func) {
        // TODO: check if rvalue reference is necessary here
        if constexpr (IsFunctionPointer<Func>) {
            return func;
        } else {
            return detail::functionPointerImpl<UniqueId>(std::forward<Func>(func), FunctionPointer<Func>(nullptr));
        }
    }
}

#endif //LLHTTP_CPP_INCLUDE_LLHTTP_CPP_FUNCTIONAL_H
