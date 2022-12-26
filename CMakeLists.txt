cmake_minimum_required(VERSION 3.25)
project(grpcflat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH C:/Library/grpc-1.51.1 C:/Library/flatbuffers-22.12.06)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(flatbuffers REQUIRED)

set(FBS_NAME greeter)

set(GEN_SRC ${CMAKE_BINARY_DIR}/autogen/${FBS_NAME}_generated.h
        ${CMAKE_BINARY_DIR}/autogen/${FBS_NAME}.grpc.fb.cc ${CMAKE_BINARY_DIR}/autogen/${FBS_NAME}.grpc.fb.h)

add_custom_command(
        OUTPUT ${GEN_SRC}
        COMMAND flatbuffers::flatc
        ARGS --grpc --cpp -o ${CMAKE_BINARY_DIR}/autogen
        ${CMAKE_SOURCE_DIR}/${FBS_NAME}.fbs)

add_executable(echoclient ${GEN_SRC} client.cpp)
target_link_libraries(echoclient gRPC::grpc++ flatbuffers::flatbuffers)
target_include_directories(echoclient PRIVATE ${CMAKE_BINARY_DIR}/autogen)

add_executable(echoserver ${GEN_SRC} server.cpp)
target_link_libraries(echoserver gRPC::grpc++ flatbuffers::flatbuffers)
target_include_directories(echoserver PRIVATE ${CMAKE_BINARY_DIR}/autogen)
