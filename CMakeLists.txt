cmake_minimum_required(VERSION 3.10)
project(CaesarCipherTUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 查找ncurses库
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# 源文件
set(SOURCES
    src/CaesarCipher.cpp
    src/TUI.cpp
    src/main.cpp
)

# 可执行文件
add_executable(caesar_cipher_tui ${SOURCES})

# 链接库
target_link_libraries(caesar_cipher_tui ${CURSES_LIBRARIES})

# 安装目标
install(TARGETS caesar_cipher_tui
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# 测试文件
enable_testing()

# 添加测试
add_test(NAME test_caesar_cipher
    COMMAND caesar_cipher_tui --help
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
set(ENV{LANG} "zh_CN.UTF-8")
set(ENV{LC_CTYPE} "zh_CN.UTF-8")
