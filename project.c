#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct driver {
    char *surname;
    char *team;
    int hour, minute, second, totalSeconds;
    struct driver *next;
} Driver;

Driver *head = NULL;




void addDriver(char *surname, char *team) {
    Driver *current = head;
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            printf("Driver \"%s\" is already in the database.\n", surname);
            return;
        }
        current = current->next;
    }

    Driver *newDriver = (Driver *) malloc(sizeof(Driver));
    
    newDriver->surname = malloc(strlen(surname) + 1);
    strcpy(newDriver->surname, surname);
    newDriver->team = malloc(strlen(team) + 1);
    strcpy(newDriver->team, team);
    
    newDriver->hour = 0;
    newDriver->minute = 0;
    newDriver->second = 0;
    newDriver->totalSeconds = 0;
    
    newDriver->next = head;
    head = newDriver;

    printf("SUCCESS\n");
}




void addDriverList(char *surname, char *team, int hour, int minute, int second) {
    Driver *newDriver = (Driver *) malloc(sizeof(Driver));
    newDriver->surname = malloc(strlen(surname) + 1);
    strcpy(newDriver->surname, surname);
    newDriver->team = malloc(strlen(team) + 1);
    strcpy(newDriver->team, team);
    
    newDriver->hour = hour;
    newDriver->minute = minute;
    newDriver->second = second;
    newDriver->totalSeconds = hour*3600 + minute*60 + second;
    newDriver->next = NULL;
    
    if (head == NULL) {
        head = newDriver;
        return;
    }

    Driver *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newDriver;    
}




void updateTime(char *surname, int hour, int minute, int second) {
    Driver *current = head;
    while(current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            int totalSeconds = hour*3600 + minute*60 + second;
            current->totalSeconds += totalSeconds;
            current->hour += totalSeconds / 3600;
            current->minute += totalSeconds % 3600 / 60;
            current->second += totalSeconds % 3600 % 60;

            if (current->second >= 60) {
                current->minute += current->second / 60;
                current->second %= 60;
            }

            if (current->minute >= 60) {
                current->hour += current->minute / 60;
                current->minute %= 60;
            }
            printf("SUCCESS\n");
            return;
        }
        current = current->next;
    }
    printf("ERROR: Driver not found.\n");
}




void printRace() {
    if (head == NULL) {
        printf("ERROR: No drivers found.\n");
        return;
    }

    Driver *current = head;

    while (current != NULL) {
        printf("%s %s %u %u %u\n", current->surname, current->team, current->hour, current->minute, current->second);
        current = current->next;
    }
    printf("SUCCESS\n");
}




void saveToFile(char *filename) {
    FILE *fp;
    fp = fopen(filename, "w");
    Driver *current = head;

    while (current != NULL) {
        fprintf(fp, "%s %s %u %u %u\n", current->surname, current->team, current->hour, current->minute, current->second);
        current = current->next;
    }

    fclose(fp);
    printf("SUCCESS\n");
}




void loadFromFile(char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s for reading.\n", filename);
        return;
    }
    char buffer[1000];
    char surname[1000];
    char team[1000];
    int hour, minute, second;

    freeList();
    
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%s %s %d %d %d", surname, team, &hour, &minute, &second);
        addDriverList(surname, team, hour, minute, second);
    }

    fclose(fp);
    printf("SUCCESS\n");
}




void freeList() {
    Driver *current = head;
    Driver *next;
    while (current != NULL) {
        next = current->next;
        free(current->surname);
        free(current->team);
        free(current);
        current = next;
    }
    head = NULL;
}




int main(void) {

    char input[1000];
    char surname[1000];
    char team[1000];
    int hour, minute, second;
    char filename[1000];
    int status = 1;


    while (status) {
        fgets(input, sizeof(input), stdin);
        char command = input[0];

        switch (command) {
            case 'Q':
                freeList();
                status = 0;
                printf("SUCCESS\n");
                break;

            case 'A':
                if (sscanf(input, "%c %s %s", &command, surname, team) == 3) {
                    addDriver(surname, team);
                } else {
                    printf("A should be followed by exactly 2 arguments.\n");
                }
                break;

            case 'U':
                if (sscanf(input, "%c %s %d %d %d", &command, surname, &hour, &minute, &second) != 5){
                    printf("U should be followed by exactly 4 arguments.\n");
                    break;
                }
                if (hour < 0) {
                    printf("Hour cannot be negative.\n");
                    break;
                } else if (minute < 0 || minute > 59) {
                    printf("Minute cannot be negative or greater than 59.\n");
                    break;
                } else if (second < 0 || second > 59) {
                    printf("Second cannot be negative or greater than 59.\n");
                    break;
                } else {
                    updateTime(surname, hour, minute, second);
                    break;
                }

            case 'L':
                printRace();
                break;

            case 'W':
                sscanf(input, "%c %s", &command, filename);
                saveToFile(filename);
                break;

            case 'O':
                sscanf(input, "%c %s", &command, filename);
                loadFromFile(filename);
                break;

            default:
                printf("Invalid command %c\n", command);
        }
    }
}