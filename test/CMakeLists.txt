enable_testing()

include(GoogleTest)
find_package(GTest REQUIRED)

set(TEST_SOURCES ${LLHTTP_CPP_TEST_DIR}/FunctionalTest.cpp
        ${LLHTTP_CPP_TEST_DIR}/SettingsTest.cpp)


message(STATUS "GTest libraries: ${GTEST_LIBRARIES}")
message(STATUS "GTest main libraries: ${GTEST_MAIN_LIBRARIES}")
message(STATUS "Test Sources: ${TEST_SOURCES}")

add_executable(LLHttpCppTest ${TEST_SOURCES})
target_link_libraries(LLHttpCppTest llhttp_cpp GTest::gtest_main)
gtest_discover_tests(LLHttpCppTest)
