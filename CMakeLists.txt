cmake_minimum_required(VERSION 3.28)

project(TypeSpeedTerminal)
add_executable(${PROJECT_NAME} main.cpp)

find_package(fmt CONFIG REQUIRED)
find_package(ftxui REQUIRED)

target_link_libraries(${PROJECT_NAME} fmt::fmt ftxui::screen ftxui::dom ftxui::component)