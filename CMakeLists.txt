cmake_minimum_required(VERSION 3.9)
project(grblconnector VERSION 1.1.0 DESCRIPTION "Connector to GRBL")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( Threads REQUIRED )

set(BOOST_LIBS system regex)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/lib/asyncserial/AsyncSerial.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/gprogram.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/gparser.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/gtransceiver.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/grblconnector.cpp
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/inc/gcon.hpp
            ${CMAKE_CURRENT_LIST_DIR}/lib/asyncserial/AsyncSerial.hpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${Boost_INCLUDE_DIR}
            src
            lib
            inc
        INTERFACE
            .
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
)
