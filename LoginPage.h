#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<string.h>
#include<time.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

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
    ch[strlen(ch) - 1] = '\0';
}

void generateUsername(char email[50], char username[50])
{
    
    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@') break;
        else username[i] = email[i];
    }
    return;
}

void takepassword(char pwd[50])
{
    int i = 0; // Initialize 'i' to 0
    char ch;
    while (1){
        ch = getch();
        if (ch == ENTER || ch == TAB)
        {
            pwd[i] = '\0';
            break;
        }
        else if (ch == BCKSPC)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            pwd[i++] = ch;
            printf("* \b");
        }
    }
}

void signup()
{
    FILE *fp;
    struct user user;
    char password2[50];

    printf("\nEnter your full name:\t");
    takeinput(user.fullName);
    printf("Enter your email:\t");
    takeinput(user.email);
    printf("Enter your contact no:\t");
    takeinput(user.phone);
    printf("Enter your password:\t");
    takepassword(user.password);
    printf("\nConfirm your password:\t");
    takepassword(password2);

    if (!strcmp(user.password, password2))
    {
        generateUsername(user.email, user.username);
        fp = fopen("Users.dat", "a+");
        fwrite(&user, sizeof(struct user), 1, fp);
        if (fwrite != 0) {
            printf("\n\nUser registration success.\nYour username is %s.\nGetting Started....\n", user.username);
        delay(1000);
        }
        else 
            printf("\n\nSorry! Something went wrong :(");
        fclose(fp);
    }
    else
    {
        printf("\n\nPassword don't match");
        Beep(750, 300);
    }

}


char* login(char *userFileName)
{
    FILE *fp;
    struct user usr;
    int usrFound = 0;
    char username[50], pword[50];

    printf("\nEnter your username:\t");
    takeinput(username);
    printf("Enter your password:\t");
    takepassword(pword);

    fp = fopen("Users.dat", "r");
    if (fp == NULL) {
        perror("Error opening file.\n");
        return NULL;
    }

    while (fread(&usr, sizeof(struct user), 1, fp))
    {
        if (!strcmp(usr.username, username))
        {
            if (!strcmp(usr.password, pword))
            {
                printf("\n\t\t\t\t\t\tWelcome to To-Do List %s. We are currently getting started", usr.fullName);
                strcpy(userFileName, usr.username);
                sprintf(userFileName, "%s.txt", userFileName);
                fclose(fp);
                return userFileName;
            }
            else 
            {
                printf("\n\nInvalid Password!\n");
                Beep(800, 300);
                fclose(fp);
                return NULL;
            }
            usrFound = 1;
        }
    }

    if (!usrFound)
    {
        printf("\n\nUser is not registered!");
        Beep(800, 300);
    }

    fclose(fp);
    return NULL;
}


void forgotPassword() 
{
    char username[50], email[50], phone[50], fullName[50], newPassword[50], pwd[50];
    FILE *fp, *temp;
    struct user usr;

    int usrFound = 0;

    printf("\nEnter your email:\t");
    takeinput(email);
    printf("\nEnter your username:\t");
    takeinput(username);
    printf("\nEnter your Full Name:\t");
    takeinput(fullName);
    printf("\nEnter your phone number:\t");
    takeinput(phone);

    fp = fopen("Users.dat", "r");
    temp = fopen("temp.dat", "w");

    if (fp == NULL || temp == NULL) 
    {
        perror("Error while accessing files.\n");
    } else {
        while (fread(&usr, sizeof(struct user), 1, fp)) 
        {
            if (!strcmp(usr.username, username) && !strcmp(usr.email, email) &&
                !strcmp(usr.fullName, fullName) && !strcmp(usr.phone, phone)) 
                {
                printf("\n\t\t\t\t\t\tWelcome %s", usr.fullName);
                printf("\n\nFull Name:\t%s", usr.fullName);
                printf("\nEmail:\t\t%s", usr.email);
                printf("\nUsername:\t%s", usr.username);
                printf("\nContact no.:\t%s", usr.phone);
                printf("\nEnter New Password: ");
                takepassword(newPassword);
                printf("\nConfirm Password: ");
                takepassword(pwd);
                if (!strcmp(newPassword, pwd))
                    strcpy(usr.password, newPassword);
                else {
                    printf("\n\nPasswords don't match.");
                    Beep(800, 300);
                }

                usrFound = 1;  // Set usrFound to 1 to indicate an issue
            }
            fwrite(&usr, sizeof(struct user), 1, temp);
        }
        if (!usrFound) {
            printf("\n\nUser not found or information provided is incorrect!");
            Beep(800, 300);
        }

        fclose(fp);
        fclose(temp);

        remove("Users.dat");
        rename("temp.dat", "Users.dat");
    }
}




