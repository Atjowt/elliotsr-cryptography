cmake_minimum_required(VERSION 3.0.0)
project(cryptography VERSION 0.1.0 LANGUAGES C)

add_executable(cryptography main.c)

# Link and include libsodium
target_include_directories(cryptography PRIVATE ${LIBSODIUM_INCLUDE_DIRS})
target_link_libraries(cryptography PRIVATE ${LIBSODIUM_LIBRARIES})

# Link libcurl
target_link_libraries(cryptography PRIVATE curl)