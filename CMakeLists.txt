cmake_minimum_required(VERSION 3.10)

project(TEST8 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/dolphin/Qt/5.15.19/gcc_64/lib/cmake") # 指向包含Qt5Config.cmake的目录
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 只添加src子目录
add_subdirectory(src)

# 安装资源文件
install(DIRECTORY resources/ DESTINATION share/${PROJECT_NAME}/resources)

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "STUSYS")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dolphin")
set(CPACK_PACKAGE_VERSION "0.1")
include(CPack)

# 合并打包和移动 deb 包为一个目标
add_custom_target(deb_and_move
    COMMAND ${CMAKE_MAKE_PROGRAM} package
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/deb
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/*.deb
        ${CMAKE_SOURCE_DIR}/deb/
    COMMENT "Build and move .deb files to deb/ directory"
)