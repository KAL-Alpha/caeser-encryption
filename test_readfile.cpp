#include "include/CaesarCipher.h"
#include <iostream>
#include <fstream>

int main() {
    CaesarCipher cipher(3);
    
    try {
        // 测试1：读取output目录下的文件（只提供文件名）
        std::cout << "测试1：读取output目录下的文件（只提供文件名）" << std::endl;
        std::string content1 = cipher.readFromFile("test_file.txt");
        std::cout << "读取成功，内容：" << std::endl;
        std::cout << content1 << std::endl;
        std::cout << "------------------------" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "测试1失败：" << e.what() << std::endl;
    }
    
    // 创建一个不在output目录的测试文件
    std::ofstream testFile("outside_test.txt");
    testFile << "这是一个不在output目录的测试文件";
    testFile.close();
    
    try {
        // 测试2：读取当前目录下的文件（提供文件名）
        std::cout << "\n测试2：读取当前目录下的文件（提供文件名）" << std::endl;
        std::string content2 = cipher.readFromFile("outside_test.txt");
        std::cout << "读取成功，内容：" << std::endl;
        std::cout << content2 << std::endl;
        std::cout << "------------------------" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "测试2失败：" << e.what() << std::endl;
    }
    
    // 清理测试文件
    std::remove("outside_test.txt");
    
    try {
        // 测试3：尝试读取不存在的文件
        std::cout << "\n测试3：尝试读取不存在的文件" << std::endl;
        std::string content3 = cipher.readFromFile("non_existent_file.txt");
        std::cout << "错误：应该抛出异常但没有" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "测试3成功：正确抛出异常 - " << e.what() << std::endl;
    }
    
    return 0;
}
