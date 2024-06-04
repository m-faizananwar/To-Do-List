#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "LoginPage.h"



#define MAX_TASKS 100                                    // Total Tasks that can be stored
#define TASK_NAME_LENGTH 1000                            // Tasks length limit

typedef enum 
{
    ROUTINE = 1,                                        // Tasks are of two types ,routine & work
    WORK,
} 
TaskType;

FILE *user_file;

typedef struct 
{
    char name[TASK_NAME_LENGTH];                            // To store task
    int completed;                                          // To store task's status
    TaskType taskType;                                      // To store task type i.e routine or work
    char timestamp[50];                                     // To store time information
} Task;

Task *toDoList;
int noOfTasks = 0;
char userFileName[55];

char* getCurrentTime() {
    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);                                // Get the current time

    localTime = localtime(&currentTime);                     // Convert the current time to the local time

    char *timeString = asctime(localTime);                  // Store the current date and time as a string

    timeString[strcspn(timeString, "\n")] = 0;              // Remove the newline character at the end of the string

    return timeString;
}


void welcomePage() 
{
    system("cls");
    printf(" \t\t                           _________________________________________________________________________\n");
    printf(" \t\t\t                          _______  ____         _____   ____    _      _____  _____ _______             \n");
    printf(" \t\t\t                         |__   __ / __ \\       |  __ \\ / __ \\  | |    |_   _|/ ____|__   __|         \n");
    printf(" \t\t\t                            | |  | |  | |______| |  | | |  | | | |      | | | (___    | |               \n");
    printf(" \t\t\t                            | |  | |  | |______| |  | | |  | | | |      | |  \\___ \\   | |             \n");
    printf(" \t\t\t                            | |  | |__| |      | |__| | |__| | | |____ _| |_ ____) |  | |               \n");
    printf(" \t\t\t                            |_|   \\____/       |_____/ \\____/  |______|_____|_____/   |_|             \n");
    printf(" \t\t                          _________________________________________________________________________\n\n\n\n");
}

void moveToNextPage() {
    puts("\nPress any key to continue.... : ");
    getch();
}




void initializeArrays() 
{
    toDoList = (Task *)malloc(MAX_TASKS * sizeof(Task));
}

void freeArrays() 
{
    free(toDoList);
}

int safeInputInt(int variable,int lowerlimit, int upperlimit)       // Takes safe input of integer and if entered value is within scope
{

    while (1) 
    {
        variable = getch() - '0';                                   // Converts acsii to integer
        if(variable >= lowerlimit && variable <= upperlimit )
        {
            printf("%d\n",variable);
            return variable;
        }
        printf("%c\n\nInvalid Input.\n\nEnter an integer (between %d and %d): ",(char)(variable + '0'),lowerlimit,upperlimit);
    }
    
}


int countTasksInFile(FILE *file) 
{   
    file = fopen(userFileName,"r");
    if (file == NULL) perror("\n\nError while opening Files.");
    rewind(file);                                                   // Move file pointer to the beginning of the file

    Task task;
    int taskCount = 0;
    printf("\n");

    while (fread(&task, sizeof(Task), 1, file) )                   // Storing data from file to in-function memory
    {
        toDoList[taskCount] = task;
        taskCount++;
    }
    fclose(file);
    return taskCount;
}



void updateNoOfTasks() 
{
    noOfTasks = countTasksInFile(user_file);                      // Update no of tasks in tasks counter
}



void LoginPage(char *userFileName)                                // Login Page Function
{
    int option;
    while(option != 4) {
        welcomePage();                                            // Calling the welcome page for user-friendly purposes
        printf("\n\t\t\t\t\t\t\t\t----------Getting Started----------\t\t\t\t");
        printf("\n\nPlease choose your operation\n\n");
        printf("\n\n1.\tSign up");
        printf("\n\n2.\tLogin");
        printf("\n\n3.\tForgot Password");
        printf("\n\n4.\tExit");

        printf("\n\nNOTE:   You must login to enter the To-Do List ");

        printf("\n\nYour choice:\t");
        option = safeInputInt(option,1,4);
        

        switch (option)
        {
            case 1:
                signup();
                break;
            case 2:
                    user_file = fopen(login(userFileName), "a+");       // Open file in append mode
                if (strlen(userFileName) != 0) 
                {
                    fclose(user_file);                                  // Login successful, break out of the loop
                    return;
                }
                break;
            case 3:
                forgotPassword();
                break;
            case 4:
                printf("\nBye Bye :)");
                exit(0);
            default:
                printf("\nInvalid option. Please enter a valid option.\n");
                break;
            }
            moveToNextPage();
    }
    return;
}


