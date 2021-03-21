set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

set(SOURCE_FILES
        gprogram_test.cpp
        )

add_executable (GrblConnector_Test ${SOURCE_FILES})

target_link_libraries (GrblConnector_Test ${Boost_LIBRARIES})
target_link_libraries (GrblConnector_Test libgrblconnector)
