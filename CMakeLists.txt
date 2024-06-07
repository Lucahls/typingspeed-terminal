cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)

project(TypingSpeedTerminal)
add_executable(${PROJECT_NAME} main.cpp
        src/TypingSpeedTerminal.cpp
        src/Frame.cpp)

find_package(fmt CONFIG REQUIRED)
find_package(ftxui REQUIRED)

target_link_libraries(${PROJECT_NAME} fmt::fmt ftxui::screen ftxui::dom ftxui::component)