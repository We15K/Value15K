# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES("../inc")

# 设置源文件
SET(SRC_LIST server.cpp main.cpp)

# 添加生成二进制与依赖
ADD_EXECUTABLE(udp_server ${SRC_LIST})