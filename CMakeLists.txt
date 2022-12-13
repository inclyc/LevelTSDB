cmake_minimum_required(VERSION 3.10.2)

project(LevelTSDB)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)

add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set (LOCAL_PROJECT_USE_ASAN On)
endif()


option(LOCAL_PROJECT_USE_ASAN "Build with AddressSanitizer" Off)
if(LOCAL_PROJECT_USE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()


option(LOCAL_PROJECT_USE_TSAN "Build with ThreadSanitizer" Off)
if(LOCAL_PROJECT_USE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()


# Enable warnings
add_compile_options(-Wall -Wextra -pedantic)

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(test)
