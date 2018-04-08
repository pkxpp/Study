cmake_minimum_required (VERSION 2.6)

# 项目信息
project (algorithm)

# 添加头文件路径
include_directories(./ ./include)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)


# 添加 math 子目录
add_subdirectory(list)
add_subdirectory(heap)

# 指定生成目标 
add_executable(algorithm main.cpp)

# 添加链接库
target_link_libraries(algorithm List Heap)
