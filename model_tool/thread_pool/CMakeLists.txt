# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 工程名称
PROJECT (thread_pool)

# 将 src 目录添加到目录树
# ADD_SUBDIRECTORY(src bin)

# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(inc)

# 查找目录下的所以源文件
AUX_SOURCE_DIRECTORY(src THREAD_POOL_SRC_LIST)

# 添加生成二进制与依赖
ADD_EXECUTABLE(thread_pool ${THREAD_POOL_SRC_LIST})