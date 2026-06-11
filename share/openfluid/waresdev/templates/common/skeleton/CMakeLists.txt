CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT("%%WAREID%%")

FIND_PACKAGE(OpenFLUID REQUIRED)

# Optional file dedicated for automatic cmake configuration, loaded if CONTEXT_VARS is ON
OPTION(CONTEXT_VARS "Using contextual cmake variables" OFF)
IF (CONTEXT_VARS)
  MESSAGE(STATUS "Using contextual variables (warning: may be overwritten by subsequent cmake line)")
  INCLUDE(CMake.in.contextual.config OPTIONAL)
ENDIF()
# For manual adjustments use a CMake.in.local.config file and include it here

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(tests)

