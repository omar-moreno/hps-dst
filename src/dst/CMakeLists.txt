#
#
#
#

#add_library(DstUtils DstUtils.cxx)
#target_link_libraries(DstUtils 
#    ${LCIO_LIB}
#) 

# Create the SvtDataWriter library
#add_library(SvtDataWriter SvtDataWriter.cxx)
#target_link_libraries(SvtDataWriter 
#    ${LCIO_LIB} 
#    DstUtils
#    TrackUtils 
#    HpsEvent
#)

# Create the EcalDataWriter library
#add_library(EcalDataWriter EcalDataWriter.cxx)
#target_link_libraries(EcalDataWriter ${LCIO_LIB} EcalUtils HpsEvent)

# Create the MCParticleDataWriter library
#add_library(MCParticleDataWriter MCParticleDataWriter.cxx)
#target_link_libraries(MCParticleDataWriter ${LCIO_LIB} HpsEvent)

# Create the HpsParticleDataWriter library
#add_library(HpsParticleDataWriter HpsParticleDataWriter.cxx)
#target_link_libraries(HpsParticleDataWriter 
#    ${LCIO_LIB} 
#    DstUtils
#    HpsEvent
#)

# Create the HpsEventBuilder library
add_library(HpsEventBuilder HpsEventBuilder.cxx DstUtils.cxx SvtDataWriter.cxx EcalDataWriter.cxx MCParticleDataWriter.cxx HpsParticleDataWriter.cxx )
target_link_libraries(HpsEventBuilder  ${LCIO_LIB}   HpsEventLib)

if(HPS_INSTALL_ALL_LIBS)
  install(TARGETS HpsEventBuilder DESTINATION ${CMAKE_INSTALL_LIBDIR} )
endif()

# Create the dst_writer executable
add_executable(dst_maker dst_maker.cxx)
target_link_libraries(dst_maker ${GBL_LIB} ${LCIO_LIB} ${ROOT_LIBRARIES} HpsEventLib HpsEventBuilder TrackType TrackUtils EcalUtils)


install(TARGETS dst_maker DESTINATION ${CMAKE_INSTALL_BINDIR})