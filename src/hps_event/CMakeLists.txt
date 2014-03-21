
#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#

# Find all the source files that will be used to build the HpsEvent 
# shared library
file(GLOB HPS_EVENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx")
file(GLOB HPS_EVENT_DIC_SRC "${HPS_EVENT_DIC_DIR}/*.cxx")

# Add the HpsEvent library
add_library(HpsEvent SHARED ${HPS_EVENT_SRC} ${HPS_EVENT_DIC_SRC})
target_link_libraries(HpsEvent ${ROOT_CORE_LIBRARY} ${ROOT_PHYSICS_LIBRARY})

