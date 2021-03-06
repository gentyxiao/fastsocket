PROJECT(udpsvr)
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
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bulid/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bulid/bin)
# 遍历目录的所有文件赋值给Dir_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)



################################### set include path ###################################################################### 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../udpserver/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../import/libevent/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../import/libevent/include/event2)

################################### set link path ###################################################################### 
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../import/libevent/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bulid/bin/)

LINK_LIBRARIES(pthread)
LINK_LIBRARIES(event)
LINK_LIBRARIES(event_openssl)
LINK_LIBRARIES(event_extra)
LINK_LIBRARIES(event_core)
LINK_LIBRARIES(udpserver)
################################### compile options ####################################################################
if(Flag)        
message( STATUS "==============================message to Flag" )        
ADD_DEFINITIONS(-g)
else(Flag)        
ADD_DEFINITIONS(-O2)
endif()
ADD_DEFINITIONS("-std=c++11")
################################### generate lib file ################################################################
#ADD_LIBRARY(${PROJECT_NAME} SHARED ${DIR_SRCS})
#ADD_LIBRARY(${PROJECT_NAME} STATIC ${DIR_SRCS})
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread event event_openssl event_extra event_core udpserver)
