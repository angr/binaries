#include <stdio.h>

// Define a struct to hold student information
typedef struct {
    int id;
    char name[100];
    float grade;
} Student;

// Function to print student information
void printStudent(Student s) {
    printf("Student ID: %d\n", s.id);
    printf("Student Name: %s\n", s.name);
    printf("Student Grade: %.2f\n", s.grade);
}

// Function to modify and print student information
void modifyAndPrintStudent(Student s) {
    s.grade += 10.0;  // Modify the grade by adding 10 points
    printStudent(s);  // Print the modified student details
}

void long_func(int a, int b, int c, int d, int e, int f, int g, int h, char *name) {
    int sum = a + b + c + d + e + f + g + h;
    printf("Here's your sum %s: %d", name, sum);
}

int main() {
    // Initialize a Student struct
    Student stu = {1, "John Doe", 85.5};
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    // Print original student information
    printf("Original student information:\n");
    printStudent(stu);

    // Modify and print the student information by passing by value
    printf("\nModified student information inside the function:\n");
    modifyAndPrintStudent(stu);

    // Print again to show that the original data hasn't changed
    printf("\nOriginal student information after function call:\n");
    printStudent(stu);

    long_func(a, b, c, d, e, f, g, h, stu.name);

    return 0;
}
