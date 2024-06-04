#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "conio.h"
#include "assets.h"

// --- Professional Palette ---
#define CLR_ROSEWATER "\033[38;2;245;224;220m"
#define CLR_FLAMINGO  "\033[38;2;242;205;205m"
#define CLR_PINK      "\033[38;2;245;194;231m"
#define CLR_MAUVE     "\033[38;2;203;166;247m"
#define CLR_RED       "\033[38;2;243;139;168m"
#define CLR_PEACH     "\033[38;2;250;179;135m"
#define CLR_YELLOW    "\033[38;2;249;226;175m"
#define CLR_GREEN     "\033[38;2;166;227;161m"
#define CLR_TEAL      "\033[38;2;148;226;213m"
#define CLR_SKY       "\033[38;2;137;220;235m"
#define CLR_SAPPHIRE  "\033[38;2;116;199;236m"
#define CLR_BLUE      "\033[38;2;137;180;250m"
#define CLR_LAVENDER  "\033[38;2;180;190;254m"
#define CLR_TEXT      "\033[38;2;205;214;244m"
#define CLR_SUBTEXT   "\033[38;2;166;173;200m"
#define CLR_OVERLAY   "\033[38;2;108;112;134m"
#define CLR_RESET     "\033[0m"

// --- Layout Definitions ---
typedef struct {
    int header_row;
    int cat_row;
    int menu_row;
    int footer_row;
} TerminalLayout;

static const TerminalLayout g_layout = { 2, 6, 10, 18 };

// --- State Helpers ---
static inline void enter_alt_screen() { printf("\033[?1049h\033[H"); }
static inline void exit_alt_screen()  { printf("\033[?1049l"); }
static inline void hide_cursor()      { printf("\033[?25l"); }
static inline void show_cursor()      { printf("\033[?25h"); }
static inline void move_cursor(int r, int c) { printf("\033[%d;%dH", r, c); }
static inline void clear_screen()     { printf("\033[2J\033[H"); }

typedef enum { KEY_UP = 1000, KEY_DOWN, KEY_ENTER, KEY_BACKSPACE, KEY_ESC, KEY_NONE } KeyCode;

static inline KeyCode get_key_nb() {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) {
            unsigned char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    if (seq[1] == 'A') return KEY_UP;
                    if (seq[1] == 'B') return KEY_DOWN;
                }
            }
            return KEY_ESC;
        }
        if (c == 10 || c == 13) return KEY_ENTER;
        if (c == 127 || c == 8) return KEY_BACKSPACE;
        return c;
    }
    return KEY_NONE;
}

// --- Rendering ---

static inline void draw_header() {
    move_cursor(g_layout.header_row, 1);
    printf(CLR_MAUVE "      ╭───────────────────────────────────────────╮\n");
    printf("      │                 " CLR_PINK "KeyNotes" CLR_MAUVE "                  │\n");
    printf("      ╰───────────────────────────────────────────╯" CLR_RESET);
}

static inline void draw_animated_cat(int frame, int react_timer) {
    int cycle = (frame / 3) % 200; 
    int walk_cycle = (frame / 2) % 4;
    
    int offset = 0; // Fixed position as requested

    const char* face = g_cat.face_normal;
    const char* legs = g_cat.legs_stand;
    const char* bubble = " ( Moshi moshi! ) ";

    if (react_timer > 0) {
        face = g_cat.face_surprise;
        bubble = " (  Nya?! ! ! )   ";
    } else {
        int e_cycle = cycle % 100;
        if (e_cycle > 60 && e_cycle < 65) face = g_cat.face_blink;
        else if (e_cycle > 80 && e_cycle < 95) { face = g_cat.face_happy; bubble = " ( Purrr... ✨ )  "; }
        if (offset != 0) legs = (walk_cycle % 2 == 0) ? g_cat.legs_walk1 : g_cat.legs_walk2;
    }

    char pad[64]; memset(pad, ' ', 18 + offset); pad[18 + offset] = '\0';
    move_cursor(g_layout.cat_row, 1);
    printf("%s" CLR_SUBTEXT " /\\_/\\   " CLR_PINK "%s" CLR_RESET "\033[K\n", pad, bubble);
    printf("%s" CLR_SUBTEXT "%s /" CLR_RESET "\033[K\n", pad, face);
    printf("%s" CLR_SUBTEXT " %s" CLR_RESET "\033[K", pad, legs);
}

static inline int interactive_menu(const char* subtitle, const char* options[], int count) {
    int selected = 0;
    int frame = 0;
    int react_timer = 0;
    hide_cursor();
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    clear_screen();
    unsigned char dummy;
    while (read(STDIN_FILENO, &dummy, 1) > 0); // Flush buffer before starting
    while (1) {
        draw_header();
        draw_animated_cat(frame++, react_timer);
        if (react_timer > 0) react_timer--;
        move_cursor(g_layout.menu_row, 1);
        printf(CLR_TEXT "      %s\n\n" CLR_RESET, subtitle);
        for (int i = 0; i < count; i++) {
            if (i == selected) printf("      " CLR_MAUVE "➜ " CLR_PINK " %-20s" CLR_RESET "\033[K\n", options[i]);
            else printf("        " CLR_OVERLAY "  %-20s" CLR_RESET "\033[K\n", options[i]);
        }
        move_cursor(g_layout.footer_row, 1);
        printf(CLR_OVERLAY "        %s" CLR_RESET "\033[K", g_strings.menu_footer);
        fflush(stdout);
        KeyCode key = get_key_nb();
        if (key != KEY_NONE) {
            react_timer = 10;
            if (key == KEY_UP) selected = (selected <= 0) ? count - 1 : selected - 1;
            else if (key == KEY_DOWN) selected = (selected >= count - 1) ? 0 : selected + 1;
            else if (key == KEY_ENTER) {
                fcntl(STDIN_FILENO, F_SETFL, oldf);
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                show_cursor();
                return selected + 1;
            }
        }
        usleep(40000);
    }
}

static inline void show_loading(const char* msg) {
    const char* frames[] = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };
    for (int i = 0; i < 15; i++) {
        move_cursor(g_layout.menu_row + 2, 15);
        printf(CLR_MAUVE "%s  " CLR_TEXT "%s..." CLR_RESET "      ", frames[i % 10], msg);
        fflush(stdout); usleep(80000);
    }
}

static inline void show_toast(const char* msg, const char* color) {
    move_cursor(g_layout.footer_row + 1, 8);
    printf("%s[ %s ]" CLR_RESET "\033[K", color, msg);
    fflush(stdout);
    usleep(800000);
}

#endif
