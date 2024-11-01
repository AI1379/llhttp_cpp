//
// Created by Renatus Madrigal on 11/1/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_COMMON_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_COMMON_H

#include <string_view>
#include <type_traits>
#include "llhttp.h"

namespace llhttp {
#define LLHTTP_CPP_REGISTER_ENUM(enum)                                                                                 \
    case enum:                                                                                                         \
        return #enum

    template<typename Enum>
        requires std::is_enum_v<Enum>
    constexpr std::string_view enumToString(Enum value) {
        if constexpr (std::is_convertible_v<Enum, llhttp_errno_t>) {
            switch (value) {
#define LLHTTP_CPP_REGISTER(num, name, name2) LLHTTP_CPP_REGISTER_ENUM(HPE_##name);
                HTTP_ERRNO_MAP(LLHTTP_CPP_REGISTER)
#undef LLHTTP_CPP_REGISTER
                // LLHTTP_CPP_REGISTER_ENUM(HPE_OK);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INTERNAL);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_STRICT);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CR_EXPECTED);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_LF_EXPECTED);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_UNEXPECTED_CONTENT_LENGTH);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_UNEXPECTED_SPACE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CLOSED_CONNECTION);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_METHOD);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_URL);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_CONSTANT);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_VERSION);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_HEADER_TOKEN);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_CONTENT_LENGTH);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_CHUNK_SIZE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_STATUS);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_EOF_STATE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_INVALID_TRANSFER_ENCODING);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_MESSAGE_BEGIN);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_HEADERS_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_MESSAGE_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_CHUNK_HEADER);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_CHUNK_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_PAUSED);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_PAUSED_UPGRADE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_PAUSED_H2_UPGRADE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_USER);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_URL_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_STATUS_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_METHOD_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_VERSION_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_HEADER_FIELD_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_HEADER_VALUE_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_CHUNK_EXTENSION_NAME_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_CHUNK_EXTENSION_VALUE_COMPLETE);
                // LLHTTP_CPP_REGISTER_ENUM(HPE_CB_RESET);
                default:
                    return "Unknown llhttp_errno";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_flags_t>) {
            switch (value) {
                LLHTTP_CPP_REGISTER_ENUM(F_CONNECTION_KEEP_ALIVE);
                LLHTTP_CPP_REGISTER_ENUM(F_CONNECTION_CLOSE);
                LLHTTP_CPP_REGISTER_ENUM(F_CONNECTION_UPGRADE);
                LLHTTP_CPP_REGISTER_ENUM(F_CHUNKED);
                LLHTTP_CPP_REGISTER_ENUM(F_UPGRADE);
                LLHTTP_CPP_REGISTER_ENUM(F_CONTENT_LENGTH);
                LLHTTP_CPP_REGISTER_ENUM(F_SKIPBODY);
                LLHTTP_CPP_REGISTER_ENUM(F_TRAILING);
                LLHTTP_CPP_REGISTER_ENUM(F_TRANSFER_ENCODING);
                default:
                    return "Unknown llhttp_flags";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_lenient_flags_t>) {
            switch (value) {
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_HEADERS);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_CHUNKED_LENGTH);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_KEEP_ALIVE);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_TRANSFER_ENCODING);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_VERSION);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_DATA_AFTER_CLOSE);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_OPTIONAL_LF_AFTER_CR);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_OPTIONAL_CRLF_AFTER_CHUNK);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_OPTIONAL_CR_BEFORE_LF);
                LLHTTP_CPP_REGISTER_ENUM(LENIENT_SPACES_AFTER_CHUNK_SIZE);
                default:
                    return "Unknown llhttp_lenient_flags";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_type_t>) {
            switch (value) {
                LLHTTP_CPP_REGISTER_ENUM(HTTP_BOTH);
                LLHTTP_CPP_REGISTER_ENUM(HTTP_REQUEST);
                LLHTTP_CPP_REGISTER_ENUM(HTTP_RESPONSE);
                default:
                    return "Unknown llhttp_type";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_finish_t>) {
            switch (value) {
                LLHTTP_CPP_REGISTER_ENUM(HTTP_FINISH_SAFE);
                LLHTTP_CPP_REGISTER_ENUM(HTTP_FINISH_SAFE_WITH_CB);
                LLHTTP_CPP_REGISTER_ENUM(HTTP_FINISH_UNSAFE);
                default:
                    return "Unknown llhttp_finish_t";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_method_t>) {
            switch (value) {
#define LLHTTP_CPP_REGISTER(num, name, name2) LLHTTP_CPP_REGISTER_ENUM(HTTP_##name);
                HTTP_ALL_METHOD_MAP(LLHTTP_CPP_REGISTER)
#undef LLHTTP_CPP_REGISTER
                default:
                    return "Unknown llhttp_method_t";
            }
        } else if constexpr (std::is_convertible_v<Enum, llhttp_status_t>) {
#define LLHTTP_CPP_REGISTER(num, name, name2) LLHTTP_CPP_REGISTER_ENUM(HTTP_STATUS_##name);
            switch (value) {
                HTTP_STATUS_MAP(LLHTTP_CPP_REGISTER)
                default:
                    return "Unknown llhttp_status_t";
            }
#undef LLHTTP_CPP_REGISTER
        } else {
            return "Unknown enum";
        }
    }

} // namespace llhttp

#endif // LLHTTP_CPP_INCLUDE_LLHTTP_CPP_COMMON_H
