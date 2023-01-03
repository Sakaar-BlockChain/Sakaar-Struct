cmake_minimum_required(VERSION 3.22)
project(Sakaar_Struct C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/cmake"   CACHE PATH "The the path to the cmake directory")
option(ECC_USE_EXTERNAL_JSON "Use an external JSON library" OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
MESSAGE(STATUS ${CMAKE_MODULE_PATH})
include(FindGMP)

if(NOT TARGET Sakaar_Memory_lib)
    add_subdirectory(lib/Sakaar-Memory)
    link_directories(lib/Sakaar-Memory)
    MESSAGE(STATUS "Connected: Sakaar_Memory_lib")
endif()

FILE(GLOB SOURCE_FILES_DIR_DIR "src/*/*/*.c")
FILE(GLOB SOURCE_FILES_DIR "src/*/*.c")
FILE(GLOB SOURCE_FILES "src/*.c")
add_library(${PROJECT_NAME}_lib ${SOURCE_FILES} ${SOURCE_FILES_DIR} ${SOURCE_FILES_DIR_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
MESSAGE(STATUS "Connected: Sakaar_Struct_lib")

target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Memory_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC gmp gmpxx)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)