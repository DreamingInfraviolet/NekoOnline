add_executable(Server
                    ${COMMON_SOURCES}
                    ${COMMON_HEADERS}
                    ${SERVER_SOURCES}
                    ${SERVER_HEADERS})

include_directories(. ${COMMON_DIR})