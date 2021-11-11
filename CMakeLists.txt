cmake_minimum_required(VERSION 3.9)
project(grblconnector VERSION 1.1.0 DESCRIPTION "Connector to GRBL")

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package( Threads REQUIRED )

set(BOOST_LIBS system regex)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB HEADERS_PUB_ROOT "inc/*.hpp")
file(GLOB HEADERS_PUB_SUB "inc/**/*.hpp")
set(HEADERS_PUB ${HEADERS_PUB_ROOT} ${HEADERS_PUB_SUB})

set(SOURCE_FILES
    ${HEADERS_PUB}
    lib/asyncserial/AsyncSerial.hpp
    lib/asyncserial/AsyncSerial.cpp
    src/gprogram.cpp
    src/gparser.cpp
    src/gtransceiver.cpp
    src/grblconnector.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
      ${Boost_LIBRARIES}
      Threads::Threads
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
#            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
            src
            lib
)

# unit testing
#add_subdirectory(test)

set(${PROJECT_NAME}_LIB ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lib${PROJECT_NAME}.so)

set(CMAKE_CONFIG_DIR ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake)
configure_package_config_file(
        cmake/${PROJECT_NAME}.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_CONFIG_DIR}
)

install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${HEADERS_PUB}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_CONFIG_DIR}
        NAMESPACE GCON::
)
install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKE_CONFIG_DIR}
)

export(
        EXPORT ${PROJECT_NAME}Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
        NAMESPACE GCON::
)
