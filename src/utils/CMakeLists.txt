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
add_library(EcalUtils EcalUtils.cxx)
target_link_libraries(EcalUtils ${ROOT_PHYSICS_LIB})

# Create the TrackUtils library
add_library(TrackUtils TrackUtils.cxx)
target_link_libraries(TrackUtils ${LCIO_DICT_LIB})

