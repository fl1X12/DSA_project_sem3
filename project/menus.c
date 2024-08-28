#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct teacher_node {
    int id;
    char name[40];
    char pass[12];
    struct teacher_node* next;
    struct teacher_node* prev;
} teacher;

int teacherCount = 0;
int nextID = 1; // Start teacher IDs from 1
teacher* head = NULL;

// Function to display teachers
void displayTeachers() {
    if (head == NULL) {
        printf("No teachers registered.\n");
        return;
    }

    teacher* current = head;
    do {
        printf("ID: %d, Name: %s\n", current->id, current->name);
        current = current->next;
    } while (current != head);
}

// Function to register a teacher
void registerTeacher() {
    teacher* newTeacher = (teacher*)malloc(sizeof(teacher));

    printf("Enter your name: ");
    scanf("%s", newTeacher->name);

    printf("Enter your password: ");
    scanf("%s", newTeacher->pass);

    newTeacher->id = nextID++;

    if (head == NULL) {
        head = newTeacher;
        newTeacher->next = newTeacher;
        newTeacher->prev = newTeacher;
    } else {
        newTeacher->next = head;
        newTeacher->prev = head->prev;
        head->prev->next = newTeacher;
        head->prev = newTeacher;
    }

    teacherCount++;
    printf("Registration successful! Your ID is: %d\n", newTeacher->id);

    // Write teacher information to file
    FILE* file = fopen("teachers_data.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %s %s\n", newTeacher->id, newTeacher->name, newTeacher->pass);
        fclose(file);
    } else {
        printf("Error writing to file.\n");
    }
}

// Function to log in
void login() {
    if (head == NULL) {
        printf("No teachers registered. Please register first.\n");
        return;
    }

    int inputID;
    char inputPass[12];

    printf("Enter your ID: ");
    scanf("%d", &inputID);

    printf("Enter your password: ");
    scanf("%s", inputPass);

    teacher* current = head;
    do {
        if (current->id == inputID && strcmp(current->pass, inputPass) == 0) {
            printf("Login successful! Welcome, %s!\n", current->name);
            return;
        }
        current = current->next;
    } while (current != head);

    printf("Login failed. Invalid ID or password.\n");
}

// Function to initialize teachers from file
void init_teachers() {
    FILE* file = fopen("teachers_data.txt", "r");
    if (file != NULL) {
        while (!feof(file)) {
            teacher* newTeacher = (teacher*)malloc(sizeof(teacher));
            int tempID;
            if (fscanf(file, "%d", &tempID) == 1) {
                newTeacher->id = tempID;
                fscanf(file, "%s %s", newTeacher->name, newTeacher->pass);

                if (head == NULL) {
                    head = newTeacher;
                    newTeacher->next = newTeacher;
                    newTeacher->prev = newTeacher;
                } else {
                    newTeacher->next = head;
                    newTeacher->prev = head->prev;
                    head->prev->next = newTeacher;
                    head->prev = newTeacher;
                }

                teacherCount++;
            } else {
                free(newTeacher);
            }
        }
        fclose(file);
    }
}

int main() {
    // Initialize teachers from file on program start
    init_teachers();

    int choice;

    do {
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Display Teachers\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerTeacher();
                break;
            case 2:
                login();
                break;
            case 3:
                displayTeachers();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    // Free allocated memory before exiting
    teacher* current = head;
    while (teacherCount > 0) {
        teacher* temp = current;
        current = current->next;
        free(temp);
        teacherCount--;
    }

    return 0;
}
