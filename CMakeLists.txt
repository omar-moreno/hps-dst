#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 2.6)

# Set the name of the project
project(HPS_DST)

# Set the current version
set(HPS_DST_VERSION_MAJOR 1)
set(HPS_DST_VERSION_MINOR 0)

# Set the output directories of the binaries, static and shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/.obj")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/.obj")

# Include the HPS Event headers
set(HPS_EVENT_HDR_DIR "${HPS_DST_SOURCE_DIR}/include/hps_event")
include_directories("${HPS_EVENT_HDR_DIR}")

# Include the ROOT headers
include_directories("$ENV{ROOTSYS}/include")

# Include the LCIO headers
include_directories("$ENV{LCIO}/include")

# Create the HPS Event ROOT dictionary
set(HPS_EVENT_DIC_DIR "${HPS_DST_BINARY_DIR}/src/root_dic")
file(MAKE_DIRECTORY ${HPS_EVENT_DIC_DIR})
file(GLOB HPS_EVENT_HDR "${HPS_EVENT_HDR_DIR}/*.h")
list(REMOVE_ITEM HPS_EVENT_HDR "${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h")
execute_process(COMMAND rootcint -f ${HPS_EVENT_DIC_DIR}/HpsEventDic.cxx -c ${HPS_EVENT_HDR} "${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h")

# Add the hps_event subdirectory to the build
add_subdirectory("${HPS_DST_SOURCE_DIR}/src/hps_event")

# Add the utils directory to the build
add_subdirectory("${HPS_DST_SOURCE_DIR}/src/utils")

