message(
        STATUS "CMakeLists.txt: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}"
)

set(LLHTTP_CPP_SOURCES ${LLHTTP_CPP_SOURCE_DIR}/llhttp_parser.cpp)

include_directories(${LLHTTP_CPP_INCLUDE_DIR})

message(STATUS "CMakeLists.txt: LLHTTP_CPP_SOURCES: ${LLHTTP_CPP_SOURCES}")

add_library(llhttp_cpp ${LLHTTP_CPP_SOURCES})
target_include_directories(llhttp_cpp PUBLIC ${LLHTTP_CPP_INCLUDE_DIR})
target_link_libraries(llhttp_cpp PRIVATE llhttp_static)
