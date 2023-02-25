INCLUDE_DIRECTORIES("../inc")

SET(SRC_LIST server.cpp main.cpp)

ADD_EXECUTABLE(udp_server ${SRC_LIST})