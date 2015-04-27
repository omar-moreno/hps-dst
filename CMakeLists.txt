#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#

# Specify the minimum CMake version required.  If the cmake version 
# minimum is not met, cmake will fail.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set the name of the project
project(HPS_DST)

# Set the project version
set(HPS_DST_VERSION_MAJOR 0)
set(HPS_DST_VERSION_MINOR 6)

# Set the output directories of the binaries, static and shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/lib")

# Set the path where additional CMake modules are located
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# Find ROOT
find_package(ROOT REQUIRED)

# Include the ROOT headers
include_directories(${ROOT_INCLUDE_DIRS})

# Include the LCIO headers
include_directories("$ENV{LCIO}/include")

# Find the LCIO libraries
find_library(LCIO_LIB lcio $ENV{LCIO}/lib)
if(NOT LCIO_LIB)
	message(FATAL_ERROR
		"\nThe library liblcio was not found.  Either LCIO hasn't been "
		"installed or the environmental variable LCIO has not been set. "
		"See documentation for details.")
endif()
message(STATUS "LCIO: ${LCIO_LIB}")

# Include the HPS Event headers
set(HPS_EVENT_HDR_DIR "${HPS_DST_SOURCE_DIR}/include/hps_event")
include_directories("${HPS_EVENT_HDR_DIR}")

# Include DST headers
set(DST_HDR_DIR "${HPS_DST_SOURCE_DIR}/include/dst")
include_directories("${DST_HDR_DIR}")

# Include Utils headers
set(UTILS_HDR_DIR "${HPS_DST_SOURCE_DIR}/include/utils")
include_directories("${UTILS_HDR_DIR}")

# Find GBL.
set(GBL_DIR "$ENV{GBL_DIR}")
find_package(GBL QUIET)

if(NOT GBL_FOUND)
    message(FATAL_ERROR 
        "\nGBL wasn't found!  Either GBL hasn't been installed or the " 
        "variable GBL_DIR has not been set. See documentation for details.")
endif()

# include the GBL headers
include_directories("${GBL_INCLUDE_DIR}")

#--------------------------------------------#
#--- Create the HPS Event ROOT dictionary ---#
#--------------------------------------------#

message(STATUS "Generating the HpsEvent ROOT dictionary")

# Check if rootcint exist, otherwise exit
if(NOT ROOT_CINT_EXECUTABLE)
	message(FATAL_ERROR 
        "rootcint was not found, make sure ROOTSYS/bin is "
        "include in the path")
endif()

# Set the output directory for the ROOT dictionary and make sure it exists
set(HPS_EVENT_DIC_DIR "${HPS_DST_BINARY_DIR}/src/root_dic")
file(MAKE_DIRECTORY ${HPS_EVENT_DIC_DIR})

file(GLOB HPS_EVENT_HDR "${HPS_EVENT_HDR_DIR}/*.h")
list(REMOVE_ITEM HPS_EVENT_HDR "${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h")
execute_process(COMMAND rootcint -f ${HPS_EVENT_DIC_DIR}/HpsEventDic.cxx -c ${HPS_EVENT_HDR} "${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h")

message(STATUS "HpsEvent ROOT dictionary has been generated")

# Add the hps_event subdirectory to the build
add_subdirectory("${HPS_DST_SOURCE_DIR}/src/hps_event")

# Add the utils directory to the build
add_subdirectory("${HPS_DST_SOURCE_DIR}/src/utils")

# Add dst directory to the build
add_subdirectory("${HPS_DST_SOURCE_DIR}/src/dst")

#------------------------------#
#--- Generate Documentation ---#
#------------------------------#

# Find Doxygen
find_package(Doxygen)

# If Doxygen is found, add a target to generate the documentation
if(DOXYGEN_FOUND)
	configure_file(${HPS_DST_SOURCE_DIR}/doc/Doxyfile ${HPS_DST_BINARY_DIR}/doc/Doxyfile @ONLY)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${HPS_DST_BINARY_DIR}/doc/Doxyfile WORKING_DIRECTORY ${HPS_DST_SOURCE_DIR}/doc COMMENT "Generating documentation" VERBATIM)
else()
	MESSAGE(STATUS "Doxygen was not found.  Documentation will not be generated.")
endif(DOXYGEN_FOUND)

#-------------------------------------------------#
#--- Add a custom target to build the examples ---#
#-------------------------------------------------#
add_custom_target(examples COMMENT "HPS DST examples have been built")
set(EXAMPLES_DIR "${HPS_DST_SOURCE_DIR}/examples")
add_subdirectory("${EXAMPLES_DIR}")
file(GLOB EXAMPLES_SRC "${EXAMPLES_DIR}/*.cxx")
foreach(EXAMPLE ${EXAMPLES_SRC})
	message(STATUS "Found source: ${EXAMPLE}")
	get_filename_component(EXAMPLE_NAME "${EXAMPLE}" NAME_WE)
	message(STATUS "Source name: ${EXAMPLE_NAME}")
	add_dependencies(examples "${EXAMPLE_NAME}")
endforeach(EXAMPLE)
