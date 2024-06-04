#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

// Mock getch for macOS
static inline int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Mock Sleep for macOS
#define Sleep(ms) usleep((ms) * 1000)

