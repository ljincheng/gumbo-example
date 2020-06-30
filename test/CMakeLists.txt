include_directories("/usr/local/include")
#include_directories("/usr/local/include/gq")
include_directories("/usr/include/json-c")
include_directories(${bebe_gumbo_SOURCE_DIRECTORY})
link_directories("/usr/local/lib")
link_directories("/use/lib64")

add_executable(test_html test_gq.cpp)
add_executable(test_gq2 test_gq2.cpp)

# book资讯采集
add_executable(test_book book_main.cpp)
# flai 数据采集
add_executable(test_flai flai_main.cpp)

#target_link_libraries(test_html gq)
target_link_libraries(test_html bebe_gumbo_shared)
target_link_libraries(test_gq2 curl)
target_link_libraries(test_book bebe_gumbo_shared)
target_link_libraries(test_flai bebe_gumbo_shared)


set_property(TARGET ${test_html} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${test_html} PROPERTY CXX_STANDARD_REQUIRED ON)
