PROJECT(UDPServer)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


################################## control software version ##################################
set(MAR_VERSION 0)
set(MIN_VERSION 0)
set(RELEASE_VERSION 2)

configure_file(
"${PROJECT_SOURCE_DIR}/Config.h.in"  
"${PROJECT_BINARY_DIR}/Config.h" 
)


################################### Reconfigure final output directory ##################################
set(LIBRARY_OUTPUT_PATH                    ${PROJECT_SOURCE_DIR}/../bulid/bin)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)



################################### set include path ###################################################################### 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../import/libevent/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../import/libevent/include/event2)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../import/libevent/lib)

LINK_LIBRARIES(pthread)
LINK_LIBRARIES(event)
LINK_LIBRARIES(event_openssl)
LINK_LIBRARIES(event_extra)
LINK_LIBRARIES(event_core)
################################### compile options ####################################################################
if(Flag)        
message( STATUS "==============================message to Flag" )        
ADD_DEFINITIONS(-g)
else(Flag)        
ADD_DEFINITIONS(-O2)
endif()
ADD_DEFINITIONS("-std=c++11")
################################### generate lib file ################################################################
ADD_LIBRARY(${PROJECT_NAME}                     SHARED                      ${DIR_SRCS})


