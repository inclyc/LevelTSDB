cmake_minimum_required(VERSION 3.10.2)

project(LevelTSDB)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)

# Enable warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
  endif()
endif()

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(test)
