cmake_minimum_required(VERSION 3.22)

project(common_lib)

# ===== Sources =====
set(COMMON_SOURCES
      # message/Message.cpp
      src/utils/nbo.cpp
      src/buffer/byte_buffer.cpp
)

# ===== Library =====
add_library(common SHARED
        ${COMMON_SOURCES}
)

# ===== Include =====
target_include_directories(common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ===== C++ standard =====
target_compile_features(common PUBLIC cxx_std_17)

# ===== Position Independent Code (QUAN TRỌNG cho .so) =====
set_target_properties(common PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

# ===== Visibility control (optional nhưng rất nên) =====
target_compile_options(common PRIVATE
        -fPIC
        -Wall -Wextra -Wpedantic
)

# ===== Versioning (RẤT QUAN TRỌNG cho shared lib) =====
set_target_properties(common PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
)

# ===== Output name =====
set_target_properties(common PROPERTIES
        OUTPUT_NAME "common"
)