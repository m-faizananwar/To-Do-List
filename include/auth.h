#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include "conio.h"
#include <string.h>
#include <time.h>
#include "ui.h"

#define ENTER 13
#define LF 10
#define TAB 9
#define BCKSPC 8
#define BCKSPC_MAC 127

struct user
{
    char fullName[50];
    char email[50];
    char username[50];
    char phone[50];
    char password[50];
};

void delay(int milliseconds) 
{
    long pause;
    clock_t now, then;
    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

void takeinput(char ch[50])
{
    fgets(ch, 50, stdin);
    ch[strcspn(ch, "\n")] = 0;
}

void generateUsername(char email[50], char username[50])
{
    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@') {
            username[i] = '\0';
            break;
        }
        else username[i] = email[i];
    }
}

void takepassword(char pwd[50])
{
    int i = 0;
    int ch;
    while (1){
        ch = getch();
        if (ch == ENTER || ch == LF || ch == TAB)
        {
            pwd[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == BCKSPC || ch == BCKSPC_MAC)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            if (i < 49) {
                pwd[i++] = ch;
                printf("*");
            }
        }
    }
}

void signup()
{
    FILE *fp;
    struct user user;
    char password2[50];

    printf(CLR_ROSEWATER "        Full Name:  " CLR_RESET);
    takeinput(user.fullName);
    printf(CLR_SKY       "        Email:      " CLR_RESET);
    takeinput(user.email);
    
    char defaultUser[50];
    generateUsername(user.email, defaultUser);
    printf(CLR_BLUE      "        Username (default %s): ", defaultUser);
    takeinput(user.username);
    if (strlen(user.username) == 0) strcpy(user.username, defaultUser);

    printf(CLR_GREEN     "        Phone:      " CLR_RESET);
    takeinput(user.phone);

    while (1) {
        printf(CLR_PEACH     "        Password:   " CLR_RESET);
        takepassword(user.password);
        printf(CLR_PEACH     "        Confirm:    " CLR_RESET);
        takepassword(password2);

        if (!strcmp(user.password, password2)) {
            break;
        } else {
            printf(CLR_RED "        Error: Passwords do not match. Please try again.\n" CLR_RESET);
            printf("\a");
        }
    }

    fp = fopen("Users.dat", "a+");
    if (fwrite(&user, sizeof(struct user), 1, fp) != 0) {
        printf(CLR_GREEN "\n        Registration successful. \n        Username: %s\n" CLR_RESET, user.username);
        delay(1000);
    } else {
        printf(CLR_RED "\n        System Error: Registration failed.\n" CLR_RESET);
    }
    fclose(fp);
}

char* login(char *userFileName)
{
    FILE *fp;
    struct user usr;
    char username[50], pword[50];

    while (1) {
        printf(CLR_BLUE     "        Username:   " CLR_RESET);
        takeinput(username);
        printf(CLR_LAVENDER "        Password:   " CLR_RESET);
        takepassword(pword);

        fp = fopen("Users.dat", "r");
        if (fp == NULL) {
            printf(CLR_RED "        Error: No users found. Please register first.\n" CLR_RESET);
            return NULL;
        }

        int usrFound = 0;
        while (fread(&usr, sizeof(struct user), 1, fp)) {
            if (!strcmp(usr.username, username)) {
                usrFound = 1;
                if (!strcmp(usr.password, pword)) {
                    printf(CLR_GREEN "\n        Authenticated. Welcome back, %s.\n" CLR_RESET, usr.fullName);
                    strcpy(userFileName, usr.username);
                    strcat(userFileName, ".txt");
                    fclose(fp);
                    return userFileName;
                } else {
                    printf(CLR_RED "        Error: Invalid password. Try again.\n" CLR_RESET);
                    printf("\a");
                    break;
                }
            }
        }
        
        if (!usrFound) {
            printf(CLR_RED "        Error: Account not found. Retry or exit to Sign Up.\n" CLR_RESET);
            printf("\a");
        }
        
        fclose(fp);
        
        printf(CLR_SUBTEXT "        ( Press any key to retry or ESC to go back )\n" CLR_RESET);
        int ch = getch(); 
        if (ch == 27) return NULL; 
    }
}

void forgotPassword() 
{
    char username[50], email[50], phone[50], fullName[50], newPassword[50], pwd[50];
    FILE *fp, *temp;
    struct user usr;
    int usrFound = 0;

    printf(CLR_SKY       "        Email:      " CLR_RESET);
    takeinput(email);
    printf(CLR_BLUE      "        Username:   " CLR_RESET);
    takeinput(username);
    printf(CLR_ROSEWATER "        Full Name:  " CLR_RESET);
    takeinput(fullName);
    printf(CLR_GREEN     "        Contact #:  " CLR_RESET);
    takeinput(phone);

    fp = fopen("Users.dat", "r");
    temp = fopen("temp.dat", "w");

    if (fp == NULL || temp == NULL) 
    {
        printf(CLR_RED "        Error accessing system files.\n" CLR_RESET);
    } else {
        while (fread(&usr, sizeof(struct user), 1, fp)) 
        {
            int matches = 0;
            if (!strcmp(usr.username, username)) matches++;
            if (!strcmp(usr.email, email)) matches++;
            if (!strcmp(usr.fullName, fullName)) matches++;
            if (!strcmp(usr.phone, phone)) matches++;

            if (matches >= 3) 
            {
                printf(CLR_GREEN "\n        Identity Verified: %s" CLR_RESET, usr.fullName);
                
                while (1) {
                    printf(CLR_PEACH "\n        New Password:   " CLR_RESET);
                    takepassword(newPassword);
                    printf(CLR_PEACH "        Confirm:        " CLR_RESET);
                    takepassword(pwd);
                    if (!strcmp(newPassword, pwd)) {
                        strcpy(usr.password, newPassword);
                        printf(CLR_GREEN "\n        Credentials updated successfully.\n" CLR_RESET);
                        break;
                    } else {
                        printf(CLR_RED "        Error: Mismatch. Try again.\n" CLR_RESET);
                        printf("\a");
                    }
                }
                usrFound = 1;
            }
            fwrite(&usr, sizeof(struct user), 1, temp);
        }
        if (!usrFound) {
            printf(CLR_RED "\n        Verification failed. Incorrect details.\n" CLR_RESET);
            printf("\a");
        }

        fclose(fp);
        fclose(temp);
        remove("Users.dat");
        rename("temp.dat", "Users.dat");
    }
}

#endif
