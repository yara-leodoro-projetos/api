cmake_minimum_required(VERSION 3.0.0)
project(ApiRest VERSION 0.1.0)

find_package(cpprestsdk REQUIRED)

include(CTest)
enable_testing()

add_executable(ApiRest MyCommandHandler.cpp)

target_link_libraries(ApiRest PRIVATE cpprestsdk::cpprest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
