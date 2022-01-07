########################################################################
# CMake build script for Plain Framework.
#
# To run the tests for Plain Framework itself on Linux, use 'make test' or
# ctest.  You can select which tests to run using 'ctest -R regex'.
# For more options, run 'ctest --help'.

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# When other libraries are using a shared version of runtime libraries,
# Plain Framework also has to use one.
option(
  pf_plugin_force_shared_crt
  "Use shared (DLL) run-time lib even when Plain Framework is built as static lib."
  OFF)

option(pf_plugin_build_tests "Build all of pf's own tests." OFF)

option(pf_plugin_build_shared "Build all of pf as share library." OFF)

option(pf_plugin_build_samples "Build pf's sample programs." OFF)

option(pf_plugin_disable_pthreads "Disable uses of pthreads in pf." OFF)

option(
  pf_plugin_hide_internal_symbols
  "Build pf with internal symbols hidden in shared libraries."
  OFF)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Generate debug library name with a postfix.")

# Defines pre_project_set_up_hermetic_build() and set_up_hermetic_build().
include(inc/common.cmake OPTIONAL)
include(inc/hermetic_build.cmake OPTIONAL)

if (COMMAND pre_project_set_up_hermetic_build)
  pre_project_set_up_hermetic_build()
endif()

########################################################################
#
# Project-wide settings

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${root_dir} and to the root binary directory as
# ${pf_plugin_BINARY_DIR}.
# Language "C" is required for find_package(Threads).
set(PF_PLUGIN_VERSION 1.0.0)
if (CMAKE_VERSION VERSION_LESS 3.0)
  project(pf_plugin CXX C)
else()
  cmake_policy(SET CMP0048 NEW)
  project(pf_plugin VERSION ${PF_PLUGIN_VERSION} LANGUAGES CXX C)
endif()
cmake_minimum_required(VERSION 2.8.12)

if (POLICY CMP0063) # Visibility
  cmake_policy(SET CMP0063 NEW)
endif (POLICY CMP0063)

if (COMMAND set_up_hermetic_build)
  set_up_hermetic_build()
endif()

if (pf_plugin_hide_internal_symbols)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

