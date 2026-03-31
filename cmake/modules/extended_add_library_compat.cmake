# Minimal compatibility shim for util-cmake's extended_add_library.
# Supports the argument forms used in this repository.

include(CMakeParseArguments)

function(extended_add_library)
  set(_options POSITION_INDEPENDENT INSTALL)
  set(_oneValueArgs NAME TYPE NAMESPACE INSTALL_DESTINATION)
  set(_multiValueArgs
    SOURCES
    LIBRARIES
    INCLUDE_DIRECTORIES
    SYSTEM_INCLUDE_DIRECTORIES
    COMPILE_DEFINITIONS
    DEPENDS
  )

  cmake_parse_arguments(EAL "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

  if(NOT EAL_NAME)
    message(FATAL_ERROR "extended_add_library: NAME is required")
  endif()

  if(EAL_NAMESPACE)
    set(_target "${EAL_NAMESPACE}-${EAL_NAME}")
  else()
    set(_target "${EAL_NAME}")
  endif()

  if(EAL_TYPE)
    add_library(${_target} ${EAL_TYPE} ${EAL_SOURCES})
  else()
    # Used for wrapper targets such as Singular in FindSingular.cmake
    add_library(${_target} INTERFACE)
  endif()

  if(EAL_NAMESPACE)
    add_library(${EAL_NAMESPACE}::${EAL_NAME} ALIAS ${_target})
  endif()

  if(EAL_INCLUDE_DIRECTORIES)
    target_include_directories(${_target} ${EAL_INCLUDE_DIRECTORIES})
  endif()

  if(EAL_SYSTEM_INCLUDE_DIRECTORIES)
    target_include_directories(${_target} SYSTEM ${EAL_SYSTEM_INCLUDE_DIRECTORIES})
  endif()

  if(EAL_COMPILE_DEFINITIONS)
    if(EAL_TYPE)
      target_compile_definitions(${_target} PRIVATE ${EAL_COMPILE_DEFINITIONS})
    else()
      target_compile_definitions(${_target} INTERFACE ${EAL_COMPILE_DEFINITIONS})
    endif()
  endif()

  if(EAL_LIBRARIES)
    if(EAL_TYPE)
      target_link_libraries(${_target} PRIVATE ${EAL_LIBRARIES})
    else()
      target_link_libraries(${_target} INTERFACE ${EAL_LIBRARIES})
    endif()
  endif()

  if(EAL_POSITION_INDEPENDENT)
    set_target_properties(${_target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()

  if(EAL_DEPENDS)
    add_dependencies(${_target} ${EAL_DEPENDS})
  endif()

  if(EAL_INSTALL)
    if(EAL_INSTALL_DESTINATION)
      install(TARGETS ${_target} DESTINATION ${EAL_INSTALL_DESTINATION})
    else()
      install(TARGETS ${_target})
    endif()
  endif()
endfunction()
