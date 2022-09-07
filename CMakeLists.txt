cmake_minimum_required(VERSION 3.23)
project(ap1_classifier_server)

set(CMAKE_CXX_STANDARD 11)

# adding client executable
file(GLOB client_sources client/*.cpp client/command_handlers/*.cpp common.cpp)
add_executable(ap1_classifier_client ${client_sources})

# adding server executable
file(GLOB server_sources server/classifier/*.cpp server/commands/*.cpp server/csv/*.cpp server/*.cpp io/*.cpp common.cpp )
add_executable(ap1_classifier_server ${server_sources})

# linking with pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ap1_classifier_server Threads::Threads)
target_link_libraries(ap1_classifier_client Threads::Threads)