INCLUDE_DIRECTORIES("../inc")

SET(SRC_LIST client.cpp main.cpp)

ADD_EXECUTABLE(udp_client ${SRC_LIST})