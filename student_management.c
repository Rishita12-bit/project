#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[50];
    int age;
    float grade;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

void loadFromFile();
void saveToFile();
void addStudent();
void displayAll();
void searchById();
void updateStudent();
void deleteStudent();
void clearInputBuffer();

int main() {
    int choice;
    loadFromFile();

    do {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchById(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                saveToFile();
                printf("Data saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No existing data file. Starting fresh.\n");
        return;
    }
    fread(&studentCount, sizeof(int), 1, file);
    if (studentCount > MAX_STUDENTS) {
        printf("Data file corrupted.\n");
        studentCount = 0;
        fclose(file);
        return;
    }
    fread(students, sizeof(Student), studentCount, file);
    fclose(file);
    printf("Loaded %d student(s).\n", studentCount);
}

void saveToFile() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error saving file.\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
    printf("Data saved.\n");
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Maximum students reached.\n");
        return;
    }
    Student newStudent;
    printf("Enter ID: ");
    scanf("%d", &newStudent.id);
    clearInputBuffer();

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == newStudent.id) {
            printf("ID already exists.\n");
            return;
        }
    }

    printf("Enter Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &newStudent.age);
    clearInputBuffer();

    printf("Enter Grade: ");
    scanf("%f", &newStudent.grade);
    clearInputBuffer();

    students[studentCount++] = newStudent;
    printf("Student added.\n");
}

void displayAll() {
    if (studentCount == 0) {
        printf("No students.\n");
        return;
    }
    printf("\n%-10s %-30s %-5s %-6s\n", "ID", "Name", "Age", "Grade");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-30s %-5d %-6.2f\n",
               students[i].id, students[i].name,
               students[i].age, students[i].grade);
    }
}

void searchById() {
    int id;
    printf("Enter ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            printf("\nID: %d\nName: %s\nAge: %d\nGrade: %.2f\n",
                   students[i].id, students[i].name,
                   students[i].age, students[i].grade);
            return;
        }
    }
    printf("Student not found.\n");
}

void updateStudent() {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {   
            printf("Current: Name=%s, Age=%d, Grade=%.2f\n",
                   students[i].name, students[i].age, students[i].grade);

            char newName[50];
            printf("Enter new Name (or press Enter to skip): ");
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if (strlen(newName) > 0) strcpy(students[i].name, newName);

            char input[10];
            printf("Enter new Age (or Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            if (strlen(input) > 1) {
                int newAge = atoi(input);
                if (newAge > 0) students[i].age = newAge;
            }

            printf("Enter new Grade (or Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            if (strlen(input) > 1) {
                float newGrade = atof(input);
                if (newGrade >= 0 && newGrade <= 100) students[i].grade = newGrade;
            }
            printf("Student updated.\n");
            return;
        }
    }
    printf("Student not found.\n");
}

void deleteStudent() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf("Student deleted.\n");
            return;
        }
    }
    printf("Student not found.\n");
}