cmake_minimum_required(VERSION 3.16)

project(lt-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

add_executable(${PROJECT_NAME}
    reporttools.cpp
    myserver.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Sql)