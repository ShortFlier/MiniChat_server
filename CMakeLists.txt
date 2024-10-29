cmake_minimum_required(VERSION 3.16)

project(chat_server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(chat_server
  main.cpp
  littlesmtp.h littlesmtp.cpp
  websocketconnect.h websocketconnect.cpp
  webconnect.h webconnect.cpp
  datahead.h datahead.cpp
  src.qrc
  webmanager.h webmanager.cpp
  webconnectpool.h webconnectpool.cpp
  webtelecom.h webtelecom.cpp
  dbconnect.h dbconnect.cpp
  controller.h controller.cpp
  application.h application.cpp
  captcha.h captcha.cpp
  entity.h
  entity.cpp
)
target_link_libraries(chat_server PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(chat_server PRIVATE Qt6::WebSockets)
target_link_libraries(chat_server PRIVATE Qt6::Sql)
target_link_libraries(chat_server PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS chat_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
