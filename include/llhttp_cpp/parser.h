//
// Created by Renatus Madrigal on 10/30/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H

#include <cstdlib>
#include "llhttp.h"
#include "llhttp_cpp/common.h"

namespace llhttp {
    class Parser {
    public:
        Parser() noexcept {
            parser_ = static_cast<llhttp_t *>(malloc(sizeof(llhttp_t)));
            llhttp_init(parser_, HTTP_BOTH, nullptr);
        }

        explicit Parser(llhttp_type_t type) noexcept {
            parser_ = static_cast<llhttp_t *>(malloc(sizeof(llhttp_t)));
            llhttp_init(parser_, type, nullptr);
        }

        Parser(const Parser &other) noexcept {
            parser_ = static_cast<llhttp_t *>(malloc(sizeof(llhttp_t)));
            *(this->parser_) = *(other.parser_);
        }

        Parser(Parser &&other) noexcept {
            this->parser_ = other.parser_;
            other.parser_ = nullptr;
        }

        Parser &operator=(const Parser &other) noexcept {
            if (this == &other)
                return *this;
            if (!parser_)
                parser_ = static_cast<llhttp_t *>(malloc(sizeof(llhttp_t)));
            *(this->parser_) = *(other.parser_);
            return (*this);
        }

        Parser &operator=(Parser &&other) noexcept {
            this->parser_ = other.parser_;
            return *this;
        }

        ~Parser() { free(parser_); }

    private:
        llhttp_t *parser_;
    };
} // namespace llhttp

#endif // LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H
