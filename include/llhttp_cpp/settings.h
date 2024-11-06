//
// Created by Renatus Madrigal on 10/29/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_SETTINGS_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_SETTINGS_H

#include <cstdlib>
#include "concepts.h"
#include "functional.h"
#include "llhttp.h"

namespace llhttp {
    class Settings {
    public:
        Settings() noexcept {
            settings_ = static_cast<SettingsPtr>(malloc(sizeof(llhttp_settings_t)));
            llhttp_settings_init(this->settings_);
        }

        Settings(const Settings &other) noexcept {
            settings_ = static_cast<SettingsPtr>(malloc(sizeof(llhttp_settings_t)));
            *(this->settings_) = *(other.settings_);
        }

        Settings(Settings &&other) noexcept {
            this->settings_ = other.settings_;
            // TODO: check if it is safe
            other.settings_ = nullptr;
        }

        // TODO: Find a way to Implement this better
        // This cannot be explicit to allow for the following syntax:
        // Settings settings = { .on_message_begin = []() { return 0; } };
        explicit Settings(const llhttp_settings_t &settings) noexcept {
            settings_ = static_cast<SettingsPtr>(malloc(sizeof(llhttp_settings_t)));
            *(this->settings_) = settings;
        }

        Settings &operator=(const Settings &other) noexcept {
            if (!settings_)
                settings_ = static_cast<SettingsPtr>(malloc(sizeof(llhttp_settings_t)));
            *(this->settings_) = *(other.settings_);
            return (*this);
        }

        Settings &operator=(Settings &&other) noexcept {
            this->settings_ = other.settings_;
            return *this;
        }

        ~Settings() { free(settings_); }

    private:
        // TODO: Implement this
        // template<Callable F>
        // [[maybe_unused]] static auto llhttpParserWrapper(F &&f) noexcept {
        //     return [&f]<typename... Args>(llhttp_t *, Args args...) {
        //         return f(args...);
        //     };
        // }

        // TODO: Implement convertCallback
        // TODO: Add std::string_view support and bind llhttp_parser to the callback if llhttp_t * is not a parameter
        // TODO: Try to support auto conversion between int_types
        // TODO: Check if it is necessary to support multiple callbacks for http parser
        template<Callable F, typename type>
            requires std::is_convertible_v<FunctionPointer<F>, type>
        [[maybe_unused]] static auto convertCallback(F &&f) noexcept {
            return toFunctionPointer(f);
        }

    public:
#define LLHTTP_CPP_SETTINGS_SETTER(func, name, type)                                                                   \
    template<Callable F>                                                                                               \
    Settings &func(F &&f) noexcept {                                                                                   \
        this->settings_->name = Settings::convertCallback<F, type>(std::forward<F &&>(f));                             \
        return *this;                                                                                                  \
    }

        LLHTTP_CPP_SETTINGS_SETTER(OnMessageBegin, on_message_begin, llhttp_cb)

        LLHTTP_CPP_SETTINGS_SETTER(OnUrl, on_url, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnStatus, on_status, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnMethod, on_method, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnVersion, on_version, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnHeaderField, on_header_field, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnHeaderValue, on_header_value, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnChunkExtensionName, on_chunk_extension_name, llhttp_data_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnChunkExtensionValue, on_chunk_extension_value, llhttp_data_cb)

        LLHTTP_CPP_SETTINGS_SETTER(OnHeadersComplete, on_headers_complete, llhttp_cb)

        LLHTTP_CPP_SETTINGS_SETTER(OnBody, on_body, llhttp_data_cb)

        LLHTTP_CPP_SETTINGS_SETTER(OnMessageComplete, on_message_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnUrlComplete, on_url_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnStatusComplete, on_status_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnMethodComplete, on_method_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnVersionComplete, on_version_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnHeaderFieldComplete, on_header_field_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnHeaderValueComplete, on_header_value_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnChunkExtensionNameComplete, on_chunk_extension_name_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnChunkExtensionValueComplete, on_chunk_extension_value_complete, llhttp_cb)

        LLHTTP_CPP_SETTINGS_SETTER(OnChunkHeader, on_chunk_header, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnChunkComplete, on_chunk_complete, llhttp_cb)
        LLHTTP_CPP_SETTINGS_SETTER(OnReset, on_reset, llhttp_cb)

#undef LLHTTP_CPP_SETTINGS_SETTER

        [[nodiscard]] llhttp_settings_t *get() const noexcept { return settings_; }

    private:
        // TODO: check if it is safe to add test proxy
#ifdef LLHTTP_CPP_TEST_ENABLED
        friend struct SettingsTestProxy;
#endif
        using SettingsPtr = llhttp_settings_t *;
        SettingsPtr settings_;
    };
} // namespace llhttp

#endif // LLHTTP_CPP_INCLUDE_LLHTTP_CPP_SETTINGS_H
