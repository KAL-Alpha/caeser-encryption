#include "CaesarCipher.h"
#include <iostream>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

// 构造函数
CaesarCipher::CaesarCipher(int shift) : shift(shift % 26) {
    if (this->shift < 0) {
        this->shift += 26;
    }
}

// 设置移位值
void CaesarCipher::setShift(int newShift) {
    shift = newShift % 26;
    if (shift < 0) {
        shift += 26;
    }
}

// 获取当前移位值
int CaesarCipher::getShift() const {
    return shift;
}

// 检查字符是否为字母
bool CaesarCipher::isAlpha(char c) const {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// 检查字符是否为大写字母
bool CaesarCipher::isUpper(char c) const {
    return (c >= 'A' && c <= 'Z');
}

// 检查字符是否为小写字母
bool CaesarCipher::isLower(char c) const {
    return (c >= 'a' && c <= 'z');
}

// 加密单个字符
char CaesarCipher::encryptChar(char c, int shift) const {
    if (!isAlpha(c)) {
        return c; // 非字母字符保持不变
    }
    
    char base = isUpper(c) ? 'A' : 'a';
    return static_cast<char>((c - base + shift) % 26 + base);
}

// 解密单个字符
char CaesarCipher::decryptChar(char c, int shift) const {
    if (!isAlpha(c)) {
        return c; // 非字母字符保持不变
    }
    
    char base = isUpper(c) ? 'A' : 'a';
    int result = (c - base - shift) % 26;
    if (result < 0) {
        result += 26;
    }
    return static_cast<char>(result + base);
}

// 加密字符串
std::string CaesarCipher::encrypt(const std::string& text) const {
    std::string result;
    for (char c : text) {
        result += encryptChar(c, shift);
    }
    return result;
}

// 解密字符串
std::string CaesarCipher::decrypt(const std::string& text) const {
    std::string result;
    for (char c : text) {
        result += decryptChar(c, shift);
    }
    return result;
}

// 加密文件内容
std::string CaesarCipher::encryptFile(const std::string& filename) const {
    std::string content = readFromFile(filename);
    return encrypt(content);
}

// 解密文件内容
std::string CaesarCipher::decryptFile(const std::string& filename) const {
    std::string content = readFromFile(filename);
    return decrypt(content);
}

// 从文件读取文本
std::string CaesarCipher::readFromFile(const std::string& filename) {
    // 首先尝试在output目录下查找文件
    std::string outputPath = "output/" + filename;
    std::ifstream file(outputPath);
    
    // 如果在output目录下找不到，再尝试直接打开文件（可能是绝对路径或相对路径）
    if (!file.is_open()) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件: " + filename + " (在output目录和当前目录均未找到)");
        }
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    // 移除最后一个换行符（如果有）
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    
    return content;
}

// 保存加密文本到文件（按日期时间自动命名）
void CaesarCipher::saveToFile(const std::string& text, const std::string& prefix) const {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    
    // 创建文件名：prefix_YYYYMMDD_HHMMSS.txt
    std::ostringstream filename;
    filename << prefix << "_"
             << (now_tm->tm_year + 1900)
             << std::setfill('0') << std::setw(2) << (now_tm->tm_mon + 1)
             << std::setfill('0') << std::setw(2) << now_tm->tm_mday
             << "_"
             << std::setfill('0') << std::setw(2) << now_tm->tm_hour
             << std::setfill('0') << std::setw(2) << now_tm->tm_min
             << std::setfill('0') << std::setw(2) << now_tm->tm_sec
             << ".txt";
    
    // 确保output目录存在
    fs::create_directory("output");
    
    std::string filepath = "output/" + filename.str();
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("无法创建文件: " + filepath);
    }
    
    file << text;
    file.close();
    
    std::cout << "文件已保存到: " << filepath << std::endl;
}
