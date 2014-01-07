#
#
#
#

# Find the necessary LCIO libraries
find_library(LCIO_DICT_LIB lcioDict $ENV{LCIO}/lib)
find_library(LCIO_LIB lcio $ENV{LCIO}/lib)

# Find the required ROOT libraries
find_library(ROOT_TREE_LIB Tree $ENV{ROOTSYS}/lib)
find_library(ROOT_CORE_LIB Core $ENV{ROOTSYS}/lib)
find_library(ROOT_RIO_LIB RIO $ENV{ROOTSYS}/lib)
find_library(ROOT_PHYSICS_LIB Physics $ENV{ROOTSYS}/lib)

# Create the SvtDataWriter library
add_library(SvtDataWriter SvtDataWriter.cxx)
target_link_libraries(SvtDataWriter ${LCIO_LIB} TrackUtils)

# Create the HpsEventBuilder library
add_library(HpsEventBuilder HpsEventBuilder.cxx)
target_link_libraries(HpsEventBuilder SvtDataWriter HpsEvent)

# Create the dst_writer executable
add_executable(dst_maker dst_maker.cxx)
target_link_libraries(dst_maker ${LCIO_LIB} ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} HpsEvent ${ROOT_RIO_LIB} HpsEventBuilder)
