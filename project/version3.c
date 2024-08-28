#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student_node {
    int studentID;
    float marks;
    struct student_node* next;
} student;

typedef struct teacher_node {
    int id;
    char name[40];
    char pass[12];
    struct teacher_node* next;
    struct teacher_node* prev;
    struct student_node* students;
} teacher;

typedef struct Student_node {
    int studentID;
    char pass[12];
    struct Student_node* next;
} Student;

int teacherCount = 0;
int studentCount = 0;
int nextID = 1; // Start teacher IDs from 1
int nextStudentID = 1;
teacher* head = NULL;
Student* studentHead = NULL;

// Function to display students' marks for a teacher
void displayStudentMarks(teacher* currentTeacher) {
    printf("Student Marks for Teacher %s (ID: %d):\n", currentTeacher->name, currentTeacher->id);

    if (currentTeacher->students == NULL) {
        printf("No students' marks available.\n");
        return;
    }

    student* currentStudent = currentTeacher->students;
    do {
        printf("Student ID: %d, Marks: %.2f\n", currentStudent->studentID, currentStudent->marks);
        currentStudent = currentStudent->next;
    } while (currentStudent != currentTeacher->students);
}

// Function to add student marks for a teacher
void addStudentMarks(teacher* currentTeacher) {
    student* newStudent = (student*)malloc(sizeof(student));

    printf("Enter student ID: ");
    scanf("%d", &newStudent->studentID);

    printf("Enter student marks: ");
    scanf("%f", &newStudent->marks);

    newStudent->next = NULL;

    if (currentTeacher->students == NULL) {
        currentTeacher->students = newStudent;
        newStudent->next = newStudent;
    } else {
        newStudent->next = currentTeacher->students;
        student* temp = currentTeacher->students;
        while (temp->next != currentTeacher->students) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }

    printf("Student marks added successfully!\n");

    // Write student marks to file
    FILE* file = fopen("student_marks.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %d %.2f\n", currentTeacher->id, newStudent->studentID, newStudent->marks);
        fclose(file);
    } else {
        printf("Error writing to file.\n");
    }
}

// Function to register a teacher
void registerTeacher() {
    teacher* newTeacher = (teacher*)malloc(sizeof(teacher));

    printf("Enter your name: ");
    scanf("%s", newTeacher->name);

    printf("Enter your password: ");
    scanf("%s", newTeacher->pass);

    newTeacher->id = nextID++;
    newTeacher->next = NULL;
    newTeacher->prev = NULL;
    newTeacher->students = NULL;

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
}

// Function to log in as a teacher
teacher* loginTeacher() {
    if (head == NULL) {
        printf("No teachers registered. Please register first.\n");
        return NULL;
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
            return current;
        }
        current = current->next;
    } while (current != head);

    printf("Login failed. Invalid ID or password.\n");
    return NULL;
}

// Function to display student marks for a student
void displayStudentMarksForStudent(student* currentStudent) {
    printf("Your Marks:\n");
    printf("Marks: %.2f\n", currentStudent->marks);
}

// Function to log in as a student
student* loginStudent() {
    if (studentHead == NULL) {
        printf("No students registered.\n");
        return NULL;
    }

    int inputID;
    char inputPass[12];

    printf("Enter your ID: ");
    scanf("%d", &inputID);

    printf("Enter your password: ");
    scanf("%s", inputPass);

    Student* current = studentHead;
    do {
        if (current->studentID == inputID && strcmp(current->pass, inputPass) == 0) {
            printf("Login successful! Welcome, Student ID: %d!\n", current->studentID);
            return NULL;
        }
        current = current->next;
    } while (current != studentHead);

    printf("Login failed. Invalid ID or password.\n");
    return NULL;
}

