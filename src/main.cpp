#include "TUI.h"
#include <iostream>
#include <cstdlib>

int main() {
    try {
        // 创建TUI实例并运行
        TUI tui(3); // 默认移位值为3
        tui.run();
        
        std::cout << "程序正常退出。" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "程序发生错误: " << e.what() << std::endl;
        std::cerr << "请确保已安装ncurses库。" << std::endl;
        std::cerr << "在Ubuntu/Debian上运行: sudo apt-get install libncurses-dev" << std::endl;
        return EXIT_FAILURE;
    }
}
