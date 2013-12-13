#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#

# Find the necessary LCIO libraries
find_library(LCIO_DICT_LIB lcioDict $ENV{LCIO}/lib)
find_library(LCIO_LIB lcio $ENV{LCIO}/lib)

# Add the dump_hps_event executable
add_executable(dump_hps_event dump_hps_event.cxx)
target_link_libraries(dump_hps_event ${LCIO_DICT_LIB} ${LCIO_LIB})

# Find the required ROOT libraries
find_library(ROOT_TREE_LIB Tree $ENV{ROOTSYS}/lib)
find_library(ROOT_CORE_LIB Core $ENV{ROOTSYS}/lib)
find_library(ROOT_RIO_LIB RIO $ENV{ROOTSYS}/lib)
find_library(ROOT_PHYSICS_LIB Physics $ENV{ROOTSYS}/lib)

# Add the EcalUtils library
include_directories(.)
add_library(EcalUtils EcalUtils.cxx)
target_link_libraries(EcalUtils ${ROOT_PHYSICS_LIB})

# Add the write_hps_event executable
add_executable(write_hps_event write_hps_event.cxx)
target_link_libraries(write_hps_event ${LCIO_LIB} ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} HpsEvent ${ROOT_RIO_LIB} EcalUtils)
