cmake_minimum_required(VERSION 3.14)

project(server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

find_package(Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )

add_executable(server
  main.cpp
  server.h server.cpp
  database.h database.cpp
  xmlparser.h xmlparser.cpp
  BlockModel.h
  BoardModel.h
  PortModel.h
)

file(COPY "./db_xml" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/db_xml)

target_link_libraries(server Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
