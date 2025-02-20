#
# Dependencies
#
find_package(Threads)

#
# Sources
#
set(SOURCES
    src/main.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


#
# Target
#
add_executable(tcp_client_test ${SOURCES})
target_link_libraries(tcp_client_test
    PRIVATE
        common
        myproto
)