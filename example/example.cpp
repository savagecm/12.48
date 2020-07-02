

#include "epd1248b.hpp"

int main()
{
    epd1248 *epdInstance = epd1248::getInstance();
    epdInstance->init();
    epdInstance->printString("你好世界", 48, 100, 100, BLACK, WHITE);
    epdInstance->printString("红色的字白色的底", 48, 200, 100, RED, WHITE);
    epdInstance->printString("红色的字黑色的底", 48, 300, 100, RED, BLACK);

    epdInstance->printString("abcdefg", 24, 400, 100, BLACK, RED);
    epdInstance->printString("hijklmn", 24, 500, 100, BLACK, WHITE);

    epdInstance->printString("12345^&*()|}{~!", 24, 600, 100, BLACK, WHITE);

    epdInstance->printString("准备进入睡眠模式", 48, 700, 100, BLACK, WHITE);
    epdInstance->display();
    epdInstance->sleep();
    epdInstance->printString("从睡眠模式中唤醒", 48, 700, 100, BLACK, WHITE);
    epdInstance->display();
    epdInstance->sleep();
}