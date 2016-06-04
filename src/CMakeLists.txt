               
set(SERVER_DIR                                     ${CMAKE_CURRENT_SOURCE_DIR}/server/)
set(CLIENT_DIR                                     ${CMAKE_CURRENT_SOURCE_DIR}/client/)
set(COMMON_DIR                                     ${CMAKE_CURRENT_SOURCE_DIR}/)
                   
file(GLOB    COMMON_SOURCES                        ${COMMON_DIR}/*.cpp
                                                   ${COMMON_DIR}/*.cc
                                                   ${COMMON_DIR}/*.c)
file(GLOB    COMMON_HEADERS                        ${COMMON_DIR}/*.h
                                                   ${COMMON_DIR}/*.hpp)
                   
file(GLOB    SERVER_SOURCES                        ${SERVER_DIR}/*.cpp
                                                   ${SERVER_DIR}/*.cc
                                                   ${SERVER_DIR}/*.c)
file(GLOB    SERVER_HEADERS                        ${SERVER_DIR}/*.h
                                                   ${SERVER_DIR}/*.hpp)

file(GLOB    CLIENT_SOURCES                        ${CLIENT_DIR}/*.cpp
                                                   ${CLIENT_DIR}/*.cc
                                                   ${CLIENT_DIR}/*.c)
file(GLOB    CLIENT_HEADERS                        ${CLIENT_DIR}/*.h
                                                   ${CLIENT_DIR}/*.hpp)
                                                 

#Boost asio:
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include/) 

include_directories(${COMMON_DIR})
add_subdirectory(server)
add_subdirectory(client) 