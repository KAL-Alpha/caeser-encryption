#include "TUI.h"
#include <ncursesw/ncurses.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <locale.h>

// 构造函数
TUI::TUI(int shift) : cipher(shift), running(true) {
    initNCurses();
}

// 析构函数
TUI::~TUI() {
    cleanupNCurses();
}

// 初始化ncurses
void TUI::initNCurses() {
    // 设置区域设置以支持UTF-8
    setlocale(LC_ALL, "");
    
    initscr();            // 初始化ncurses
    cbreak();             // 禁用行缓冲
    noecho();             // 不显示输入字符
    keypad(stdscr, TRUE); // 启用功能键
    curs_set(0);          // 隐藏光标
    
    // 启用颜色
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);  // 标题颜色
        init_pair(2, COLOR_CYAN, COLOR_BLACK);   // 菜单颜色
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // 高亮颜色
        init_pair(4, COLOR_RED, COLOR_BLACK);    // 错误颜色
        init_pair(5, COLOR_WHITE, COLOR_BLACK);  // 正常文本颜色
    }
}

// 清理ncurses
void TUI::cleanupNCurses() {
    endwin();
}

// 显示标题
void TUI::showTitle() {
    clear();
    
    // 显示标题
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, (COLS - 30) / 2, "==============================");
    mvprintw(2, (COLS - 30) / 2, "     凯撒加密工具 (TUI)      ");
    mvprintw(3, (COLS - 30) / 2, "==============================");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    // 显示当前设置
    showCurrentSettings();
    
    refresh();
}

// 显示当前设置
void TUI::showCurrentSettings() {
    attron(COLOR_PAIR(5));
    mvprintw(5, 2, "当前设置:");
    mvprintw(6, 4, "移位值: %d", cipher.getShift());
    attroff(COLOR_PAIR(5));
}

// 显示消息
void TUI::showMessage(const std::string& message, bool isError) {
    int color_pair = isError ? 4 : 5;
    
    attron(COLOR_PAIR(color_pair));
    mvprintw(LINES - 3, 2, "%s", message.c_str());
    attroff(COLOR_PAIR(color_pair));
    
    refresh();
    napms(2000); // 显示2秒
    
    // 清除消息
    move(LINES - 3, 2);
    clrtoeol();
    refresh();
}

// 获取用户输入
std::string TUI::getInput(const std::string& prompt) {
    echo(); // 显示输入字符
    curs_set(1); // 显示光标
    
    mvprintw(LINES - 5, 2, "%s", prompt.c_str());
    refresh();
    
    char input[256];
    getnstr(input, sizeof(input) - 1);
    
    noecho(); // 恢复不显示输入字符
    curs_set(0); // 隐藏光标
    
    return std::string(input);
}

// 显示主菜单
int TUI::showMainMenu() {
    showTitle();
    
    std::vector<std::string> menuItems = {
        "1. 加密文本",
        "2. 解密文本",
        "3. 加密文件",
        "4. 解密文件",
        "5. 设置移位值",
        "6. 退出"
    };
    
    // 显示菜单项
    attron(COLOR_PAIR(2));
    for (size_t i = 0; i < menuItems.size(); ++i) {
        mvprintw(10 + i * 2, (COLS - 20) / 2, "%s", menuItems[i].c_str());
    }
    attroff(COLOR_PAIR(2));
    
    // 显示提示
    attron(COLOR_PAIR(5));
    mvprintw(LINES - 2, 2, "使用数字键选择选项 (1-6): ");
    attroff(COLOR_PAIR(5));
    
    refresh();
    
    // 获取用户选择
    int choice = 0;
    while (choice < 1 || choice > 6) {
        int ch = getch();
        if (ch >= '1' && ch <= '6') {
            choice = ch - '0';
        }
    }
    
    return choice;
}

// 显示结果窗口
void TUI::showResult(const std::string& title, const std::string& content) {
    clear();
    
    // 显示标题
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, (COLS - title.length()) / 2, "%s", title.c_str());
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    // 显示内容
    attron(COLOR_PAIR(5));
    
    // 分页显示内容
    int start_line = 4;
    int max_width = COLS - 4;
    int max_lines = LINES - 8;
    
    std::string display_text = content;
    size_t pos = 0;
    int line = start_line;
    
    while (pos < display_text.length() && line < start_line + max_lines) {
        size_t end_pos = pos + max_width;
        if (end_pos > display_text.length()) {
            end_pos = display_text.length();
        }
        
        std::string line_text = display_text.substr(pos, end_pos - pos);
        mvprintw(line, 2, "%s", line_text.c_str());
        
        pos = end_pos;
        line++;
    }
    
    attroff(COLOR_PAIR(5));
    
    // 显示提示
    attron(COLOR_PAIR(2));
    mvprintw(LINES - 3, 2, "按任意键返回主菜单...");
    attroff(COLOR_PAIR(2));
    
    refresh();
    getch();
}

