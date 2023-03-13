# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(../inc)

SET(SRC_LIST thread_pool.cpp main.cpp)

# 添加生成二进制与依赖
ADD_EXECUTABLE(thread_pool ${SRC_LIST})