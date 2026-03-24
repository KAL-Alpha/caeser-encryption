#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class CaesarCipher {
private:
    int shift;
    
    // 加密单个字符
    char encryptChar(char c, int shift) const;
    
    // 解密单个字符
    char decryptChar(char c, int shift) const;
    
    // 检查字符是否为字母
    bool isAlpha(char c) const;
    
    // 检查字符是否为大写字母
    bool isUpper(char c) const;
    
    // 检查字符是否为小写字母
    bool isLower(char c) const;

public:
    // 构造函数，默认移位值为3
    CaesarCipher(int shift = 3);
    
    // 设置移位值
    void setShift(int newShift);
    
    // 获取当前移位值
    int getShift() const;
    
    // 加密字符串
    std::string encrypt(const std::string& text) const;
    
    // 解密字符串
    std::string decrypt(const std::string& text) const;
    
    // 加密文件内容
    std::string encryptFile(const std::string& filename) const;
    
    // 解密文件内容
    std::string decryptFile(const std::string& filename) const;
    
    // 保存加密文本到文件（按日期时间自动命名）
    void saveToFile(const std::string& text, const std::string& prefix = "encrypted") const;
    
    // 从文件读取文本
    static std::string readFromFile(const std::string& filename);
};

#endif // CAESARCIPHER_H