# Define helper functions and macros used by Plain Framework.
include(inc/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

set(lua_version 5.4.3)

# Where Plain Framework's .h files can be found.
include_directories(
  "${plainframework_dir}/include"
  "${root_dir}/lua/include"
  "${root_dir}/odbc/include"
  "${root_dir}/protocol/include"
  "${root_dir}/dependencies/lua/${lua_version}/src"
  "/usr/local/unixODBC/include")

# Summary of tuple support for Microsoft Visual Studio:
# Compiler    version(MS)  version(cmake)  Support
# ----------  -----------  --------------  -----------------------------
# <= VS 2010  <= 10        <= 1600         Use Plain Frameworks's own tuple.
# VS 2012     11           1700            std::tr1::tuple + _VARIADIC_MAX=10
# VS 2013     12           1800            std::tr1::tuple
# VS 2015     14           1900            std::tuple
# VS 2017     15           >= 1910         std::tuple
if (MSVC AND MSVC_VERSION EQUAL 1700)
  add_definitions(/D _VARIADIC_MAX=10)
endif()

########################################################################
#
# Defines the pf_plugin * libraries.  User tests should link
# with one of them.

set(pf_plugin_build_shared ON)
set(INSTALL_PF_PLUGIN ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined")
endif()

#For utf8 no boom.
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4819")
endif()

if (NOT has_output_path)

  # This is the directory into which the executables are built.
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${root_dir}/bin)

  # This is the directory into which the librarys are built.
  set(LIBRARY_OUTPUT_PATH ${root_dir}/lib)

  set(has_output_path 1)

endif()

#The lua plugin with lua code.
file(GLOB_RECURSE LIB_DEPS_LUA_SOURCES "${root_dir}/dependencies/lua/${lua_version}/src/*.c")
list(REMOVE_ITEM LIB_DEPS_LUA_SOURCES "${root_dir}/dependencies/lua/${lua_version}/src/lua.c")
list(REMOVE_ITEM LIB_DEPS_LUA_SOURCES "${root_dir}/dependencies/lua/${lua_version}/src/luac.c")
file(GLOB_RECURSE LIB_LUA_SOURCES "${root_dir}/lua/src/*.cc")
list(APPEND LIB_LUA_SOURCES ${LIB_DEPS_LUA_SOURCES})

file(GLOB_RECURSE LIB_ODBC_SOURCES "${root_dir}/odbc/src/*.cc")

file(GLOB_RECURSE LIB_PROTOCOL_SOURCES "${root_dir}/protocol/src/*.cc")

# Plain Framework plugin libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that pf can be compiled by a user
# aggressive about warnings.
if (pf_plugin_build_shared)

  string(REPLACE "-pedantic" "" cxx_default "${cxx_default}")
  cxx_shared_library(pf_plugin_lua "${cxx_default}" "" ${LIB_LUA_SOURCES})
  set(cxx_default "${cxx_default} -pedantic")

  cxx_shared_library(pf_plugin_odbc "${cxx_default}" "odbc" ${LIB_ODBC_SOURCES})
  cxx_shared_library(pf_plugin_protocol "${cxx_default}" "" ${LIB_PROTOCOL_SOURCES})
else()

  string(REPLACE "-pedantic" "" cxx_default "${cxx_default}")
  cxx_library(pf_plugin_lua "${cxx_strict}" ${LIB_LUA_SOURCES})
  set(cxx_default "${cxx_default} -pedantic")

  cxx_library(pf_plugin_odbc "${cxx_strict}" ${LIB_ODBC_SOURCES})
  cxx_library(pf_plugin_protocol "${cxx_strict}" ${LIB_PROTOCOL_SOURCES})
endif()

# If the CMake version supports it, attach header directory information
# to the targets for when we are part of a parent build (ie being pulled
# in via add_subdirectory() rather than being a standalone build).
if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.12")
  #target_include_directories(pf_plugin_core      SYSTEM INTERFACE "${root_dir}/include")
endif()

########################################################################
#
# Install rules
if(INSTALL_PF_PLUGIN)

  install(TARGETS pf_plugin_lua
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

  install(TARGETS pf_plugin_odbc
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

  install(TARGETS pf_plugin_protocol
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

  install(DIRECTORY "${root_dir}/lua/include/pf"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(DIRECTORY "${root_dir}/odbc/include/pf"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(DIRECTORY "${root_dir}/protocol/include/pf"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  # configure and install pkgconfig files
  configure_file(
    inc/pf_plugin.pc.in
    "${CMAKE_BINARY_DIR}/pf_plugin.pc"
    @ONLY)
  install(FILES "${CMAKE_BINARY_DIR}/pf_plugin.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pf_plugin_config")
endif()

########################################################################
#
# Samples on how to link user tests with pf or pf_plugin_*.
#
# They are not built by default.  To build them, set the
# pf_plugin_build_samples option to ON.  You can do it by running ccmake
# or specifying the -Dpf_plugin_build_samples=ON flag when running cmake.

if (pf_plugin_build_samples)

endif()

########################################################################
#
# Plain Framework's own tests.
#
# You can skip this section if you aren't interested in testing
# Plain Framework itself.
#
# The tests are not built by default.  To build them, set the
# pf_plugin_build_tests option to ON.  You can do it by running ccmake
# or specifying the -Dpf_plugin_build_tests=ON flag when running cmake.

if (pf_plugin_build_tests)
  # This must be set in the root directory for the tests to be run by
  # 'make test' or ctest.
  enable_testing()

  ############################################################
  # C++ tests built with standard compiler flags.

  #cxx_test(pf-death-test_test pf_plugin_main)

  ############################################################
  # C++ tests built with non-standard compiler flags.

  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)

  endif()


  if (NOT MSVC OR MSVC_VERSION LESS 1600)  # 1600 is Visual Studio 2010.
    # Visual Studio 2010, 2012, and 2013 define symbols in std::tr1 that
    # conflict with our own definitions. Therefore using our own tuple does not
    # work on those compilers.

  endif()

  ############################################################
  # Python tests.

  # Visual Studio .NET 2003 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)  # 1310 is Visual Studio .NET 2003

  endif()

  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)

  endif()

endif()
