#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct student_node {
    int studentID;
    char pass[12];
    float marks;
    struct student_node* next;
} student;

int studentCount = 0;
int nextID = 1; // Start teacher IDs from 1
int nextStudentID = 1; // Start student IDs from 1
student* studentHead = NULL;

int main(){
    FILE *file;
    file = fopen("student_marks.txt", "r");
    if (file != NULL) {
        while (!feof(file)) {
            int teacherID, studentID;
            float marks;
            char pass[12];
            if (fscanf(file, "%d %d %f %s", &teacherID, &studentID, &marks, pass) == 4) {
                // Student ID is unique, so use it directly without checking for teacher ID
                printf("%d %d %f %s \n",teacherID,studentID,marks,pass);
                student* newStudent = (student*)malloc(sizeof(student));
                newStudent->studentID = studentID;
                newStudent->marks = marks;
                strcpy(newStudent->pass, pass);

                if (studentHead == NULL) {
                    studentHead = newStudent;
                    newStudent->next = newStudent;
                } else {
                    //newStudent->next = studentHead;
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
        student* temp=studentHead;
        while(temp!=NULL){
            printf("%d %f\n",temp->studentID,temp->marks);
            temp=temp->next;
        }
    }
    return 0;
}