cmake_minimum_required(VERSION 3.0.0)
project(cryptography VERSION 0.1.0 LANGUAGES C)

add_executable(client src/client.c)
add_executable(server src/server.c)

# Include local folder
include_directories(include)

# Link and include libsodium
include_directories(${LIBSODIUM_INCLUDE_DIRS})
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)
target_link_libraries(client PRIVATE ${LIBSODIUM_LIBRARIES})
target_link_libraries(server PRIVATE ${LIBSODIUM_LIBRARIES})
# target_link_libraries(client PRIVATE libsodium)
