#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../include/auth.h"
#include "../include/ui.h"

#define MAX_TASKS 100
#define TASK_NAME_LENGTH 1000

typedef enum { ROUTINE = 1, WORK } TaskType;
FILE *user_file;
typedef struct { char name[TASK_NAME_LENGTH]; int completed; TaskType taskType; char timestamp[50]; } Task;
Task *toDoList;
int noOfTasks = 0;
char userFileName[55];

char* getCurrentTime() {
    time_t currentTime = time(NULL); struct tm *localTime = localtime(&currentTime);
    char *timeString = asctime(localTime); timeString[strcspn(timeString, "\n")] = 0;
    return timeString;
}

void moveToNextPage() { printf(CLR_OVERLAY "\n        ( Press any key to continue )" CLR_RESET); getch(); }
void initializeArrays() { toDoList = (Task *)malloc(MAX_TASKS * sizeof(Task)); }
void freeArrays() { free(toDoList); }

int countTasksInFile() {   
    FILE *file = fopen(userFileName,"r"); if (file == NULL) return 0;
    Task task; int taskCount = 0; while (fread(&task, sizeof(Task), 1, file)) taskCount++;
    fclose(file); return taskCount;
}

void updateNoOfTasks() { noOfTasks = countTasksInFile(); }

void LoginPage(char *userFileName) {
    int option = 0;
    const char* options[] = { "Create Account", "Sign In", "Recover Password", "Exit" };
    while(option != 4) {
        option = interactive_menu("Account Access", options, 4);
        switch (option) {
            case 1: clear_screen(); draw_header(); signup(); moveToNextPage(); break;
            case 2: clear_screen(); draw_header(); 
                    char* logged_in = login(userFileName);
                    if (logged_in != NULL && strlen(logged_in) > 0) { show_loading("Initializing Workspace"); return; }
                    moveToNextPage(); break;
            case 3: clear_screen(); draw_header(); forgotPassword(); moveToNextPage(); break;
            case 4: exit_alt_screen(); exit(0);
        }
    }
}

void petTheCat() {
    for (int i = 0; i < 15; i++) {
        clear_screen(); draw_header();
        const char* hand = (i % 2 == 0) ? " ( ' ' )_ " : " ( ' ' )  ";
        printf(CLR_PINK "\n        You gently pat the cat... \n" CLR_RESET);
        printf(CLR_SUBTEXT "                  /\\_/\\  " CLR_PINK "(  Purrr! <3 ) " CLR_RESET "\n");
        printf(CLR_SUBTEXT "                 ( ^.^ ) / %s" CLR_RESET "\n", hand);
        printf(CLR_SUBTEXT "                  > ^ <    " CLR_RESET "\n");
        fflush(stdout); usleep(150000);
    }
    show_toast("The cat is very happy!", CLR_GREEN);
    moveToNextPage();
}

int taskTypeFilter() {
    const char* opts[] = { "All Categories", "Routine Only", "Work Focus" };
    int choice = interactive_menu("Filter View", opts, 3);
    if (choice == 1) return 0; return choice - 1;
}

void addTask() {
    updateNoOfTasks();
    while(1) {
        const char* menu_opts[] = { "New Entry", "Dashboard" };
        int choice = interactive_menu("Add Task", menu_opts, 2);
        if (choice == 2) break;
        const char* type_opts[] = { "Routine", "Work Focus" };
        int type = interactive_menu("Category", type_opts, 2);
        clear_screen(); draw_header();
        printf(CLR_LAVENDER "        --- New Entry ---\n\n" CLR_RESET);
        int tasksNumber; printf(CLR_TEXT "        Quantity: ");
        if(scanf("%d", &tasksNumber) != 1) { getchar(); continue; }
        getchar();
        if (tasksNumber <= 0 || noOfTasks + tasksNumber > MAX_TASKS) { show_toast("Limit exceeded", CLR_RED); moveToNextPage(); return; }
        user_file = fopen(userFileName,"a+");
        for (int i = noOfTasks; i < noOfTasks + tasksNumber; i++) {
            printf(CLR_SKY "        Task #%d: " CLR_RESET, i + 1);
            fgets(toDoList[i].name, TASK_NAME_LENGTH, stdin);
            toDoList[i].name[strcspn(toDoList[i].name, "\n")] = 0;
            strcpy(toDoList[i].timestamp, getCurrentTime());
            toDoList[i].taskType = type; toDoList[i].completed = 0;
            fwrite(&toDoList[i], sizeof(Task), 1, user_file);
        }
        fclose(user_file); updateNoOfTasks(); show_loading("Indexing"); show_toast("Saved successfully", CLR_GREEN); moveToNextPage();
    }
}

