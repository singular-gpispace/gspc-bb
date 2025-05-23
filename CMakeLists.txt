cmake_minimum_required (VERSION 3.16)
project (singular-buchberger LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set (INSTALL_DO_NOT_BUNDLE ON CACHE BOOL "")

find_package (GPISpace REQUIRED)

find_package (util-generic REQUIRED)
find_package (util-cmake REQUIRED)

include (util-cmake/add_macros)

include (util-cmake/beautify_find_boost)
include (util-cmake/beautify_find_GPISpace)

find_boost (1.61 REQUIRED QUIET FROM_GPISPACE_INSTALLATION COMPONENTS
  date_time
  filesystem
  program_options
  regex
  serialization
  system
  test_exec_monitor
  thread
  unit_test_framework
  timer
  iostreams
  chrono
)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
find_package (Singular REQUIRED)

#set (BUNDLE_ROOT "libexec/bundle")
#bundle_GPISpace (DESTINATION "${BUNDLE_ROOT}/gpispace" COMPONENTS "runtime" "monitoring")

set (SP_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}")

include_directories ("src")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories ("${CMAKE_CURRENT_BINARY_DIR}/src")

add_subdirectory (src)
add_subdirectory (buchberger)
add_subdirectory (workflow)

configure_file("buchberger/config.hpp.in"
  "${CMAKE_BINARY_DIR}/buchberger/config.hpp"
  @ONLY
)

add_custom_target (Config
  ALL
  DEPENDS "${CMAKE_BINARY_DIR}/buchberger/config.hpp"
)

#install (DIRECTORY "${SINGULAR_HOME}/"
#  DESTINATION "${BUNDLE_ROOT}/singular"
#  USE_SOURCE_PERMISSIONS
#  PATTERN "share/singular/html/*" EXCLUDE
#)

install (DIRECTORY examples
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share"
)

