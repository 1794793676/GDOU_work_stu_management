# GDOU 学生信息管理系统

## 项目简介

本项目是一个基于 Qt 的学生信息管理系统，支持学生信息的增删查改、数据持久化、快捷键操作和图片资源管理。项目采用 C++ 语言开发，界面美观，结构清晰，支持 Linux 和 Windows 跨平台编译运行。
(应付学校大一的课设，随便学了点Qt做着玩玩)

## 目录结构

```
TEST8/
├── CMakeLists.txt           # 主CMake配置
├── README.md                # 项目说明文档
├── deb/                     # 存放生成的deb安装包
├── resources/               # 资源文件（图片、qrc等）
│   ├── image.qrc
│   └── icon/
│       ├── bk.png
│       ├── logo1.png
│       ├── logo2.png
│       └── logo3.png
├── src/                     # 源代码和UI文件
│   ├── main.cpp
│   ├── mainwindow.cpp/h/ui
│   ├── form.cpp/h/ui
└── ...                      # 其他配置文件
```

## 编译与打包

### Linux (推荐 Ubuntu/Debian)

1. 安装依赖：
   ```bash
   sudo apt-get install qtbase5-dev qt5-qmake cmake g++
   ```
2. 编译与打包：
   ```bash
   mkdir build && cd build
   cmake ..
   make
   make deb_and_move
   ```
   生成的 `.deb` 安装包会自动移动到根目录的 `deb/` 文件夹。

### Windows

1. 安装 Qt（建议用 Qt 官方安装器，包含 Qt Creator 和 MinGW/MSVC）。
2. 用 Qt Creator 打开本项目，选择合适的 kit，直接构建即可。
3. 用 `windeployqt` 工具将依赖 DLL 打包到可执行文件目录。

## 运行方法

- **Linux**：
  ```bash
  ./build/bin/TEST8
  ```
  或安装 deb 包后在菜单中启动。
- **Windows**：
  双击 `TEST8.exe` 运行。

## 跨平台说明

- 本项目完全基于 Qt 和 C++ 标准库，无平台专有依赖。
- 资源文件通过 Qt 的资源系统（qrc）管理，路径兼容性好。
- 数据文件建议使用 UTF-8 编码，确保中英文兼容。

## 其他

- 数据修改是通过操作build/data/data.txt实现的，后续可能会改数据库 吧？
- 有任何问题欢迎提 issue 或联系作者。（主包纯小白，不一定会解决）
