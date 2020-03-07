cmake_minimum_required(VERSION 3.11.4)
project(kafe_cli VERSION ${KAFE_VERSION} LANGUAGES CXX C)

message(STATUS "kafe cli version is defined as ${PROJECT_VERSION}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FIND_LIBRARY_USE_LIB64_PATHS ON)

if(NOT UNIX AND NOT APPLE)
    message( FATAL_ERROR "Not Unix or Apple, your operating system is not supported!" )
endif()

if(APPLE)
    set(CMAKE_PREFIX_PATH  "/usr/local/opt/libarchive/;/usr/local/opt/libssh/;/usr/local/opt/curl/;/usr/local/opt/libgit2/;/usr/local/opt/lua/")
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "CMAKE IN DEBUG MODE")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "CMAKE IN RELEASE MODE")
endif ()

find_package(Lua 5.3 REQUIRED)
find_package(LIBSSH 0.7 REQUIRED)
find_package(LibArchive 3.1.2 REQUIRED)
find_package(CURL 7.11 REQUIRED)
find_package(LIBGIT2 REQUIRED)
find_package(Filesystem COMPONENTS Experimental Final REQUIRED)

add_executable(kafe_cli main.cpp main.hpp logger.hpp logger.cpp)

target_link_libraries(kafe_cli PRIVATE kafe_lib_shared)

target_include_directories(kafe_cli PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(kafe_cli PRIVATE ${LIBSSH_INCLUDE_DIR})
target_include_directories(kafe_cli PRIVATE ${LibArchive_INCLUDE_DIRS})
target_include_directories(kafe_cli PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(kafe_cli PRIVATE ${LIBGIT2_INCLUDE_DIR})

string(TIMESTAMP KAFE_CMAKE_LIB_BUILD_TS "%Y-%m-%d" UTC)
target_compile_definitions(kafe_cli PUBLIC KAFE_CMAKE_CLI_BUILD_TS="${KAFE_CMAKE_LIB_BUILD_TS}")

target_compile_definitions(kafe_cli PUBLIC KAFE_CMAKE_CLI_VERSION="${PROJECT_VERSION}")
target_compile_definitions(kafe_cli PUBLIC KAFE_CMAKE_CLI_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(kafe_cli PUBLIC KAFE_CMAKE_CLI_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(kafe_cli PUBLIC KAFE_CMAKE_CLI_VERSION_RELEASE=${PROJECT_VERSION_PATCH})

find_path(KAFE_CMAKE_LIB_LUA_54 lua5.4/lua.h PATHS ${LUA_INCLUDE_DIR})
find_path(KAFE_CMAKE_LIB_LUA_53 lua5.3/lua.h PATHS ${LUA_INCLUDE_DIR})
find_path(KAFE_CMAKE_LIB_LUA_ROOT lua.h PATHS ${LUA_INCLUDE_DIR})

if (KAFE_CMAKE_LIB_LUA_54)
    target_include_directories(kafe_cli PRIVATE ${KAFE_CMAKE_LIB_LUA_54})
elseif (KAFE_CMAKE_LIB_LUA_53)
    target_include_directories(kafe_cli PRIVATE ${KAFE_CMAKE_LIB_LUA_53})
elseif (KAFE_CMAKE_LIB_LUA_ROOT)
    target_include_directories(kafe_cli PRIVATE ${KAFE_CMAKE_LIB_LUA_ROOT})
else ()
    message(FATAL_ERROR "Could not determine Lua header to include. Expected either <luaN.N/lua.hpp> or <lua.hpp>")
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "kafe")
include(GNUInstallDirs)
install(TARGETS kafe_cli DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT cli)