int taskTypeFilter() 
{
    int filter;
    printf("\n\nEnter task type (1 for Routine, 2 for Work, 0 for all): ");
    scanf("%d", &filter);
    getchar();                                                              // to remove the \n character from the buffer
    if (filter != 1 && filter != 2 && filter != 0) 
    {
        printf("\n\nInvalid type selected!");
        return taskTypeFilter();                                            // Return the result of the recursive call
    }
    return filter;
}


void addTask() 
{
    updateNoOfTasks();
    while(1) 
    {
        int tasksNumber, type, choice;
        welcomePage();
        
        printf("\n\n1.  Add Tasks\n\n2.  Back");
        printf("\n\nEnter your choice : ");
        choice = safeInputInt(choice,1,2);
        
        

        if (choice == 2)
            break;
        else {
            printf("\n\nCurrently, there are %d tasks in to-do list.\n", noOfTasks);
            printf("\n\nEnter task type (1 for Routine, 2 for Work): ");
            type = safeInputInt(type,1,2);
        
            printf("\n\nEnter number of tasks to be added (%d can be added) : ",MAX_TASKS -noOfTasks);
            scanf("%d", &tasksNumber);
            getchar();


            if (tasksNumber <= 0 || noOfTasks + tasksNumber > MAX_TASKS) 
            {
                printf("\n\nInvalid Input or too many tasks. Delete some tasks to continue!...\n\n");
                moveToNextPage();
                return;
            } else {

                user_file = fopen(userFileName,"a+");
                if (user_file == NULL) {
                    perror("\n\nError while opening files.");
                    moveToNextPage();
                    return;
                }
                else{                                                   // Successful opening of file
                for (int i = noOfTasks; i < noOfTasks + tasksNumber; i++) 
                {
                    printf("\n\nEnter Task %d: ", i + 1);
                    fgets(toDoList[i].name, TASK_NAME_LENGTH, stdin);
                    toDoList[i].name[strlen(toDoList[i].name)-1]='\0';   // Removing new line character
                    printf("\n\nTask added at: %s\n", getCurrentTime());
                    strcpy(toDoList[i].timestamp,getCurrentTime());
                    
                    toDoList[i].taskType = type;

                    toDoList[i].completed = 0;
                    fwrite(&toDoList[i], sizeof(Task), 1, user_file);
                    
                }
                noOfTasks += tasksNumber;
                printf("\n\nAdding tasks...\n");
                delay(1000);
                printf("\n\nTasks added successfully.\n");
                fclose(user_file);
             }
            }
        }
    } 
    moveToNextPage();
}


void displayTasks(int taskType) {
    printf("\n\nLoading tasks...\n");
    delay(1000);

    user_file = fopen(userFileName, "r");
    if (user_file == NULL) {
        printf("\n\nError opening file.\n");
        moveToNextPage();
        return;
    }

    int taskCounter = 0;
    Task task;

    printf("\n\nTasks:\n\n");

    while (fread(&task, sizeof(Task), 1, user_file)) 
    {
        taskCounter++;
            if (taskType == 0 || task.taskType == (TaskType)taskType)           // Check if the task matches the specified type filter
            {
            printf("%d.\tTask : %s\t\t\tType : %s\tStatus : %s\t\tTime :  %s\n\n",
                   taskCounter,
                   task.name,
                   (task.taskType == ROUTINE ? "Routine" : "Work"),
                   (task.completed ? "Completed" : "Pending"),
                   task.timestamp);
        }
    }

    fclose(user_file);

    if (taskCounter == 0) {
        printf("No tasks available.\n");
    }
    moveToNextPage();
}



