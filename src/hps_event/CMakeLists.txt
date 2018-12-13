#
#   @file: CMakeList.txt
#   @author: Omar Moreno <omoreno1@ucsc.edu>
#   @date: December 06, 2013
#
project(HpsEvent)

SET(headers 
  CalorimeterHit.h 
  Cluster.h 
  EcalCluster.h 
  EcalHit.h 
  GblStripData.h 
  GblTrack.h 
  GblTrackData.h 
  HpsEvent.h 
  HpsMCParticle.h
  MCParticle.h 
  HpsParticle.h 
  MCParticle.h 
#  MuonCluster.h 
  SvtHit.h 
  SvtTrack.h 
  TriggerData.h )

foreach(h ${headers})
  LIST(APPEND dir_headers ${HPS_EVENT_HDR_DIR}/${h} )
endforeach()

if( ROOT_VERSION VERSION_GREATER 6.0.0)
  ROOT_GENERATE_DICTIONARY(HpsEventDict ${dir_headers} MODULE ${PROJECT_NAME} LINKDEF ${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h )
else()
  ROOT_GENERATE_DICTIONARY(HpsEventDict ${dir_headers} LINKDEF ${HPS_EVENT_HDR_DIR}/HpsEventLinkDef.h )
endif()

#  LegacyGblTrack.h

add_library(HpsEventLib
  EcalCluster.cxx 
  EcalHit.cxx 
  GblTrack.cxx
  HpsEvent.cxx
  HpsEventDict.cxx
  HpsMCParticle.cxx
  HpsParticle.cxx
  MCParticle.cxx
  SvtHit.cxx 
  SvtTrack.cxx
  TriggerData.cxx 
#  MuonCluster.cxx
)

add_library(HpsEvent SHARED 
  EcalCluster.cxx 
  EcalHit.cxx 
  GblTrack.cxx
  HpsEvent.cxx
  HpsEventDict.cxx
  HpsMCParticle.cxx
  MCParticle.cxx
  HpsParticle.cxx
  SvtHit.cxx 
  SvtTrack.cxx
  TriggerData.cxx 
#  MuonCluster.cxx
)  

target_link_libraries(HpsEvent  ${ROOT_LIBRARIES} )

if(HPS_INSTALL_ALL_LIBS)
  install(TARGETS HpsEventLib DESTINATION ${CMAKE_INSTALL_LIBDIR} )
endif()
install(TARGETS HpsEvent DESTINATION ${CMAKE_INSTALL_LIBDIR} )

