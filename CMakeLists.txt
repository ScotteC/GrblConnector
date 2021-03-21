cmake_minimum_required(VERSION 3.9)
project(libgrblconnector VERSION 1.0.0 DESCRIPTION "Connector to GRBL")

set(CMAKE_CXX_STANDARD 17)

find_package( Threads REQUIRED )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(BOOST_LIBS system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED
        src/AsyncSerial.cpp
        inc/gprogram.hpp
        src/gprogram.cpp
        src/gstatus.cpp
        src/gtransceiver.cpp
        src/grbl-controller.cpp
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        )

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE inc)
#target_include_directories(${PROJECT_NAME} PRIVATE src)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# unit testing
add_subdirectory(test)
