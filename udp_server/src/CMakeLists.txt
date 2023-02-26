# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES("../inc")

SET(SRC_LIST file.cpp server.cpp main.cpp)

# 添加生成二进制与依赖
ADD_EXECUTABLE(udp_server ${SRC_LIST})