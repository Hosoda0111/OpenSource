cmake_minimum_required(VERSION 3.16)
project(WallpaperCalcApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

set(SOURCES
    main.cpp mainwindow.cpp taskmanager.cpp calcworker.cpp calcprogressdialog.cpp
)
set(HEADERS
    mainwindow.h taskmanager.h calcworker.h calcprogressdialog.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ole32 oleaut32 user32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
