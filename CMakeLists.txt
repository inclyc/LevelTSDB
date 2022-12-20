cmake_minimum_required(VERSION 3.10.2)

project(LevelTSDB)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)

add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  option(LOCAL_PROJECT_USE_ASAN "Build with AddressSanitizer" On)
else ()
  option(LOCAL_PROJECT_USE_ASAN "Build with AddressSanitizer" Off)
endif()

if(LOCAL_PROJECT_USE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()


option(LOCAL_PROJECT_USE_TSAN "Build with ThreadSanitizer" Off)
if(LOCAL_PROJECT_USE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()

if( PROJECT_USE_LINKER )
  append("-fuse-ld=${PROJECT_USE_LINKER}"
    CMAKE_EXE_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS)
  check_cxx_source_compiles("int main() { return 0; }" CXX_SUPPORTS_CUSTOM_LINKER)
  if ( NOT CXX_SUPPORTS_CUSTOM_LINKER )
    message(FATAL_ERROR "Host compiler does not support '-fuse-ld=${PROJECT_USE_LINKER}'")
  endif()
endif()

# Enable warnings
add_compile_options(-Wall -Wextra -pedantic)

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(test)
