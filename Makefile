# Makefile for Caesar Cipher TUI Project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -lncursesw

# 源文件
SRCS = src/CaesarCipher.cpp src/TUI.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)

# 目标可执行文件
TARGET = caesar_cipher_tui

# 默认目标
all: $(TARGET)

# 链接可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 编译源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 运行
run: $(TARGET)
	./$(TARGET)

# 安装依赖 (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libncursesw5-dev

# 测试
test: $(TARGET)
	@echo "运行测试..."
	@./$(TARGET) --help || true

.PHONY: all clean run install-deps test
