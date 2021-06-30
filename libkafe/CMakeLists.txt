cmake_minimum_required(VERSION 3.11.4)
project(kafe VERSION ${KAFE_VERSION} LANGUAGES CXX C)

# Important: must be update for breaking scripting API changes
set(KAFE_API_LEVEL 1)

message(STATUS "libkafe version is defined as ${PROJECT_VERSION}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FIND_LIBRARY_USE_LIB64_PATHS ON)

if (NOT UNIX AND NOT APPLE)
    message(FATAL_ERROR "Not Unix or Apple, your operating system is not supported!")
endif ()

if(APPLE)
    set(CMAKE_PREFIX_PATH  "/usr/local/opt/libarchive/;/usr/local/opt/libssh/;/usr/local/opt/curl/;/usr/local/opt/libgit2/;/usr/local/opt/lua/;/opt/homebrew/opt/libarchive/;/opt/homebrew/opt/libssh/;/opt/homebrew/opt/curl/;/opt/homebrew/opt/libgit2/;/opt/homebrew/opt/lua/")
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "CMAKE IN DEBUG MODE")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "CMAKE IN RELEASE MODE")
endif ()

find_package(Lua 5.3 REQUIRED)
find_package(LIBSSH 0.7 REQUIRED)
find_package(LibArchive 3.1.2 REQUIRED)
find_package(CURL 7.11 REQUIRED)
find_package(LIBGIT2 REQUIRED)
find_package(Filesystem COMPONENTS Experimental Final REQUIRED)

file(GLOB_RECURSE _HEADERS "include/*.hpp")
file(GLOB_RECURSE _SOURCES "src/*.[hc]pp")

add_library(kafe_lib_shared SHARED ${_HEADERS} ${_SOURCES})
add_library(kafe_lib_static STATIC ${_HEADERS} ${_SOURCES})

set_target_properties(kafe_lib_shared PROPERTIES OUTPUT_NAME "kafe" SOVERSION ${KAFE_SOVERSION})
set_target_properties(kafe_lib_static PROPERTIES OUTPUT_NAME "kafe")

if (LUA_VERSION_MAJOR EQUAL 5 AND LUA_VERSION_MINOR EQUAL 4)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_LUA_54)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_LUA_54)
    message(STATUS "Building with Lua 5.4")
elseif (LUA_VERSION_MAJOR EQUAL 5 AND LUA_VERSION_MINOR EQUAL 3)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_LUA_53)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_LUA_53)
    message(STATUS "Building with Lua 5.3")
else ()
    message(FATAL_ERROR "Unsupported Lua version")
endif ()

target_link_libraries(kafe_lib_shared LINK_PRIVATE ${LUA_LIBRARIES})
target_link_libraries(kafe_lib_static LINK_PRIVATE ${LUA_LIBRARIES})

target_link_libraries(kafe_lib_shared LINK_PRIVATE ${LIBSSH_LIBRARIES})
target_link_libraries(kafe_lib_static LINK_PRIVATE ${LIBSSH_LIBRARIES})

target_link_libraries(kafe_lib_shared LINK_PRIVATE ${LibArchive_LIBRARIES})
target_link_libraries(kafe_lib_static LINK_PRIVATE ${LibArchive_LIBRARIES})

target_link_libraries(kafe_lib_shared LINK_PRIVATE ${CURL_LIBRARIES})
target_link_libraries(kafe_lib_static LINK_PRIVATE ${CURL_LIBRARIES})

target_link_libraries(kafe_lib_shared LINK_PRIVATE ${LIBGIT2_LIBRARIES})
target_link_libraries(kafe_lib_static LINK_PRIVATE ${LIBGIT2_LIBRARIES})

target_link_libraries(kafe_lib_shared LINK_PRIVATE std::filesystem)
target_link_libraries(kafe_lib_static LINK_PRIVATE std::filesystem)

target_include_directories(kafe_lib_shared PUBLIC include)
target_include_directories(kafe_lib_static PUBLIC include)

