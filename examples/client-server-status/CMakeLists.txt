cmake_minimum_required(VERSION 3.0.0)
project(server_status)

find_package(rpclib REQUIRED)

include_directories(${RPCLIB_INCLUDE_DIR})

add_executable(status_client main_client.cc)
add_executable(status_server main_server.cc)
target_link_libraries(status_client ${RPCLIB_LIBS})
target_link_libraries(status_server ${RPCLIB_LIBS})
set_property(TARGET status_client status_server PROPERTY CXX_STANDARD 14)

