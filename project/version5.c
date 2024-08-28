#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

typedef struct student_node {
    int assignedTeacher;
    int studentID;
    char pass[12];
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

int teacherCount = 0;
int studentCount = 0;
int nextID = 1; // Start teacher IDs from 1
int nextStudentID = 1; // Start student IDs from 1
teacher* head = NULL;
student* studentHead = NULL;

// Function to initialize teachers and students from file
void initializeDataFromFile() {
    // Initialize teachers from file
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
            char pass[12];
            if (fscanf(file, "%d %d %f %s", &teacherID, &studentID, &marks, pass) == 4) {
                // Student ID is unique, so use it directly without checking for teacher ID
                student* newStudent = (student*)malloc(sizeof(student));
                newStudent->assignedTeacher=teacherID;
                newStudent->studentID = studentID;
                newStudent->marks = marks;
                strcpy(newStudent->pass, pass);
                if (studentHead == NULL) {
                    studentHead = newStudent;
                    newStudent->next = newStudent;
                } else {
                    newStudent->next = studentHead;
                    student* temp = studentHead;
                    while (temp->next != studentHead) {
                        temp = temp->next;
                    }
                    temp->next = newStudent;
                }

                studentCount++;
            }
        }
        fclose(file);
    }
}

void stats(){
    int sum=0,count=0;
    float mean,median,q1,q3;
    float student_marks[studentCount];
    student *temp=studentHead;
    if(temp==NULL){
        printf("no students registered");
        return;
    }
    do{
        sum=sum+temp->marks;
        temp=temp->next;
        student_marks[count]=temp->marks;
        count++;
    }while(temp!=studentHead);
    
    // sorting the marks array using selection sort
    int iol=0;
    for(int i=0;i<studentCount;i++){
        iol=i;
        for(int j=i;j<studentCount;j++){
            if(student_marks[iol]>student_marks[j]){
                iol=j;
            }
        }
        //swapping
        int num_temp=student_marks[iol];
        student_marks[iol]=student_marks[i];
        student_marks[i]=num_temp;   
    }

    int a,b,c;
    a=(studentCount+1)/2;
    b=(studentCount+1)/4;
    c=3*((studentCount+1)/4);

    if(studentCount%2!=0){
        median=student_marks[a];
        q1=student_marks[b];
        q3=student_marks[c];
    }
    else{
        median=(student_marks[a]+student_marks[a+1])/2;
        q1=(student_marks[b]+student_marks[b+1])/2;
        q3=(student_marks[c]+student_marks[c+1])/2;
    }

    mean=sum/studentCount;
    printf("class average is : %.2f\n",mean);
    printf("50 percentile is : %.2f\n",median);
    printf("25 percentile is : %.2f\n",q1);
    printf("75 percentile is : %.2f\n",q3);
}

// Function to display all students' marks for all teachers
void displayAllStudentMarks() {
    student *temp = studentHead;

    if (temp == NULL) {
        printf("No students registered.\n");
        return;
    }
    do {
        printf("Student ID:%d, Marks:%f \n", temp->studentID, temp->marks);
        temp = temp->next;
    } while (temp != studentHead);

    /*
    if (head == NULL) {
        printf("No teachers registered.\n");
        return;
    }

    teacher* currentTeacher = head;
    do {
        printf("\nStudent Marks for Teacher %s (ID: %d):\n", currentTeacher->name, currentTeacher->id);

        if (currentTeacher->students == NULL) {
            printf("No students' marks available.\n");
        } else {
            student* currentStudent = currentTeacher->students;
            do {
                printf("Student ID: %d, Marks: %.2f\n", currentStudent->studentID, currentStudent->marks);
                currentStudent = currentStudent->next;
            } while (currentStudent != currentTeacher->students);
        }

        currentTeacher = currentTeacher->next;
    } while (currentTeacher != head);
    */
}

void displayTeacherMarks(teacher *currentTeacher){
    student* temp=studentHead;
    do{
        if(temp->assignedTeacher==currentTeacher->id){
            printf("Student ID: %d, Marks: %.2f\n",temp->studentID,temp->marks);
        }
        temp=temp->next;
    }while(temp!=studentHead);
}