target_include_directories(kafe_lib_shared PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(kafe_lib_static PRIVATE ${LUA_INCLUDE_DIR})

target_include_directories(kafe_lib_shared PRIVATE ${LIBSSH_INCLUDE_DIR})
target_include_directories(kafe_lib_static PRIVATE ${LIBSSH_INCLUDE_DIR})

target_include_directories(kafe_lib_shared PRIVATE ${LibArchive_INCLUDE_DIRS})
target_include_directories(kafe_lib_static PRIVATE ${LibArchive_INCLUDE_DIRS})

target_include_directories(kafe_lib_shared PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(kafe_lib_static PRIVATE ${CURL_INCLUDE_DIRS})

target_include_directories(kafe_lib_shared PRIVATE ${LIBGIT2_INCLUDE_DIR})
target_include_directories(kafe_lib_static PRIVATE ${LIBGIT2_INCLUDE_DIR})

string(TIMESTAMP KAFE_CMAKE_LIB_BUILD_TS "%Y-%m-%d" UTC)
target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_BUILD_TS="${KAFE_CMAKE_LIB_BUILD_TS}")
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_BUILD_TS="${KAFE_CMAKE_LIB_BUILD_TS}")

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_VERSION="${PROJECT_VERSION}")
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_VERSION="${PROJECT_VERSION}")

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_API_VERSION=${KAFE_API_LEVEL})
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_API_VERSION=${KAFE_API_LEVEL})

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_VERSION_INT=${KAFE_VERSION_INT})
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_VERSION_INT=${KAFE_VERSION_INT})

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_VERSION_MINOR=${PROJECT_VERSION_MINOR})

target_compile_definitions(kafe_lib_shared PUBLIC KAFE_CMAKE_LIB_VERSION_RELEASE=${PROJECT_VERSION_PATCH})
target_compile_definitions(kafe_lib_static PUBLIC KAFE_CMAKE_LIB_VERSION_RELEASE=${PROJECT_VERSION_PATCH})

find_path(KAFE_CMAKE_LIB_LUA_54 lua5.4/lua.h PATHS ${LUA_INCLUDE_DIR})
find_path(KAFE_CMAKE_LIB_LUA_53 lua5.3/lua.h PATHS ${LUA_INCLUDE_DIR})
find_path(KAFE_CMAKE_LIB_LUA_ROOT lua.h PATHS ${LUA_INCLUDE_DIR})

if (KAFE_CMAKE_LIB_LUA_54)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_LUA_54_VERSIONED)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_LUA_54_VERSIONED)

    target_include_directories(kafe_lib_shared PRIVATE ${KAFE_CMAKE_LIB_LUA_54})
    target_include_directories(kafe_lib_static PRIVATE ${KAFE_CMAKE_LIB_LUA_54})

    message(STATUS "Lua 5.4 referred to by versioned include path")
elseif (KAFE_CMAKE_LIB_LUA_53)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_LUA_53_VERSIONED)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_LUA_53_VERSIONED)

    target_include_directories(kafe_lib_shared PRIVATE ${KAFE_CMAKE_LIB_LUA_53})
    target_include_directories(kafe_lib_static PRIVATE ${KAFE_CMAKE_LIB_LUA_53})

    message(STATUS "Lua 5.3 referred to by versioned include path")
elseif (KAFE_CMAKE_LIB_LUA_ROOT)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_LUA_ROOT)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_LUA_ROOT)

    target_include_directories(kafe_lib_shared PRIVATE ${KAFE_CMAKE_LIB_LUA_ROOT})
    target_include_directories(kafe_lib_static PRIVATE ${KAFE_CMAKE_LIB_LUA_ROOT})

    message(STATUS "Lua referred to by root include path")
else ()
    message(FATAL_ERROR "Could not determine Lua header to include. Expected either <luaN.N/lua.hpp> or <lua.hpp>")
endif ()

if (_CXX_FILESYSTEM_HAVE_HEADER)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_FILESYSTEM)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_FILESYSTEM)
elseif (_CXX_FILESYSTEM_HAVE_EXPERIMENTAL_HEADER)
    target_compile_definitions(kafe_lib_shared PUBLIC KAFE_WITH_FILESYSTEM_EXPERIMENTAL)
    target_compile_definitions(kafe_lib_static PUBLIC KAFE_WITH_FILESYSTEM_EXPERIMENTAL)
else ()
    message(FATAL_ERROR "No filesystem library")
endif ()

include(GNUInstallDirs)
install(TARGETS kafe_lib_shared DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libkafe)
install(TARGETS kafe_lib_static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libkafe-dev)
install(FILES ${_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kafe COMPONENT libkafe-dev)
