cmake_minimum_required(VERSION 2.8.12...3.10)

#
# Run with make VERBOSE=1 to see how things are build
# configure_file as a build step: https://cmake.org/pipermail/cmake/2012-May/050221.html
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

# Build type defaults to Debug. Change to Release or RelWithDebInfo with cmake -DCMAKE_BUILD_TYPE=<choise>
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type defaulting to Debug. Change to Relese or RelWithDebInfo with -DCMAKE_BUILD_TYPE=<choise>")
    set(CTL_TMP_BUILD_TYPE "Debug")
else()
   set(CTL_TMP_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif ()
set(CMAKE_BUILD_TYPE "${CTL_TMP_BUILD_TYPE}" CACHE STRING "Choose the type of build. Options are: Debug, Release or RelWithDebInfo" FORCE)

# Default to link libctl as static in-tree. Change to installable static or shared
# library with -DCTL_LINK_TYPE={Static,Shared}
if(NOT DEFINED CTL_LINK_TYPE)
    message(STATUS "Linking of libctl defaulting to static Intree. Change to 'Static' or 'Shared' with -DCTL_LINK_TYPE=<choise>")
    set(CTL_LINK_TYPE_TMP "Intree")
else()
    set(CTL_LINK_TYPE_TMP ${CTL_LINK_TYPE})
endif()
set(CTL_LINK_TYPE ${CTL_LINK_TYPE_TMP} CACHE STRING "How to link libctl. Options are: Intree, Static or Shared" FORCE)
if(CTL_LINK_TYPE STREQUAL "Shared")
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

# We need the PkgConfig cmake module to be able to find libraries we depend on
find_package(PkgConfig REQUIRED)

# Use GNUInstallDirs to get standard install directories for include, lib, bin and so on
include(GNUInstallDirs)

# Define the target for our library
add_library(ctl "${PROJECT_SOURCE_DIR}/src/ctl.c")
target_include_directories(ctl PRIVATE "${PROJECT_SOURCE_DIR}/include")

# Add compile and link options to our target. We differentiate between gcc and clang
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_VERSION VERSION_LESS "3.1")
        target_compile_options(ctl PRIVATE -std=gnu${CMAKE_C_STANDARD})
    endif()
    target_compile_options(ctl PRIVATE -Wall)
    target_compile_options(ctl PRIVATE -Wextra)
    target_compile_options(ctl PRIVATE -pedantic)
    #target_compile_options(ctl PRIVATE -pg)
    #target_link_options(ctl PRIVATE -pg)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_VERSION VERSION_LESS "3.1")
        target_compile_options(ctl PRIVATE -std=gnu${CMAKE_C_STANDARD})
    endif()
    target_compile_options(ctl PRIVATE -Wall)
    target_compile_options(ctl PRIVATE -Wextra)
    target_compile_options(ctl PRIVATE -Wpedantic)
else()
endif()

# Set properties on our target
set_target_properties(ctl PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/ctl.h")
set_target_properties(ctl PROPERTIES VERSION        ${CTL_VERSION_STRING})
set_target_properties(ctl PROPERTIES SOVERSION      ${CTL_VERSION_MAJOR})

# Add additional libraries that we depend on
pkg_check_modules(JANSSON REQUIRED jansson>=2)
target_include_directories(ctl PRIVATE ${JANSSON_INCLUDE_DIRS})
target_link_libraries(ctl PRIVATE ${JANSSON_LIBRARIES})

if(NOT CTL_LINK_TYPE STREQUAL "Intree")
    # Install options
    install(TARGETS ctl LIBRARY       DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
                        ARCHIVE       DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
                        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/${PROJECT_NAME}")

    # Create empty install_manifest.txt so that it does not get owned by root on sudo make install
    file(COPY "${PROJECT_SOURCE_DIR}/cmake/install_manifest.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

    # Different pkg-config files depending on static or shared
    if(CTL_LINK_TYPE STREQUAL "Shared")
        configure_file("${PROJECT_SOURCE_DIR}/pkgcfg/lib${PROJECT_NAME}.pc.in"
                       "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.pc" @ONLY)
        install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.pc"
                DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
    else()
        configure_file("${PROJECT_SOURCE_DIR}/pkgcfg/lib${PROJECT_NAME}-static.pc.in"
                       "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}-static.pc" @ONLY)
        install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}-static.pc"
                DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
    endif()

    # Create a uninstall target if it does not already exist
    if(NOT TARGET uninstall)
        configure_file("${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                       "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
        add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    endif()
endif()

# Dump our config for review
message(STATUS "The following build options will be used for libctl:")
message(STATUS "    CTL_LINK_TYPE=${CTL_LINK_TYPE}")
message(STATUS "    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "    CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID}")
message(STATUS "    CMAKE_C_COMPILER_VERSION=${CMAKE_C_COMPILER_VERSION}")
if(NOT CTL_LINK_TYPE STREQUAL "Intree")
message(STATUS "    BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
message(STATUS "    CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "    CMAKE_INSTALL_FULL_INCLUDEDIR=${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LIBDIR=${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_BINDIR=${CMAKE_INSTALL_FULL_BINDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_SYSCONFDIR=${CMAKE_INSTALL_FULL_SYSCONFDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DATAROOTDIR=${CMAKE_INSTALL_FULL_DATAROOTDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_LOCALSTATEDIR=${CMAKE_INSTALL_FULL_LOCALSTATEDIR}")
message(STATUS "    CMAKE_INSTALL_FULL_DOCDIR=${CMAKE_INSTALL_FULL_DOCDIR}")
endif()
