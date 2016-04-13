add_executable(Client WIN32
                    ${COMMON_SOURCES}
                    ${COMMON_HEADERS}
                    ${CLIENT_SOURCES}
                    ${CLIENT_HEADERS})

include_directories(. ${COMMON_DIR})  