void eliminateTask() {
    
    int deleteTask;
    updateNoOfTasks();
    if (noOfTasks == 0)
    {
        printf("Add some tasks in the To-Do List to remove. Currently no tasks available");
        moveToNextPage();
        return;
    }
    printf("Enter task Number (between 1-%d): ",noOfTasks);
    scanf("%d", &deleteTask);
    getchar();                                              // to remove the '\n' character from the buffer

    if (deleteTask <= 0 || deleteTask > noOfTasks) 
    {
        printf("Invalid task number!\n");
        return;
    }

    user_file = fopen(userFileName,"r");
    rewind(user_file);                                      // Move file pointer to the beginning of the file
    FILE *tempFile = fopen("temp.txt", "w");                // Temporary file to write updated data

    Task task;
    int taskCounter = 0;

    while (fread(&task, sizeof(Task), 1, user_file) == 1) {
        if (taskCounter != deleteTask - 1) {
            fwrite(&task, sizeof(Task), 1, tempFile);
        }
        taskCounter++;
    }

    fclose(user_file);
    fclose(tempFile);

    remove(userFileName);                                   // Delete the original file
    rename("temp.txt", userFileName);                       // Rename the temporary file to the original file name

    printf("Loading...\n");
    delay(1000);
    printf("Task removed successfully.\n");

    for (int i = deleteTask - 1; i < noOfTasks - 1; i++)   // Update the in-memory task list
    {
        toDoList[i] = toDoList[i + 1];
    }
    noOfTasks--;
    moveToNextPage();
}

void editTask() 
{
    int editTask;
    if (noOfTasks == 0) {
        printf("\n\nAdd some tasks in the To-Do List to edit. Currently no tasks available");
        moveToNextPage();
        return;
    }
    
    printf("\n\nEnter task Number (between 1-%d): ", noOfTasks);
    scanf("%d", &editTask);
    getchar();                                                // to remove the '\n' character from the buffer

    if (editTask <= 0 || editTask > noOfTasks) {
        printf("\n\nInvalid task number!\n");
        return;
    }

    printf("\n\nEnter new content for task %d: ", editTask);
    fgets(toDoList[editTask - 1].name, TASK_NAME_LENGTH, stdin);
    toDoList[editTask - 1].name[strlen(toDoList[editTask - 1].name)-1]='\0';

    user_file = fopen(userFileName, "r");
    if (user_file == NULL) {
        perror("\n\nError opening file for reading");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("\n\nError opening temporary file for writing");
        fclose(user_file);
        return;
    }

    Task task;
    int taskCounter = 0;

    while (fread(&task, sizeof(Task), 1, user_file) == 1) {
        if (taskCounter == editTask - 1) {
            strcpy(task.name, toDoList[editTask - 1].name);                         // Update task 
            strcpy(task.timestamp, getCurrentTime());                               // Update task time 
        }
        fwrite(&task, sizeof(Task), 1, tempFile);
        taskCounter++;
    }

    fclose(user_file);
    fclose(tempFile);

    remove(userFileName);                                                       // Delete the original file
    rename("temp.txt", userFileName);                                           // Rename the temporary file to the original file name

    printf("Loading...\n");
    delay(1000);
    printf("\n\nTask edited successfully.\n");
    moveToNextPage();
}


int viewPendingTasks(int taskType) 
{
    int pendingTasksCount = 0;
    printf("\n\nLoading Pending tasks...\n");
    delay(1000);
    
    user_file = fopen(userFileName, "r");
    if (user_file == NULL) {
        perror("\n\nError opening file for reading");
        moveToNextPage();
        return 0;
    }

    rewind(user_file);                                                            // Move file pointer to the beginning of the file

    Task task;
    int taskCounter = 0;
    while (fread(&task, sizeof(Task), 1, user_file) == 1)  
    {
        taskCounter++;
        if ((taskType == 0 || task.taskType == taskType) && task.completed == 0) 
        {
            printf("%d.\tTask : %s\t\t\tType : %s\tStatus : %s\t\tTime :  %s\n\n",
                   taskCounter,
                   task.name,
                   (task.taskType == ROUTINE ? "Routine" : "Work"),
                   (task.completed ? "Completed" : "Pending"),
                   task.timestamp);
            pendingTasksCount++;
        }
        
    }

    if (pendingTasksCount == 0)
        printf("\n\nNo pending tasks found.\n");
    fclose(user_file);
    moveToNextPage();
    return pendingTasksCount;
}

