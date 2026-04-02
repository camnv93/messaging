cmake_minimum_required(VERSION 3.22)
project(UTS)

# ===== Dependencies =====
find_package(GTest REQUIRED)

# ===== Common test config =====
function(add_uts_test target_name source_file)
    add_executable(${target_name} ${source_file})

    target_include_directories(${target_name}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

    target_compile_features(${target_name}
        PRIVATE
        cxx_std_17
    )

    target_link_libraries(${target_name}
        PRIVATE
        GTest::gtest_main
    )
endfunction()

# ===== Test targets =====
#add_uts_test(uts_nbo UTS_NBO.cpp)
# add_uts_test(uts_bytebuffer UTS_ByteBuffer.cpp)
add_uts_test(uts_tlv_writer UTS_TLV_WRITER.cpp)
# add_uts_test(uts_tlv_reader UTS_TLV_READER.cpp)
# add_uts_test(uts_tlv_builder UTS_TLV_MSG_BUILDER.cpp)
# ===== Auto discover tests (optional but recommended) =====
include(GoogleTest)

#gtest_discover_tests(uts_nbo)
# gtest_discover_tests(uts_bytebuffer)
gtest_discover_tests(uts_tlv_writer)
# gtest_discover_tests(uts_tlv_reader)
# gtest_discover_tests(uts_tlv_builder)