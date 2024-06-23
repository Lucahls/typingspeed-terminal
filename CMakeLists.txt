cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)

project(TypingSpeedTerminal)
add_executable(${PROJECT_NAME} main.cpp
        src/TypingSpeedTerminal.cpp
        src/Frame.cpp
        src/Quotes.cpp
        src/Timer.cpp)

find_package(fmt CONFIG REQUIRED)
find_package(ftxui REQUIRED)
find_package(cpr REQUIRED)


find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} fmt::fmt ftxui::screen ftxui::dom ftxui::component cpr::cpr nlohmann_json::nlohmann_json)