void markCompletedTask() {
    if (noOfTasks == 0) {
        printf("\n\nAdd some tasks in the To-Do List to edit. Currently no tasks available\n");
        moveToNextPage();
        return;
    }

    printf("\n\nPending Tasks are below. Enter the task number to mark them as completed.\n");
    int pendingTask = viewPendingTasks(0);                                      // Passing 0 to view all pending tasks

    int completedTask;
    printf("\n\nEnter task Number: ");
    scanf("%d", &completedTask);
    getchar();                                                                  // to remove the '\n' character from the buffer

    if (completedTask <= 0 || completedTask > noOfTasks) {
        printf("Invalid task number!\n");
        return;
    }

    toDoList[completedTask - 1].completed = 1;

    user_file = fopen(userFileName, "r");
    if (user_file == NULL) {
        perror("\n\nError opening file for reading");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("\n\nError opening temporary file for writing");
        fclose(user_file);
        return;
    }

    Task task;
    int taskCounter = 0;

    while (fread(&task, sizeof(Task), 1, user_file) == 1) 
    {
        if (taskCounter == completedTask - 1) 
        {
            task.completed = 1;                                             // Mark the task as completed
        }
        fwrite(&task, sizeof(Task), 1, tempFile);
        taskCounter++;
    }

    fclose(user_file);
    fclose(tempFile);

    remove(userFileName);                                                   // Delete the original file
    rename("temp.txt", userFileName);                                       // Rename the temporary file to the original file name

    printf("\n\nLoading...\n");
    delay(1000);
    printf("\n\nTask marked as completed successfully.\n");
    moveToNextPage();
}


int viewCompletedTasks(int taskType) 
{
    int completedTasksCount = 0;
    printf("Loading completed tasks...\n");
    delay(1000);

    user_file = fopen(userFileName,"r");
    rewind(user_file);                                                      // Move file pointer to the beginning of the file

    Task task;
    int taskCounter = 0;
    while (fread(&task, sizeof(Task), 1, user_file) == 1) 
    {
       
        taskCounter++;
        if ((taskType == 0 || task.taskType == (TaskType)taskType) && task.completed == 1) 
        {
            completedTasksCount++;
           printf("%d.\tTask : %s\t\t\tType : %s\tStatus : %s\t\tTime :  %s\n\n",
                   taskCounter,
                   task.name,
                   (task.taskType == ROUTINE ? "Routine" : "Work"),
                   (task.completed ? "Completed" : "Pending"),
                   task.timestamp);
        }
 
    }

    if (completedTasksCount == 0)
        printf("No completed tasks found.\n");
    fclose(user_file);
    moveToNextPage();
    return completedTasksCount;
}


void allTasks() {                                                       // Leads to task operation page
    int choice;

    while (choice != 7) 
    {
        welcomePage();
        printf("\t\t\t\t\t\t\t---------------- Tasks ----------------");
        printf("\n\n1.\tView all tasks\n"
               "\n\n2.\tView Pending Tasks\n"
               "\n\n3.\tView Completed Tasks\n"
               "\n\n4.\tMark Task as Completed\n"
               "\n\n5.\tEdit Task\n"
               "\n\n6.\tRemove Task\n"
               "\n\n7.\tBack\n");

        printf("\n\nEnter your choice : ");
        choice = safeInputInt(choice,1,7);

        switch (choice) 
        {
        case 1:
            displayTasks(taskTypeFilter());
            break;
        case 2:
            viewPendingTasks(taskTypeFilter());
            break;
        case 3:
            viewCompletedTasks(taskTypeFilter());
            break;
        case 4:
            markCompletedTask();
            break;
        case 5:
            editTask();
            break;
        case 6:
            eliminateTask();
            break;
        case 7:
            return;

        default:
            printf("\n\nInvalid Choice!\n");
            break;
        }
    }

}

    typedef enum 
    {
    Black = '0',
        Blue  = '1',
        Green = '2',
        Aqua  = '3', 
        Red   = '4',
        Purple= '5',
        Yellow= '6',
        White = '7',
        Gray  = '8',
        Light_Blue  = '9',
        Light_Green ='A',
        Light_Aqua  ='B',
        Light_Red   ='C',
        Light_Purple='D',
        Light_Yellow='E',
        Bright_White='F'
} Color;


