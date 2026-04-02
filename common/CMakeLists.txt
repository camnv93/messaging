cmake_minimum_required(VERSION 3.22)

project(common_lib)

# ===== Sources =====
set(COMMON_SOURCES
      src/utils/nbo.cpp
      src/utils/error.cpp
      src/utils/hex2bin.cpp
      src/utils/bit_convert.cpp
      src/utils/looper.cpp
      src/buffer/byte_buffer.cpp
      src/codec/tlv/tlv_writer.cpp
      src/codec/tlv/tlv_reader.cpp
      src/codec/tlv/tlv_view.cpp
      src/protocol/message.cpp
      src/protocol/tlv_message_builder.cpp
)

# ===== Library =====
add_library(common SHARED
        ${COMMON_SOURCES}
)
target_compile_definitions(common PRIVATE -DDEBUG_)
# ===== Include =====
target_include_directories(common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_include_directories(common
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../library/include/
)


# ===== C++ standard =====
target_compile_features(common PUBLIC cxx_std_14)

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