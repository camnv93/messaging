cmake_minimum_required(VERSION 3.22)

project(net_lib)

# ===== Sources =====
set(NET_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/server_socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/utils.cpp
)

# ===== Library =====
add_library(net SHARED
        ${NET_SOURCES}
)
# add_executable(net main.cpp
#         ${NET_SOURCES}
# )

# ===== Include =====
target_include_directories(net
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ===== C++ standard =====
target_compile_features(net PUBLIC cxx_std_14)

# ===== Position Independent Code (QUAN TRỌNG cho .so) =====
set_target_properties(net PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

# ===== Visibility control (optional nhưng rất nên) =====
target_compile_options(net PRIVATE
        -fPIC
        -Wall -Wextra -Wpedantic
)

# ===== Versioning (RẤT QUAN TRỌNG cho shared lib) =====
set_target_properties(net PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
)

# ===== Output name =====
set_target_properties(net PROPERTIES
        OUTPUT_NAME "net"
)