#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#

# Find the necessary LCIO libraries
find_library(LCIO_LIB lcio $ENV{LCIO}/lib)

# Find the necessary GBL library
#find_library(GBL_LIB GBL $ENV{GBL}/lib)
find_library(GBL_LIB GBL ${GBL_DIR}/lib)

# Add the dump_hps_event executable
add_executable(dump_hps_event dump_hps_event.cxx)
target_link_libraries(dump_hps_event ${LCIO_LIB})

# Create the TrackUtils library
add_library(TrackUtils TrackUtils.cxx)
target_link_libraries(TrackUtils ${LCIO_LIB})

# Create the EcalUtils library
add_library(EcalUtils EcalUtils.cxx)
target_link_libraries(EcalUtils ${LCIO_LIB})

# If GBL is installed, create the HpsGblFitter library
find_library(GBL_LIB GBL ${GBL_LIBRARY_DIRS})
add_library(HpsGblFitter HpsGblFitter.cxx)
target_link_libraries(HpsGblFitter ${GBL_LIB} ${ROOT_MATHCORE_LIBRARY} ${ROOT_MATRIX_LIBRARY})