// 加密文本界面
void TUI::encryptTextUI() {
    showTitle();
    
    attron(COLOR_PAIR(5));
    mvprintw(8, 2, "=== 加密文本 ===");
    attroff(COLOR_PAIR(5));
    
    std::string text = getInput("请输入要加密的文本: ");
    
    if (!text.empty()) {
        std::string encrypted = cipher.encrypt(text);
        
        // 显示结果
        clear();
        showTitle();
        
        attron(COLOR_PAIR(5));
        mvprintw(8, 2, "原始文本: %s", text.c_str());
        mvprintw(10, 2, "加密结果: %s", encrypted.c_str());
        attroff(COLOR_PAIR(5));
        
        // 询问是否保存
        attron(COLOR_PAIR(2));
        mvprintw(12, 2, "是否保存到文件? (y/n): ");
        attroff(COLOR_PAIR(2));
        
        echo();
        curs_set(1);
        char choice = getch();
        noecho();
        curs_set(0);
        
        if (choice == 'y' || choice == 'Y') {
            try {
                cipher.saveToFile(encrypted, "encrypted_text");
                showMessage("文件保存成功!", false);
            } catch (const std::exception& e) {
                showMessage(std::string("保存失败: ") + e.what(), true);
            }
        }
        
        showResult("加密结果", encrypted);
    } else {
        showMessage("输入不能为空!", true);
    }
}

// 解密文本界面
void TUI::decryptTextUI() {
    showTitle();
    
    attron(COLOR_PAIR(5));
    mvprintw(8, 2, "=== 解密文本 ===");
    attroff(COLOR_PAIR(5));
    
    std::string text = getInput("请输入要解密的文本: ");
    
    if (!text.empty()) {
        std::string decrypted = cipher.decrypt(text);
        showResult("解密结果", decrypted);
    } else {
        showMessage("输入不能为空!", true);
    }
}

// 加密文件界面
void TUI::encryptFileUI() {
    showTitle();
    
    attron(COLOR_PAIR(5));
    mvprintw(8, 2, "=== 加密文件 ===");
    attroff(COLOR_PAIR(5));
    
    std::string filename = getInput("请输入文件名: ");
    
    try {
        std::string encrypted = cipher.encryptFile(filename);
        
        // 显示结果
        clear();
        showTitle();
        
        attron(COLOR_PAIR(5));
        mvprintw(8, 2, "文件: %s", filename.c_str());
        mvprintw(10, 2, "加密结果: (显示前100个字符)");
        
        // 显示部分结果
        std::string preview = encrypted.substr(0, std::min(100, (int)encrypted.length()));
        if (encrypted.length() > 100) {
            preview += "...";
        }
        mvprintw(12, 2, "%s", preview.c_str());
        attroff(COLOR_PAIR(5));
        
        // 询问是否保存
        attron(COLOR_PAIR(2));
        mvprintw(14, 2, "是否保存到文件? (y/n): ");
        attroff(COLOR_PAIR(2));
        
        echo();
        curs_set(1);
        char choice = getch();
        noecho();
        curs_set(0);
        
        if (choice == 'y' || choice == 'Y') {
            try {
                cipher.saveToFile(encrypted, "encrypted_file");
                showMessage("文件保存成功!", false);
            } catch (const std::exception& e) {
                showMessage(std::string("保存失败: ") + e.what(), true);
            }
        }
        
        showResult("文件加密结果", encrypted);
    } catch (const std::exception& e) {
        showMessage(std::string("错误: ") + e.what(), true);
    }
}

// 解密文件界面
void TUI::decryptFileUI() {
    showTitle();
    
    attron(COLOR_PAIR(5));
    mvprintw(8, 2, "=== 解密文件 ===");
    attroff(COLOR_PAIR(5));
    
    std::string filename = getInput("请输入文件名: ");
    
    try {
        std::string decrypted = cipher.decryptFile(filename);
        showResult("文件解密结果", decrypted);
    } catch (const std::exception& e) {
        showMessage(std::string("错误: ") + e.what(), true);
    }
}

// 设置移位值界面
void TUI::setShiftUI() {
    showTitle();
    
    attron(COLOR_PAIR(5));
    mvprintw(8, 2, "=== 设置移位值 ===");
    mvprintw(10, 2, "当前移位值: %d", cipher.getShift());
    attroff(COLOR_PAIR(5));
    
    std::string input = getInput("请输入新的移位值 (1-25): ");
    
    try {
        int newShift = std::stoi(input);
        if (newShift >= 1 && newShift <= 25) {
            cipher.setShift(newShift);
            showMessage("移位值已更新!", false);
        } else {
            showMessage("移位值必须在1-25之间!", true);
        }
    } catch (const std::exception& e) {
        showMessage("无效的输入!", true);
    }
}

// 运行TUI主循环
void TUI::run() {
    while (running) {
        int choice = showMainMenu();
        
        switch (choice) {
            case 1:
                encryptTextUI();
                break;
            case 2:
                decryptTextUI();
                break;
            case 3:
                encryptFileUI();
                break;
            case 4:
                decryptFileUI();
                break;
            case 5:
                setShiftUI();
                break;
            case 6:
                running = false;
                showMessage("感谢使用凯撒加密工具!", false);
                break;
        }
    }
}
