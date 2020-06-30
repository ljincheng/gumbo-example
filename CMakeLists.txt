PROJECT(bebe_gumbo CXX)
cmake_minimum_required(VERSION 3.2)

# Debug build
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE Debug)

# Set our output directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Create library directory if it does not exist.
file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
 


# Set our library source directory.
set(CMAKE_CXX_STANDARD 11)
set(bebe_gumbo_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)


## 加入jsoncpp库名
find_package(jsoncpp)
if (jsoncpp_FOUND)
     set(LIB_JSONCPP_NAME "jsoncpp")
else ()
    set(LIB_JSONCPP_NAME "json_linux-gcc-4.8.5_libmt")
endif(jsoncpp_FOUND)

#FIND_LIBRARY(libJsoncpp json_linux-gcc-4.8.5_libmt /usr/local/lib)

#IF(NOT libJsoncpp)
#    set(LIB_JSONCPP_NAME "jsoncpp")
#ELSE ()
#     set(LIB_JSONCPP_NAME "json_linux-gcc-4.8.5_libmt")
#ENDIF(NOT libJsoncpp)


add_subdirectory(src)
add_subdirectory(test)

#  打印操作系统信息
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM_NAME},jsoncpp lib name is ${LIB_JSONCPP_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

MESSAGE(STSTUS "###################################")

 
