cmake_minimum_required(VERSION 3.9)
project(grblconnector VERSION 1.0.0 DESCRIPTION "Connector to GRBL")

set(CMAKE_CXX_STANDARD 17)

find_package( Threads REQUIRED )

set(BOOST_LIBS system regex)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(GNUInstallDirs)

set(SOURCE_FILES
        src/AsyncSerial.cpp
        src/gprogram.cpp
    src/gparser.cpp
        src/gtransceiver.cpp
        src/grblconnector.cpp
        )

set(HEADER_FILES
        inc/AsyncSerial.hpp
        inc/gcommand.hpp
        inc/gdumper.hpp
        inc/gmodal.hpp
        inc/gstatus.hpp
        inc/gerror.hpp
        inc/galarm.hpp
        inc/gprogram.hpp
        inc/grblconnector.hpp
        inc/gsender.hpp
    inc/gparser.hpp
        inc/gtransceiver.hpp
        )


add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        )

#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${HEADER_FILES})

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_include_directories(${PROJECT_NAME} PRIVATE src)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# unit testing
add_subdirectory(test)
