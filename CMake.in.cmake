#
# Configuration file for CMakeLists.txt files
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
# This file is included by the main CMakeLists.txt file, and defines variables
# to configure the build and install 
#
# The variables in this file can also be overriden through the  
# CMake.in.local.config file 
#


################### general ###################

SET(OPENFLUID_MAIN_NAME "openfluid")

SET(OPENFLUID_RELATIVEDIR "${OPENFLUID_MAIN_NAME}")

SET(OPENFLUID_DEFAULT_CONFIGFILE "openfluid.conf")

SET(OPENFLUID_INPUTDIR "OPENFLUID.IN")
SET(OPENFLUID_OUTPUTDIR "OPENFLUID.OUT")
SET(OPENFLUID_FUNCSDIR "functions")

SET(OPENFLUID_FUNCSMAXNUMTHREADS "4")

SET(OPENFLUID_PROJECT_FILE ".openfluidprj")
SET(OPENFLUID_PROJECT_INPUTDIR "IN")
SET(OPENFLUID_PROJECT_OUTPUTDIRPREFIX "OUT")


################### install path ###################

SET(BIN_INSTALL_PATH "bin")
SET(LIB_INSTALL_PATH "lib")
SET(INCLUDE_INSTALL_PATH "include")
SET(SHARE_INSTALL_PATH "share")
SET(INCLUDE_OPENFLUID_INSTALL_PATH "${INCLUDE_INSTALL_PATH}/openfluid")
SET(SHARE_OPENFLUID_INSTALL_PATH "${SHARE_INSTALL_PATH}/openfluid")
SET(SHARE_DESKTOPENTRY_INSTALL_PATH "${SHARE_INSTALL_PATH}/applications")

SET(SHARE_COMMON_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/common")
SET(SHARE_APPS_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/apps")
SET(FUNCTIONS_INSTALL_PATH "lib/openfluid/functions")
SET(BUILDEREXT_INSTALL_PATH "lib/openfluid/builderext")

SET(PKGCONFIG_INSTALL_PATH "lib/pkgconfig")

SET(DOC_INSTALL_PATH "${SHARE_INSTALL_PATH}/doc/openfluid")
SET(API_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/API")
SET(FUNCTIONS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/functions")
SET(EXAMPLES_INSTALL_PATH "${DOC_INSTALL_PATH}/examples")
SET(EXAMPLES_SRC_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/src")
SET(EXAMPLES_DATA_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/datasets")
SET(KERNEL_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}")
SET(MANUALS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/manuals")
SET(MANUALS_DOCPDF_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/pdf")
SET(MANUALS_DOCHTML_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/html")
SET(EXAMPLES_DOCPDF_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}")

################### source and output path ###################

SET(BUILD_OUTPUT_PATH "${CMAKE_BINARY_DIR}/dist")

SET(LIB_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${LIB_INSTALL_PATH}")
SET(FUNCTIONS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${FUNCTIONS_INSTALL_PATH}")
SET(BUILDEREXT_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BUILDEREXT_INSTALL_PATH}")
SET(BIN_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BIN_INSTALL_PATH}")
SET(PKGCONFIG_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${PKGCONFIG_INSTALL_PATH}")

SET(APIDOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${API_DOC_INSTALL_PATH}")
SET(APIDOC_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/openfluid")
SET(APIDOC_DOCFILES_DIR "${CMAKE_SOURCE_DIR}/doc/API-doxygen")

SET(DOCFORDEV_OUTPUT_PATH "${CMAKE_BINARY_DIR}/docfordev")
SET(DOCFORDEV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

SET(FUNCTIONS_DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${FUNCTIONS_DOC_INSTALL_PATH}")

SET(DOC_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources/doc")
SET(DOC_BUILD_DIR "${CMAKE_BINARY_DIR}/docdir-for-build")
SET(MANUALS_DOC_BUILD_DIR "${DOC_BUILD_DIR}/manuals")
SET(MANUALS_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCPDF_INSTALL_PATH}")
SET(MANUALS_DOCHTML_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCHTML_INSTALL_PATH}")

SET(EXAMPLES_DOC_BUILD_DIR "${DOC_BUILD_DIR}/examples")
SET(EXAMPLES_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${EXAMPLES_DOCPDF_INSTALL_PATH}")
SET(EXAMPLES_DATASETS_PATH "${CMAKE_SOURCE_DIR}/resources/examples/datasets")


SET(OPENFLUID_NLS_LOCALEPATH "${SHARE_INSTALL_PATH}/locale")
SET(PO_BUILD_DIR "${BUILD_OUTPUT_PATH}/${SHARE_INSTALL_PATH}/locale")

################### test paths ###################

SET(TEST_OUTPUT_PATH "${CMAKE_BINARY_DIR}/tests-bin")
SET(TESTS_DATASETS_PATH "${CMAKE_SOURCE_DIR}/resources/tests/datasets")
SET(TESTS_OUTPUTDATA_PATH "${CMAKE_BINARY_DIR}/tests-output")


################### versions ###################

SET(CUSTOM_CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 7)
SET(VERSION_PATCH 1)
#SET(VERSION_STATUS "rc4") # example: SET(VERSION_STATUS "rc1")

SET(FULL_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

IF(VERSION_STATUS)
  SET(FULL_VERSION "${FULL_VERSION}~${VERSION_STATUS}")
ENDIF(VERSION_STATUS)


################### compilation and build ###################
IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC) 
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")  
ENDIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

SET(FUNCTIONS_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}mpi")

SET(DEBUG_PREFIX "[OpenFLUID debug]")


################### applications build ###################

# set this to 1 to build openfluid-engine
SET(BUILD_APP_ENGINE 1)

# set this to 1 to build openfluid-builder
SET(BUILD_APP_BUILDER 1)

# set this to 1 to build openfluid-market-client
SET(BUILD_APP_MARKETCLIENT 0)


################### functions build ###################

# uncomment this to build functions in foundry directory
#SET(BUILD_FUNCTIONS_FOUNDRY 1)

# uncomment this to build functions mixing C++ and fortran source codes (in this source tree)
#SET(BUILD_FORTRAN_FUNCTIONS 1)


################### latex build ###################

# uncomment this to build latex docs
#SET(BUILD_LATEX_DOCS 1)


################### docs for developers build ###################

#SET(BUILD_DOCFORDEV 1)


################### tests to run ###################

#SET(TESTS_RUNHIGHLOAD 1)


################### market ###################

SET(OPENFLUID_MARKETBAGDIR "market-bag")
SET(OPENFLUID_MARKETPLACE_SITEFILE "OpenFLUID-Marketplace")
SET(OPENFLUID_MARKETPLACE_CATALOGFILE "Catalog")

SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-DNOFUNC2DOC=1")

IF(WIN32)
  SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-G \\\"${CMAKE_GENERATOR}\\\" ${OPENFLUID_MARKET_COMMONBUILDOPTS}")
ENDIF(WIN32)


################### i18n ###################

# Native Language Support
SET(OPENFLUID_NLS_ENABLE 1)

SET(OPENFLUID_NLS_PACKAGE openfluid)
SET(PO_DIR po)
SET(PO_PACKAGENAME ${OPENFLUID_NLS_PACKAGE})
SET(PO_COPYRIGHTHOLDER "INRA-Montpellier SupAgro")
SET(PO_BUGSADDRESS "libres at supagro.inra.fr")


################### builder ###################


SET(BUILDEREXTENSION_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}bepi")
SET(BUILDER_EXTSDIR "builder-extensions")



