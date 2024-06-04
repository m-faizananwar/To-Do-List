#include "../include/assets.h"
#include <stdio.h>
#include <string.h>

AppStrings g_strings;
CatFrames g_cat;

static void trim(char* s) {
    char* p = s + strlen(s) - 1;
    while (p >= s && (*p == '\n' || *p == '\r' || *p == ' ')) *p-- = '\0';
}

void load_assets() {
    FILE* f;
    char line[256];

    // Load Strings
    f = fopen("assets/strings.txt", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char* val = strchr(line, ':');
            if (!val) continue;
            *val++ = '\0'; trim(val);
            if (strcmp(line, "APP_NAME") == 0) strcpy(g_strings.app_name, val);
            else if (strcmp(line, "MSG_WELCOME") == 0) strcpy(g_strings.msg_welcome, val);
            else if (strcmp(line, "MSG_EXIT") == 0) strcpy(g_strings.msg_exit, val);
            else if (strcmp(line, "MSG_RETRY") == 0) strcpy(g_strings.msg_retry, val);
            else if (strcmp(line, "MENU_FOOTER") == 0) strcpy(g_strings.menu_footer, val);
        }
        fclose(f);
    }

    // Load Cat Frames
    f = fopen("assets/cat_anim.txt", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char* val = strchr(line, ':');
            if (!val) continue;
            *val++ = '\0'; trim(val);
            if (strcmp(line, "FACE_NORMAL") == 0) strcpy(g_cat.face_normal, val);
            else if (strcmp(line, "FACE_BLINK") == 0) strcpy(g_cat.face_blink, val);
            else if (strcmp(line, "FACE_HAPPY") == 0) strcpy(g_cat.face_happy, val);
            else if (strcmp(line, "FACE_LOVE") == 0) strcpy(g_cat.face_love, val);
            else if (strcmp(line, "FACE_SURPRISE") == 0) strcpy(g_cat.face_surprise, val);
            else if (strcmp(line, "LEGS_STAND") == 0) strcpy(g_cat.legs_stand, val);
            else if (strcmp(line, "LEGS_WALK1") == 0) strcpy(g_cat.legs_walk1, val);
            else if (strcmp(line, "LEGS_WALK2") == 0) strcpy(g_cat.legs_walk2, val);
        }
        fclose(f);
    }
}