void displayTasks(int taskType) {
    show_loading("Retrieving"); clear_screen(); draw_header();
    printf(CLR_LAVENDER "        --- Task Summary ---\n\n" CLR_RESET);
    user_file = fopen(userFileName, "r");
    if (!user_file) { show_toast("No records found", CLR_YELLOW); moveToNextPage(); return; }
    Task task; int i = 1; bool found = false;
    while (fread(&task, sizeof(Task), 1, user_file)) {
        if (taskType == 0 || task.taskType == (TaskType)taskType) {
            found = true;
            printf("        " CLR_MAUVE "%d. " CLR_TEXT "%-30s " CLR_RESET "%s\n", i++, task.name, task.completed ? CLR_GREEN "[DONE]" : CLR_PEACH "[TODO]");
            printf("          " CLR_OVERLAY "└─ %s | %s\n\n" CLR_RESET, task.taskType == ROUTINE ? "Routine" : "Work", task.timestamp);
        }
    }
    fclose(user_file); if (!found) printf(CLR_SUBTEXT "        No matching tasks found.\n" CLR_RESET);
    moveToNextPage();
}

void markCompletedTask() {
    updateNoOfTasks(); if (noOfTasks == 0) { show_toast("List empty", CLR_YELLOW); moveToNextPage(); return; }
    clear_screen(); draw_header(); printf(CLR_TEXT "        Index to mark done: ");
    int idx; scanf("%d", &idx); getchar();
    if (idx <= 0 || idx > noOfTasks) { show_toast("Invalid index", CLR_RED); moveToNextPage(); return; }
    FILE *f = fopen(userFileName, "r"); FILE *t = fopen("temp.dat", "w"); Task task; int i = 1;
    while (fread(&task, sizeof(Task), 1, f)) { if (i++ == idx) task.completed = 1; fwrite(&task, sizeof(Task), 1, t); }
    fclose(f); fclose(t); remove(userFileName); rename("temp.dat", userFileName); show_toast("Task completed", CLR_GREEN); moveToNextPage();
}

void eliminateTask() {
    updateNoOfTasks(); if (noOfTasks == 0) { show_toast("Nothing to delete", CLR_YELLOW); moveToNextPage(); return; }
    clear_screen(); draw_header(); printf(CLR_TEXT "        Index to remove: ");
    int idx; scanf("%d", &idx); getchar();
    if (idx <= 0 || idx > noOfTasks) { show_toast("Invalid index", CLR_RED); moveToNextPage(); return; }
    FILE *f = fopen(userFileName, "r"); FILE *t = fopen("temp.dat", "w"); Task task; int i = 1;
    while (fread(&task, sizeof(Task), 1, f)) { if (i++ != idx) fwrite(&task, sizeof(Task), 1, t); }
    fclose(f); fclose(t); remove(userFileName); rename("temp.dat", userFileName); show_loading("Removing"); show_toast("Entry deleted", CLR_OVERLAY); moveToNextPage();
}

void allTasks() {
    const char* opts[] = { "Summary View", "Mark Done", "Remove Entry", "Return" };
    int choice = 0;
    while (choice != 4) {
        choice = interactive_menu("Task Management", opts, 4);
        if (choice == 1) displayTasks(taskTypeFilter());
        else if (choice == 2) markCompletedTask();
        else if (choice == 3) eliminateTask();
    }
}

int main() {
    load_assets();
    enter_alt_screen(); initializeArrays(); LoginPage(userFileName);
    const char* main_options[] = { "Manage Tasks", "Add New Entry", "Pet the Cat 🐾", "Exit" };
    int option = 0;
    while (option != 4) {
        option = interactive_menu("Main Dashboard", main_options, 4);
        if (option == 1) allTasks();
        else if (option == 2) addTask();
        else if (option == 3) petTheCat();
    }
    freeArrays(); exit_alt_screen(); return 0;
}