void selectColor(Color *backGroundColor) 
{
    int option;

    printf("\n\nSelect BackGround Color :"
           "\n\n0.\tBlack\n"
           "\n\n1.\tBlue\n"
           "\n\n2.\tGreen\n"
           "\n\n3.\tAqua\n"
           "\n\n4.\tRed\n"
           "\n\n5.\tPurple\n"
           "\n\n6.\tYellow\n"
           "\n\n7.\tWhite\n"
           "\n\n8.\tGray\n"
           "\n\n9.\tLight Blue\n"
           "\n\nA.\tLight Green\n"
           "\n\nB.\tLight Aqua\n"
           "\n\nC.\tLight Red\n"
           "\n\nD.\tLight Purple\n"
           "\n\nE.\tLight Yellow\n"
           "\n\nF.\tBright White\n");

    printf("\n\nEnter option : ");
    scanf("%c", &option);
    getchar();
    if ((option == '0') || (option == '1') || (option == '2') || (option == '3') || (option == '4') || (option == '5') || (option == '6') || (option == '7') || (option == '8') || (option == '9') || (option == 'A') || (option == 'B') || (option == 'C') || (option == 'D') || (option == 'E') || (option == 'F')) { 
        *backGroundColor = option;
    } else {
        printf("\n\nInvalid Option!\n");
    }
}

void settings()                                                     
{
    int option;
    Color backGroundColor, textColor;

    backGroundColor = Black;
    textColor = White;

    while (option != 3) {
        char totalColor[20] = "color ";
        welcomePage();
        printf("\n\t\t\t\t\t\t\t\t\t\t\t--------------------- Settings --------------------\n");
        printf("\n\n1.\tChange background Color\n");
        printf("\n\n2.\tChange text Color\n");
        printf("\n\n3.\tBack\n");
        printf("\n\nEnter option : ");
        option = safeInputInt(option, 1, 3);

        switch (option) {
            case 1:
                selectColor(&backGroundColor);
                break;
            case 2:
                selectColor(&textColor);
                break;
            case 3:
                return;
            default:
                break;
        }
        if (backGroundColor== textColor)
            printf("\n\nNOTE:\t\tBackground Color and Text Color can't be same.\n");
        else{
        totalColor[strlen(totalColor)] = backGroundColor;
        totalColor[strlen(totalColor)] = textColor;
        system(totalColor);
        }
        moveToNextPage();
    }

}

void displayHelp() 
{
    printf("\t\t\t\t\t\t\t---------------- Help ---------------\n\n");
    printf("\n1. Edit and Display Tasks: View, edit, and manage your to-do list tasks.\n");
    printf("\n2. Add Tasks: Add new tasks to your to-do list.\n");
    printf("\n3. Settings: Customize the appearance of the application.\n");
    printf("\n4. Help: Display this help menu.\n");
    printf("\n5. Exit: Close the program.\n");
    printf("\nNote: In the settings menu, you can change the background color and text color.\n");
    printf("\nThe background color and text color cannot be the same.\n");
    printf("\n--------------------------------------------------------------------------------\n");
}


int main() 
{
    system("color 0b");

    welcomePage();
    LoginPage(userFileName);
    initializeArrays();
    updateNoOfTasks();

    

    int option;

    do {                                                                // Menu page of TODO LIST
        welcomePage();
        delay(1000);
        printf("\t\t\t\t\t\t\t---------------- MENU ----------------\n");
        printf("\n1.\tEdit and Display Tasks\n");
        printf("\n2.\tAdd tasks\n");
        printf("\n3.\tSettings\n");
        printf("\n4.\tHelp\n");
        printf("\n5.\tExit\n");

        printf("\nEnter your option: ");
        option = safeInputInt(option,1,5);
      

        switch (option) 
        {
        case 1:
            allTasks();
            break;
        case 2:
            addTask();
            updateNoOfTasks();
            break;
        case 3:
            settings();
            break;
        case 4:
            displayHelp();
            break;
        case 5:
            printf("\n\nClosing the program...\n");
            delay(1000);
            freeArrays();
            return 0;
            default:
            printf("\n\nInvalid option. Please enter a valid option.\n");
        }
        moveToNextPage();

    } while (option != 5);

    
}
