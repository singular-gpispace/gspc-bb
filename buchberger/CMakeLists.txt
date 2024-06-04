extended_add_library (NAME buchberger_implementation
  TYPE MODULE
  SOURCES "implementation.cpp" "singular_functions.cpp"
  INCLUDE_DIRECTORIES PRIVATE "${CMAKE_BINARY_DIR}" "${CMAKE_BINARY_DIR}/gen" "${CMAKE_BINARY_DIR}/buchberger"
  COMPILE_DEFINITIONS SING_NDEBUG OM_NDEBUG
  LIBRARIES GPISpace::workflow_development Singular
  DEPENDS buchberger.pnet_and_wrapper-library Config
  INSTALL INSTALL_DESTINATION "libexec/workflow"
)
set_target_properties (buchberger_implementation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

install (FILES LIB/buchbergergspc.lib
  DESTINATION "${CMAKE_INSTALL_PREFIX}" 
)
