cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)

find_package( Threads REQUIRED )

set(BOOST_LIBS system regex unit_test_framework)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES
#        gprogram_test.cpp
        gstatus_test.cpp
        )

add_executable (GrblConnector_Test ${SOURCE_FILES})

target_link_libraries (GrblConnector_Test grblconnector)
target_link_libraries(GrblConnector_Test
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        )
