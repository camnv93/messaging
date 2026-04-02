cmake_minimum_required(VERSION 3.22)

project(service)

# ===== Sources =====
set(CLIENT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/tcp_processor.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/src/server/tcp_server.cpp
)

# ===== Library =====
# add_library(service SHARED
#         ${CLIENT_SOURCES}
# )
add_executable(service main_client.cpp ${CLIENT_SOURCES})
target_compile_definitions(service PRIVATE -DDEBUG_)
# ===== Include =====
target_include_directories(service
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../library/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../net/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ===== C++ standard =====
target_compile_features(service PUBLIC cxx_std_14)

# ===== Position Independent Code (QUAN TRỌNG cho .so) =====
# set_target_properties(client_test PROPERTIES
#         POSITION_INDEPENDENT_CODE ON
# )
target_link_libraries(service PRIVATE /home/devcpp/messaging/net/build/libnet.so.1.0.0)
target_link_libraries(service PRIVATE /home/devcpp/messaging/common/cmake-build-debug/libcommon.so.1.0.0)

# ===== Visibility control (optional nhưng rất nên) =====
# target_compile_options(client_test PRIVATE
#         -fPIC
#         -Wall -Wextra -Wpedantic
# )

# # ===== Versioning (RẤT QUAN TRỌNG cho shared lib) =====
# set_target_properties(client_test PROPERTIES
#         VERSION 1.0.0
#         SOVERSION 1
# )

# # ===== Output name =====
# set_target_properties(client_test PROPERTIES
#         OUTPUT_NAME "client_test"
# )