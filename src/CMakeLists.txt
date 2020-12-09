set(CLIENT_SRC client.cpp)
add_executable(client ${CLIENT_SRC})
target_link_libraries(client LibreSSL::TLS)

set(SERVER_SRC server.cpp)
add_executable(server ${SERVER_SRC})
target_link_libraries(server LibreSSL::TLS)
