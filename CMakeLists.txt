cmake_minimum_required(VERSION 3.11.4)

# IMPORTANT: updating version might require update in package dependencies at the end of this file.
set(KAFE_VERSION "1.1.4")
set(KAFE_SOVERSION "1.1")
set(KAFE_VERSION_INT 11)
set(KAFE_VERSION_DEP_NEXT_MAJOR "2.0.0")

project(kafe_all VERSION ${KAFE_VERSION} LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FIND_LIBRARY_USE_LIB64_PATHS ON)

#if (NOT UNIX AND NOT APPLE)
if (NOT UNIX)
#    message(FATAL_ERROR "Not Unix or Apple, your operating system is not supported!")
    message(FATAL_ERROR "Not Unix - your operating system is not supported!")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "CMAKE IN DEBUG MODE")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "CMAKE IN RELEASE MODE")
endif ()

add_subdirectory(libkafe)
add_subdirectory(cli)

set(CPACK_PACKAGE_NAME "kafe")
set(CPACK_PACKAGE_VENDOR "Matiss Treinis")
set(CPACK_PACKAGE_CONTACT "mrtreinis@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "Kafe is an open source scriptable systems automation toolkit.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Kafe is an open source scriptable systems automation toolkit.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/libkafe/kafe")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/packaging/COPYRIGHT")

set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)

set(CPACK_COMPONENT_CLI_DESCRIPTION "Kafe command-line interface.")
set(CPACK_COMPONENT_LIBKAFE_DESCRIPTION "Kafe shared library (libkafe)")
set(CPACK_COMPONENT_LIBKAFE-DEV_DESCRIPTION "Kafe C++ development headers (libkafe)")

set(CPACK_COMPONENT_CLI_DEPENDS libkafe)
set(CPACK_COMPONENT_LIBKAFE-DEV_DEPENDS libkafe)

# TODO - darwin

# RPM
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
set(CPACK_RPM_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_LICENSE "Apache 2.0")
set(CPACK_RPM_CLI_PACKAGE_NAME "kafe-cli")
set(CPACK_RPM_LIBKAFE_PACKAGE_NAME "libkafe")
set(CPACK_RPM_LIBKAFE-DEV_PACKAGE_NAME "libkafe-devel")
set(CPACK_RPM_PACKAGE_AUTOREQ OFF)
set(CPACK_RPM_INSTALL_WITH_EXEC ON)
set(CPACK_RPM_LIBKAFE_PACKAGE_AUTOPROV ON)
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG")
set(CPACK_RPM_CLI_PACKAGE_REQUIRES "libkafe >= ${KAFE_VERSION}, libkafe < ${KAFE_VERSION_DEP_NEXT_MAJOR}, libstdc++ >= 4.8.1, glibc >= 2.0, libgcc >= 4.8.1")
set(CPACK_RPM_LIBKAFE_PACKAGE_REQUIRES "libstdc++ >= 4.8.1, glibc >= 2.0, libgcc >= 4.8.1, lua >= 5.3, libssh >= 0.7.1, libarchive >= 3, libcurl >= 7, libgit2 >= 0.24")
set(CPACK_RPM_LIBKAFE-DEV_PACKAGE_REQUIRES "libkafe = ${KAFE_VERSION}")

# DEB
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS OFF)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_CLI_PACKAGE_NAME "kafe-cli")
set(CPACK_DEBIAN_LIBKAFE_PACKAGE_NAME "libkafe")
set(CPACK_DEBIAN_LIBKAFE-DEV_PACKAGE_NAME "libkafe-dev")
set(CPACK_DEBIAN_CLI_PACKAGE_DEPENDS "libkafe (>=${KAFE_VERSION}), libkafe (<<${KAFE_VERSION_DEP_NEXT_MAJOR}), libstdc++6, libc6, libgcc1, libc6")
set(CPACK_DEBIAN_LIBKAFE_PACKAGE_DEPENDS "libstdc++6, libc6, libgcc1, libc6, liblua5.3-0, libssh-4 (>=0.7.0), libarchive13, libcurl3 | libcurl4, libgit2-24 | libgit2-26 | libgit2-27 | libgit2-28 | libgit2-1.1")
set(CPACK_DEBIAN_LIBKAFE-DEV_PACKAGE_DEPENDS "libkafe (=${KAFE_VERSION})")

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
include(CPack)
