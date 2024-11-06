//
// Created by Renatus Madrigal on 10/30/2024.
//

#ifndef LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H
#define LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H

#include <cstdlib>
#include "llhttp.h"
#include "llhttp_cpp/common.h"
#include "llhttp_cpp/settings.h"

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

        [[nodiscard]] ParserType type() const noexcept { return toEnum<ParserType>(llhttp_get_type(this->parser_)); }

        [[nodiscard]] uint8_t httpMajor() const noexcept { return llhttp_get_http_major(this->parser_); }

        [[nodiscard]] uint8_t httpMinor() const noexcept { return llhttp_get_http_minor(this->parser_); }

        [[nodiscard]] Method method() const noexcept { return toEnum<Method>(llhttp_get_method(this->parser_)); }

        [[nodiscard]] uint8_t statusCode() const noexcept { return llhttp_get_status_code(this->parser_); }

        [[nodiscard]] bool upgrade() const noexcept { return llhttp_get_upgrade(this->parser_); }

        void reset() const noexcept { llhttp_reset(this->parser_); }

        Error execute(const char *data, size_t length) const noexcept {
            return toEnum<Error>(llhttp_execute(this->parser_, data, length));
        }

        void init(Type type, const Settings &settings) const noexcept {
            llhttp_init(this->parser_, static_cast<llhttp_type_t>(type), settings.get());
        }

        [[nodiscard]] Error finish() const noexcept { return toEnum<Error>(llhttp_finish(this->parser_)); }

        void pause() const noexcept { llhttp_pause(this->parser_); }

        void resume() const noexcept { llhttp_resume(this->parser_); }

        void resumeAfterUpgrade() const noexcept { llhttp_resume_after_upgrade(this->parser_); }

        [[nodiscard]] Error getError() const noexcept { return toEnum<Error>(llhttp_get_errno(this->parser_)); }

        [[nodiscard]] std::string_view getErrorReason() const noexcept {
            return llhttp_get_error_reason(this->parser_);
        }

        // TODO: Check this API
        void setErrorReason(const char *reason) const noexcept { llhttp_set_error_reason(this->parser_, reason); }

        [[nodiscard]] const char *getErrorPos() const noexcept { return llhttp_get_error_pos(this->parser_); }

        [[nodiscard]] bool messageNeedsEOF() const noexcept { return llhttp_message_needs_eof(this->parser_); }

        [[nodiscard]] bool shouldKeepAlive() const noexcept { return llhttp_should_keep_alive(this->parser_); }

        [[nodiscard]] llhttp_t *get() const noexcept { return parser_; }

        // llhttp_errno_name llhttp_method_name llhttp_status_name

        void setLenientFlags(llhttp_lenient_flags_t flags) const noexcept {
#define LLHTTP_CPP_REGISTER_LENIENT_CONFIG(name, name2)                                                                \
    if (flags & name) {                                                                                                \
        this->set##name2(true);                                                                                        \
    }
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_HEADERS, LenientHeaders);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_CHUNKED_LENGTH, LenientChunkedLength);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_KEEP_ALIVE, LenientKeepAlive);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_TRANSFER_ENCODING, LenientTransferEncoding);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_VERSION, LenientVersion);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_DATA_AFTER_CLOSE, LenientDataAfterClose);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_OPTIONAL_LF_AFTER_CR, LenientOptionalLfAfterCr);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_OPTIONAL_CRLF_AFTER_CHUNK, LenientOptionalCrlfAfterChunk);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_OPTIONAL_CR_BEFORE_LF, LenientOptionalCrBeforeLf);
            LLHTTP_CPP_REGISTER_LENIENT_CONFIG(LENIENT_SPACES_AFTER_CHUNK_SIZE, LenientSpacesAfterChunkSize);

#undef LLHTTP_CPP_REGISTER_LENIENT_CONFIG
        }

        void setLenientHeaders(bool enabled) const noexcept { llhttp_set_lenient_headers(this->parser_, enabled); }

        void setLenientChunkedLength(bool enabled) const noexcept {
            llhttp_set_lenient_chunked_length(this->parser_, enabled);
        }

        void setLenientKeepAlive(bool enabled) const noexcept { llhttp_set_lenient_keep_alive(this->parser_, enabled); }

        void setLenientTransferEncoding(bool enabled) const noexcept {
            llhttp_set_lenient_transfer_encoding(this->parser_, enabled);
        }

        void setLenientVersion(bool enabled) const noexcept { llhttp_set_lenient_version(this->parser_, enabled); }

        void setLenientDataAfterClose(bool enabled) const noexcept {
            llhttp_set_lenient_data_after_close(this->parser_, enabled);
        }

        void setLenientOptionalLfAfterCr(bool enabled) const noexcept {
            llhttp_set_lenient_optional_lf_after_cr(this->parser_, enabled);
        }

        void setLenientOptionalCrBeforeLf(bool enabled) const noexcept {
            llhttp_set_lenient_optional_cr_before_lf(this->parser_, enabled);
        }

        void setLenientOptionalCrlfAfterChunk(bool enabled) const noexcept {
            llhttp_set_lenient_optional_crlf_after_chunk(this->parser_, enabled);
        }

        void setLenientSpacesAfterChunkSize(bool enabled) const noexcept {
            llhttp_set_lenient_spaces_after_chunk_size(this->parser_, enabled);
        }

    protected:
        [[nodiscard]] llhttp_t *get() const noexcept { return parser_; }


    private:
        llhttp_t *parser_;
    };
} // namespace llhttp

#endif // LLHTTP_CPP_INCLUDE_LLHTTP_CPP_PARSER_H
