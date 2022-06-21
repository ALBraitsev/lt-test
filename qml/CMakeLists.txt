cmake_minimum_required(VERSION 3.16)

project(lt-qml-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Network Widgets Quick Qml REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    qml.qrc
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets Qt5::Quick Qt5::Qml )