// Function to initialize teachers from file
void initializeTeachersFromFile() {
    FILE* file = fopen("teachers_data.txt", "r");
    if (file != NULL) {
        while (!feof(file)) {
            teacher* newTeacher = (teacher*)malloc(sizeof(teacher));
            int tempID;
            if (fscanf(file, "%d", &tempID) == 1) {
                newTeacher->id = tempID;
                fscanf(file, "%s %s", newTeacher->name, newTeacher->pass);
                newTeacher->next = NULL;
                newTeacher->prev = NULL;
                newTeacher->students = NULL;

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

    // Initialize student marks from file
    file = fopen("student_marks.txt", "r");
    if (file != NULL) {
        while (!feof(file)) {
            int teacherID, studentID;
            float marks;
            if (fscanf(file, "%d %d %f", &teacherID, &studentID, &marks) == 3) {
                teacher* currentTeacher = head;
                while (currentTeacher != NULL && currentTeacher->id != teacherID) {
                    currentTeacher = currentTeacher->next;
                }

                if (currentTeacher != NULL) {
                    student* newStudent = (student*)malloc(sizeof(student));
                    newStudent->studentID = studentID;
                    newStudent->marks = marks;

                    if (currentTeacher->students == NULL) {
                        currentTeacher->students = newStudent;
                        newStudent->next = newStudent;
                    } else {
                        newStudent->next = currentTeacher->students;
                        student* temp = currentTeacher->students;
                        while (temp->next != currentTeacher->students) {
                            temp = temp->next;
                        }
                        temp->next = newStudent;
                    }
                }
            }
        }
        fclose(file);
    }
}

int main() {
    // Initialize teachers and student marks from files on program start
    initializeTeachersFromFile();

    int choice;
    Student* newStudent;

    do {
        printf("\n1. Register as Teacher\n");
        printf("2. Login as Teacher\n");
        printf("3. Register as Student\n");
        printf("4. Login as Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerTeacher();
                break;
            case 2: {
                teacher* loggedInTeacher = loginTeacher();
                if (loggedInTeacher != NULL) {
                    // Teacher-only features
                    int teacherChoice;
                    do {
                        printf("\n1. Add Student Marks\n");
                        printf("2. Display Student Marks\n");
                        printf("3. Back to Main Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &teacherChoice);

                        switch (teacherChoice) {
                            case 1:
                                addStudentMarks(loggedInTeacher);
                                break;
                            case 2:
                                displayStudentMarks(loggedInTeacher);
                                break;
                            case 3:
                                printf("Returning to the main menu.\n");
                                break;
                            default:
                                printf("Invalid choice. Please try again.\n");
                        }
                    } while (teacherChoice != 3);
                }
                break;
            }
            case 3:
                // Student registration (for simplicity, just store student ID and password)
                
                newStudent= (Student*)malloc(sizeof(Student));
                newStudent->studentID = nextStudentID++;
                printf("Enter your password: ");
                scanf("%s", newStudent->pass);
                newStudent->next = NULL;

                if (studentHead == NULL) {
                    studentHead = newStudent;
                    newStudent->next = newStudent;
                } else {
                    newStudent->next = studentHead;
                    Student* temp = studentHead;
                    while (temp->next != studentHead) {
                        temp = temp->next;
                    }
                    temp->next = newStudent;
                }
                studentCount++;
                printf("Registration successful! Your Student ID is: %d\n", newStudent->studentID);
                break;
            case 4: {
                student* loggedInStudent = loginStudent();
                if (loggedInStudent != NULL) {
                    // Student-only features
                    displayStudentMarksForStudent(loggedInStudent);
                }
                break;
            }
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    // Free allocated memory before exiting
    teacher* currentTeacher = head;
    while (teacherCount > 0) {
        student* currentStudent = currentTeacher->students;
        while (currentTeacher->students != NULL) {
            student* tempStudent = currentStudent;
            currentStudent = currentStudent->next;
            free(tempStudent);
        }

        teacher* tempTeacher = currentTeacher;
        currentTeacher = currentTeacher->next;
        free(tempTeacher);
        teacherCount--;
    }

    Student* currentStudentLogin = studentHead;
    while (studentCount > 0) {
        Student* tempStudentLogin = currentStudentLogin;
        currentStudentLogin = currentStudentLogin->next;
        free(tempStudentLogin);
        studentCount--;
    }

    return 0;
}
