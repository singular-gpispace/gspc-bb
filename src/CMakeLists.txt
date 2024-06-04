configure_file("installation.hpp.in" "installation.hpp" @ONLY)

extended_add_library (NAME installation
  TYPE SHARED
  NAMESPACE SINGULAR-buchberger
  SOURCES "installation.cpp"
  LIBRARIES Util::Generic GPISpace::execution
  POSITION_INDEPENDENT
  INSTALL
)
set_target_properties (SINGULAR-buchberger-installation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

extended_add_library (NAME module
  TYPE SHARED
  NAMESPACE SINGULAR-buchberger
  SOURCES "bin/SINGULAR-buchberger-module.cpp"    
          "bin/execute.cpp" 
          "bin/parse_parameters_from_singular.cpp" 
          "bin/ValuesOnPorts.cpp" 
          "bin/Workflow.cpp" 
          "bin/WorkflowResult.cpp"  
          "${CMAKE_SOURCE_DIR}/buchberger/singular_functions.cpp" 
  INCLUDE_DIRECTORIES PRIVATE "${CMAKE_SOURCE_DIR}"
  COMPILE_DEFINITIONS SING_NDEBUG OM_NDEBUG
  LIBRARIES SINGULAR-buchberger::installation Util::Generic GPISpace::execution Singular
  POSITION_INDEPENDENT
  INSTALL
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
set_target_properties (SINGULAR-buchberger-module
  PROPERTIES
  INSTALL_RPATH "$ORIGIN/lib"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)
