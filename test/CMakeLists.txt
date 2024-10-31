enable_testing()

include(GoogleTest)
find_package(GTest)

if(NOT GTest_FOUND)

message(STATUS "Cannot find GTest. Try to download it.")
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5ed21863955149a5a877a53d7d5045b6919090ed.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

endif()

set(TEST_SOURCES ${LLHTTP_CPP_TEST_DIR}/FunctionalTest.cpp
        ${LLHTTP_CPP_TEST_DIR}/SettingsTest.cpp)


message(STATUS "GTest libraries: ${GTEST_LIBRARIES}")
message(STATUS "GTest main libraries: ${GTEST_MAIN_LIBRARIES}")
message(STATUS "Test Sources: ${TEST_SOURCES}")

add_executable(LLHttpCppTest ${TEST_SOURCES})
target_link_libraries(LLHttpCppTest llhttp_cpp GTest::gtest_main)
gtest_discover_tests(LLHttpCppTest)
