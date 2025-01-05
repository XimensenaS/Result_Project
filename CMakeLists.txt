cmake_minimum_required(VERSION 3.15.0)
project(search_engine VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${json_SOURCE_DIR}/include)

FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

FetchContent_Declare(
  thread-pool
  GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
  GIT_TAG        737c6036103ac034682ee11c31634ebe6769781f # tag: v2.0.0 
)
FetchContent_MakeAvailable(thread-pool)

include_directories(${thread-pool_SOURCE_DIR})

add_subdirectory(Google_tests)

include_directories(include src)

add_executable(search_engine search_engine.cpp
                            src/ConverterJSON.cpp
                            src/InvertedIndex.cpp
                            src/SearchServer.cpp
                            src/utilities.cpp
)

target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)