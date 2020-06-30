# libbebe_gumbo
include_directories("/usr/local/include")
#include_directories("/usr/local/include/gq")
include_directories("/usr/include/json-c")
link_directories("/usr/local/lib")
link_directories("/use/lib64")



file(GLOB SRC_FILES *.cpp)
add_library(bebe_gumbo_shared SHARED ${SRC_FILES})
set_target_properties(bebe_gumbo_shared PROPERTIES OUTPUT_NAME "bebegumbo")
add_library(bebe_gumbo_static STATIC ${SRC_FILES})
set_target_properties(bebe_gumbo_static PROPERTIES OUTPUT_NAME "bebegumbo")


target_link_libraries(bebe_gumbo_shared curl)
target_link_libraries(bebe_gumbo_shared gumbo)
#target_link_libraries(bebe_gumbo_shared gq)
target_link_libraries(bebe_gumbo_shared uriparser)
target_link_libraries(bebe_gumbo_shared sqlite3)
target_link_libraries(bebe_gumbo_shared ${LIB_JSONCPP_NAME})

## 加入jsoncpp库
# FIND_LIBRARY(libJsoncpp json_linux-gcc-4.8.5_libmt /usr/local/lib)

# IF(NOT libJsoncpp)
#    target_link_libraries(bebe_gumbo_shared jsoncpp)
# ELSE ()
#    target_link_libraries(bebe_gumbo_shared json_linux-gcc-4.8.5_libmt)
# ENDIF(NOT libJsoncpp)


set(bebe_gumbo_LIBRARIES bebe_gumbo_shared bebe_gumbo_static)
set_property(TARGET ${bebe_gumbo_LIBRARIES} PROPERTY CXX_STANARD 11)
set_property(TARGET ${bebe_gumbo_LIBRARIES} PROPERTY CXX_STANARD_REQUIRED ON)

install(TARGETS bebe_gumbo_shared bebe_gumbo_static
	LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)

file(GLOB HEAD_FILES *.h)
install(FILES ${HEAD_FILES} DESTINATION include/bebegumbo)


