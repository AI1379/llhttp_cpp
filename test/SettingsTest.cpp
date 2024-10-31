//
// Created by Renatus Madrigal on 10/29/2024.
//

#include <gtest/gtest.h>
#include "llhttp_cpp/settings.h"

struct llhttp::SettingsTestProxy {
    static Settings::SettingsPtr getRawPointer(const Settings &settings) { return settings.settings_; }
};

// http://info.cern.ch
[[maybe_unused]] static constexpr auto kTestRequest =
        R"(GET / HTTP/1.1)"
        "\r\n"
        R"(Accept: text/html)"
        "\r\n"
        R"(Accept-Encoding: gzip, deflate, br, zstd)"
        "\r\n"
        R"(Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7)"
        "\r\n"
        R"(Cache-Control: max-age=0)"
        "\r\n"
        R"(Connection: keep-alive)"
        "\r\n"
        R"(Host: info.cern.ch)"
        "\r\n"
        R"(If-Modified-Since: Wed, 05 Feb 2014 16:00:31 GMT)"
        "\r\n"
        R"(If-None-Match: "286-4f1aadb3105c0")"
        "\r\n"
        R"(Sec-Fetch-Dest: document)"
        "\r\n"
        R"(Sec-Fetch-Mode: navigate)"
        "\r\n"
        R"(Sec-Fetch-Site: none)"
        "\r\n"
        R"(Sec-Fetch-User: ?1)"
        "\r\n"
        R"(Upgrade-Insecure-Requests: 1)"
        "\r\n"
        R"(User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36 Edg/130.0.0.0)"
        "\r\n"
        R"(sec-ch-ua: "Chromium";v="130", "Microsoft Edge";v="130", "Not?A_Brand";v="99")"
        "\r\n"
        R"(sec-ch-ua-mobile: ?0)"
        "\r\n"
        R"(sec-ch-ua-platform: "Windows")"
        "\r\n";

[[maybe_unused]] static constexpr auto kTestResponse =
        R"(HTTP/1.1 200 OK)"
        "\r\n"
        R"(Date: Wed, 30 Oct 2024 07:20:13 GMT)"
        "\r\n"
        R"(Server: Apache)"
        "\r\n"
        R"(Last-Modified: Wed, 05 Feb 2014 16:00:31 GMT)"
        "\r\n"
        R"(ETag: "286-4f1aadb3105c0")"
        "\r\n"
        R"(Accept-Ranges: bytes)"
        "\r\n"
        R"(Connection: keep-alive)"
        "\r\n"
        R"()"
        "\r\n"
        R"(<!DOCTYPE html>)"
        "\r\n"
        R"(<html><head></head><body><header>)"
        "\r\n"
        R"(<title>http://info.cern.ch</title>)"
        "\r\n"
        R"(</header>)"
        "\r\n"
        R"()"
        "\r\n"
        R"(<h1>http://info.cern.ch - home of the first website</h1>)"
        "\r\n"
        R"(<p>From here you can:</p>)"
        "\r\n"
        R"(<ul>)"
        "\r\n"
        R"(<li><a href="http://info.cern.ch/hypertext/WWW/TheProject.html">Browse the first website</a></li>)"
        "\r\n"
        R"(<li><a href="http://line-mode.cern.ch/www/hypertext/WWW/TheProject.html">Browse the first website using the line-mode browser simulator</a></li>)"
        "\r\n"
        R"(<li><a href="http://home.web.cern.ch/topics/birth-web">Learn about the birth of the web</a></li>)"
        "\r\n"
        R"(<li><a href="http://home.web.cern.ch/about">Learn about CERN, the physics laboratory where the web was born</a></li>)"
        "\r\n"
        R"(</ul>)"
        "\r\n"
        R"(</body></html>)"
        "\r\n";

TEST(SettingsTest, OnMessageBegin) {
    llhttp::Settings settings;
    const auto rawPointer = llhttp::SettingsTestProxy::getRawPointer(settings);
    bool flag = false;
    llhttp_t parser;
    EXPECT_EQ(rawPointer->on_message_begin, nullptr);
    settings.OnMessageBegin([&](llhttp_t *) {
        flag = true;
        return 0;
    });
    EXPECT_NE(rawPointer->on_message_begin, nullptr);
    llhttp_init(&parser, HTTP_BOTH, rawPointer);
    const auto request = kTestRequest;
    const size_t request_len = strlen(request);
    const llhttp_errno_t err = llhttp_execute(&parser, request, request_len);
    EXPECT_TRUE(flag);
    EXPECT_EQ(err, HPE_OK);
}

TEST(SettingsTest, OnUrl) {
    llhttp::Settings settings;
    const auto rawPointer = llhttp::SettingsTestProxy::getRawPointer(settings);
    bool flag = false;
    std::string_view reqUrl;
    llhttp_t parser;
    EXPECT_EQ(rawPointer->on_url, nullptr);
    settings.OnUrl([&](llhttp_t *, const char *url, size_t len) {
        flag = true;
        reqUrl = {url, len};
        return 0;
    });
    EXPECT_NE(rawPointer->on_url, nullptr);
    llhttp_init(&parser, HTTP_BOTH, rawPointer);
    const llhttp_errno_t err = llhttp_execute(&parser, kTestRequest, strlen(kTestRequest));
    EXPECT_TRUE(flag);
    EXPECT_EQ(reqUrl, "/") << reqUrl;
    EXPECT_EQ(err, HPE_OK);
}

TEST(SettingsTest, OnStatus) {
    llhttp::Settings settings;
    const auto rawPointer = llhttp::SettingsTestProxy::getRawPointer(settings);
    bool flag = false;
    llhttp_t parser;
    EXPECT_EQ(rawPointer->on_status_complete, nullptr);
    settings.OnStatus([&](llhttp_t *, const char *, size_t) {
        flag = true;
        return 0;
    });
    EXPECT_NE(rawPointer->on_status, nullptr);
    llhttp_init(&parser, HTTP_BOTH, rawPointer);
    const llhttp_errno_t err = llhttp_execute(&parser, kTestResponse, strlen(kTestResponse));
    EXPECT_TRUE(flag);
    EXPECT_EQ(llhttp_get_status_code(&parser), 200);
    EXPECT_EQ(err, HPE_OK);
}