// Function to add student marks for a teacher with a unique student ID
void addStudentMarks(teacher* currentTeacher) {
    student* newStudent = (student*)malloc(sizeof(student));

    // Ensure unique student ID
    bool uniqueID;
    do {
        uniqueID = true;
        newStudent->studentID = nextStudentID++;
        newStudent->assignedTeacher=currentTeacher->id;

        // Check if the ID is already in use across all teachers
        student* tempStudent = studentHead;
        while (tempStudent != studentHead) {
            if (tempStudent->studentID == newStudent->studentID) {
                uniqueID = false;
                break;
            }
            tempStudent = tempStudent->next;
        }
    } while (!uniqueID);

    printf("Enter student marks: ");
    scanf("%f", &newStudent->marks);

    printf("Enter student password: ");
    scanf("%s", newStudent->pass);

    newStudent->next = NULL;
    //student* temp=currentTeacher->students;

    if (currentTeacher->students == NULL) {
        currentTeacher->students = newStudent;
        newStudent->next = newStudent;
    } else {
        //
    }
    initializeDataFromFile();
    student* temp=studentHead->next;
    do
    {
        temp=temp->next;
    }while(temp->next->studentID!=studentHead->studentID); 
    printf("\n %d ",temp->studentID);
    int tempid=temp->studentID;
    tempid++;
    newStudent->studentID=tempid;
    printf("Student marks added successfully!\n");

    // Write student marks to file
    FILE* file = fopen("student_marks.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %d %.2f %s\n",newStudent->assignedTeacher,newStudent->studentID, newStudent->marks, newStudent->pass);
        fclose(file);
    } else {
        printf("Error writing to file.\n");
    }
}


// Function to register a teacher with a unique ID
void registerTeacher() {
    teacher* newTeacher = (teacher*)malloc(sizeof(teacher));

    printf("Enter your name: ");
    scanf("%s", newTeacher->name);

    printf("Enter your password: ");
    scanf("%s", newTeacher->pass);

    // Ensure unique teacher ID
    bool uniqueID;
    do {
        uniqueID = true;
        newTeacher->id = nextID++;
        
        // Check if the ID is already in use
        teacher* temp = head;
        do {
            if (temp->id == newTeacher->id) {
                uniqueID = false;
                break;
            }
            temp = temp->next;
        } while (temp != head);
    } while (!uniqueID);

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

    // writing to the teacher file
    FILE *file=fopen("teachers_data.txt","a");
    fprintf(file,"%d %s %s\n",newTeacher->id,newTeacher->name,newTeacher->pass);
    fclose(file);
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
    stats();
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

    student* current = studentHead;
    do {
        if (current->studentID == inputID && strcmp(current->pass, inputPass) == 0) {
            printf("Login successful! Welcome, Student ID: %d!\n", current->studentID);
            return current; // Return the current student
        }
        current = current->next;
    } while (current != studentHead);

    printf("Login failed. Invalid ID or password.\n");
    return NULL;
}




int main() {
    // Initialize teachers and student marks from files on program start
    initializeDataFromFile();

    int choice;

    do {
        printf("\n1. Register as Teacher\n");
        printf("2. Login as Teacher\n");
        printf("3. Login as Student\n");
        printf("4. display all Marks\n");
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
                                displayTeacherMarks(loggedInTeacher);
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
            case 3: {
                student* loggedInStudent = loginStudent();
                if (loggedInStudent != NULL) {
                    // Student-only features
                    displayStudentMarksForStudent(loggedInStudent);
                }
                break;
            }
            case 4:
                displayAllStudentMarks();
                break;
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

    student* currentStudentLogin = studentHead;
    while (studentCount > 0) {
        student* tempStudentLogin = currentStudentLogin;
        currentStudentLogin = currentStudentLogin->next;
        free(tempStudentLogin);
        studentCount--;
    }

    return 0;
}
