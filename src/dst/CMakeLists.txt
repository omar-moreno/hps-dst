#
#
#
#

# Find the necessary LCIO libraries
find_library(LCIO_LIB lcio $ENV{LCIO}/lib)

# Create the SvtDataWriter library
add_library(SvtDataWriter SvtDataWriter.cxx)
target_link_libraries(SvtDataWriter ${LCIO_LIB} TrackUtils HpsEvent)

# Create the EcalDataWriter library
add_library(EcalDataWriter EcalDataWriter.cxx)
target_link_libraries(EcalDataWriter ${LCIO_LIB} EcalUtils HpsEvent)

# Create the MCParticleDataWriter library
add_library(MCParticleDataWriter MCParticleDataWriter.cxx)
target_link_libraries(MCParticleDataWriter ${LCIO_LIB} HpsEvent)

# Create the HpsEventBuilder library
add_library(HpsEventBuilder HpsEventBuilder.cxx)
target_link_libraries(HpsEventBuilder SvtDataWriter EcalDataWriter MCParticleDataWriter)

# Create the dst_writer executable
add_executable(dst_maker dst_maker.cxx)
target_link_libraries(dst_maker ${LCIO_LIB} ${ROOT_TREE_LIBRARY} ${ROOT_CORE_LIBRARY} ${ROOT_RIO_LIBRARY} HpsEvent HpsEventBuilder)
