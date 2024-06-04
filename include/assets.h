#ifndef ASSETS_H
#define ASSETS_H

typedef struct {
    char app_name[50];
    char msg_welcome[100];
    char msg_exit[100];
    char msg_retry[100];
    char menu_footer[100];
} AppStrings;

typedef struct {
    char face_normal[20];
    char face_blink[20];
    char face_happy[20];
    char face_love[20];
    char face_surprise[20];
    char legs_stand[20];
    char legs_walk1[20];
    char legs_walk2[20];
} CatFrames;

extern AppStrings g_strings;
extern CatFrames g_cat;

void load_assets();

#endif
