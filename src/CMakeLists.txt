set(CLIENT system/client.cpp)
add_executable(client ${CLIENT})
target_link_libraries(client LibreSSL::TLS)

set(PROXY system/proxy.cpp)
add_executable(proxy ${PROXY})
target_link_libraries(proxy LibreSSL::TLS)

set(SERVER system/server.cpp)
add_executable(server ${SERVER})
target_link_libraries(server LibreSSL::TLS)
