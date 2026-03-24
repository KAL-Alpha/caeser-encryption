#ifndef TUI_H
#define TUI_H

#include "CaesarCipher.h"
#include <string>
#include <vector>

class TUI {
private:
    CaesarCipher cipher;
    bool running;
    
    // 初始化ncurses
    void initNCurses();
    
    // 清理ncurses
    void cleanupNCurses();
    
    // 显示主菜单
    int showMainMenu();
    
    // 显示标题
    void showTitle();
    
    // 显示当前设置
    void showCurrentSettings();
    
    // 加密文本界面
    void encryptTextUI();
    
    // 解密文本界面
    void decryptTextUI();
    
    // 加密文件界面
    void encryptFileUI();
    
    // 解密文件界面
    void decryptFileUI();
    
    // 设置移位值界面
    void setShiftUI();
    
    // 显示消息
    void showMessage(const std::string& message, bool isError = false);
    
    // 获取用户输入
    std::string getInput(const std::string& prompt);
    
    // 显示结果窗口
    void showResult(const std::string& title, const std::string& content);

public:
    TUI(int shift = 3);
    ~TUI();
    
    // 运行TUI主循环
    void run();
};

#endif // TUI_H
