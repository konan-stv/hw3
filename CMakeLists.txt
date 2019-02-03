cmake_minimum_required(VERSION 3.6)

## Custom Allocator
set (PROJECT_NAME "calloc")

## Set our project name
project (${PROJECT_NAME} CXX)

set (ENV{TRAVIS_PROJECT_NAME} "${PROJECT_NAME}")
set(file_sh ${CMAKE_CURRENT_BINARY_DIR}/pname.sh)
file(WRITE ${file_sh} "#!/usr/bin/env bash\n")
file(APPEND ${file_sh} "export TRAVIS_PROJECT_NAME=\"${PROJECT_NAME}\"")
execute_process(COMMAND chmod a+x ${file_sh} RESULT_VARIABLE res)
#execute_process(COMMAND . ./${file_sh} RESULT_VARIABLE res)

set (GTEST_ROOT /usr/src/gtest)
set (GTEST_LIBRARY /usr/src/gtest/libgtest.a)
set (GTEST_MAIN_LIBRARY /usr/src/gtest/libgtest_main.a)

find_package (GTest REQUIRED)
find_package (Threads REQUIRED)
include_directories (${GTEST_INCLUDE_DIR})

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
  project(${PROJECT_NAME} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
  project(${PROJECT_NAME} VERSION 0.0.255)
endif()

configure_file(version.h.in version.h)

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS "*.cpp")

## Define the executable
add_library(kalloc kalloc.cpp)

add_executable(runTest test.cpp)
add_executable(${PROJECT_NAME} main.cpp)

add_dependencies(runTest kalloc)
add_dependencies(${PROJECT_NAME} kalloc)

target_link_libraries(
  runTest ${GTEST_LIBRARIES} Threads::Threads kalloc)

target_link_libraries(
  ${PROJECT_NAME} ${GTEST_LIBRARIES} kalloc)

set_target_properties(${PROJECT_NAME} runTest PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;--std=c++1z"
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT konan@konanlabs.ru)

include(CPack)

enable_testing()

add_test(runTest runTest)
