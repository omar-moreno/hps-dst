#
# @file: CMakeLists.txt
# @author: Omar Moreno <omoreno1@ucsc.edu>
#          Santa Cruz Institute for Particle Physics
#          University of California, Santa Cruz
#  @date: December 06, 2013
#

# Add the dump_hps_event executable
add_executable(dump_hps_event dump_hps_event.cxx)
target_link_libraries(dump_hps_event  ${LCIO_LIB} )

install(TARGETS dump_hps_event DESTINATION ${CMAKE_INSTALL_BINDIR})


add_library(TrackType TrackType.cxx)
target_link_libraries(TrackType HpsEventLib)
if(HPS_INSTALL_ALL_LIBS)
  install(TARGETS TrackType DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# Build the TrackUtils library
add_library(TrackUtils TrackUtils.cxx)
target_link_libraries(TrackUtils  ${LCIO_LIB})
if(HPS_INSTALL_ALL_LIBS)
  install(TARGETS TrackUtils DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# Build the EcalUtils library
add_library(EcalUtils EcalUtils.cxx)
target_link_libraries(EcalUtils  ${LCIO_LIB})
if(HPS_INSTALL_ALL_LIBS)
  install(TARGETS EcalUtils DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()