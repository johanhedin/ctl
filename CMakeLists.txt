cmake_minimum_required(VERSION 2.8.12...3.10)

#
# Run with make VERBOSE=1 to see how things are build
#
project(ctl C)

set(CTL_VERSION_MAJOR 1)
set(CTL_VERSION_MINOR 0)
set(CTL_VERSION_PATCH 0)
set(CTL_VERSION_STRING ${CTL_VERSION_MAJOR}.${CTL_VERSION_MINOR}.${CTL_VERSION_PATCH})

set(PROJECT_VERSION ${CTL_VERSION_STRING})
set(PROJECT_DESCRIPTION "A cmake template library")

# Import our local cmake Modules, if any
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# We use C99 stuff
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Default build type is Debug. Change with cmake -DCMAKE_BUILD_TYPE=Release
# or cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo.
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type defaulting to Debug. Change to Relese or RelWithDebInfo with -DCMAKE_BUILD_TYPE=<choise>")
    set(CTL_TMP_BUILD_TYPE "Debug")
else()
   set(CTL_TMP_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif ()
set(CMAKE_BUILD_TYPE "${CTL_TMP_BUILD_TYPE}" CACHE STRING "Choose the type of build. Options are: Debug, Release or RelWithDebInfo" FORCE)

# Default is to build libctl as a static library. Change to build a shared lilbrary with -DCTL_SHARED_LIBS=ON
if(NOT DEFINED CTL_SHARED_LIBS)
    message(STATUS "Defaulting to building libctl as a static library. Change to shared with -DCTL_SHARED_LIBS=ON")
    set(CTL_SHARED_LIBS_TMP OFF)
else()
    set(CTL_SHARED_LIBS_TMP ${CTL_SHARED_LIBS})
endif()
set(CTL_SHARED_LIBS ${CTL_SHARED_LIBS_TMP} CACHE BOOL "Build libctl as a shared library" FORCE)
set(BUILD_SHARED_LIBS ${CTL_SHARED_LIBS})

# We need the PkgConfig cmake module to be able to find libraries we depend on
find_package(PkgConfig REQUIRED)

# Use GNUInstallDirs to get standard install directories for include, lib, bin and so on
include(GNUInstallDirs)

# Define the target for our library
add_library(ctl "${PROJECT_SOURCE_DIR}/src/ctl.c")
target_include_directories(ctl PRIVATE "${PROJECT_SOURCE_DIR}/include")

# Add compile and link options to our target
target_compile_options(ctl PRIVATE -Wall)
target_compile_options(ctl PRIVATE -Wextra)
target_compile_options(ctl PRIVATE -Wpedantic)
target_compile_options(ctl PRIVATE -fanalyzer)
target_compile_options(ctl PRIVATE -Wanalyzer-too-complex)
#target_compile_options(ctl PRIVATE -pg)
#target_link_options(ctl PRIVATE -pg)

# Set properties on our target
set_target_properties(ctl PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/ctl.h")
set_target_properties(ctl PROPERTIES VERSION ${CTL_VERSION_STRING})
set_target_properties(ctl PROPERTIES SOVERSION ${CTL_VERSION_MAJOR})

# Add additional libraries that we depend on
pkg_check_modules(JANSSON REQUIRED jansson>=2)
target_include_directories(ctl PRIVATE ${JANSSON_INCLUDE_DIRS})
target_link_libraries(ctl PRIVATE ${JANSSON_LIBRARIES})

# Install options
install(TARGETS ctl LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
                    ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
                    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/ctl")
if(CTL_SHARED_LIBS)
    configure_file("${PROJECT_SOURCE_DIR}/pkgcfg/libctl.pc.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/libctl.pc" @ONLY)
    install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/libctl.pc"
            DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
else()
    configure_file("${PROJECT_SOURCE_DIR}/pkgcfg/libctl-static.pc.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/libctl-static.pc" @ONLY)
    install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/libctl-static.pc"
            DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
endif()

# Create a uninstall target if it does not already exist
if(NOT TARGET uninstall)
    configure_file("${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Dump our config for review
message(STATUS "The following build options will be used for libctl:")
message(STATUS "    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "    BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
message(STATUS "    CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "    CMAKE_INSTALL_FULL_INCLUDEDIR=${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LIBDIR=${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_BINDIR=${CMAKE_INSTALL_FULL_BINDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_SYSCONFDIR=${CMAKE_INSTALL_FULL_SYSCONFDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DATAROOTDIR=${CMAKE_INSTALL_FULL_DATAROOTDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LOCALSTATEDIR=${CMAKE_INSTALL_FULL_LOCALSTATEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DOCDIR=${CMAKE_INSTALL_FULL_DOCDIR}")
