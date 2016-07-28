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