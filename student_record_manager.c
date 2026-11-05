#include <stdio.h>
#include <string.h>
#define MAX 100
#define SUBJECTS 5
struct Student
{
    int roll;
    char name[50];
    float marks[SUBJECTS];
};
// 1. Search Student
int searchStudent(struct Student students[], int count, int roll)
{
    for (int i = 0; i < count; i++)
    {
        if (students[i].roll == roll)
        {
            return i;
        }
    }

    return -1;
}
// 2. Calculate Percentage
float calculatePercentage(struct Student s)
{
    float total = 0;

    for (int i = 0; i < SUBJECTS; i++)
    {
        total += s.marks[i];
    }

    return total / SUBJECTS;
}
// 3. Calculate Grade
char calculateGrade(float percentage)
{
    if (percentage >= 90)
        return 'A';

    else if (percentage >= 80)
        return 'B';

    else if (percentage >= 70)
        return 'C';

    else if (percentage >= 60)
        return 'D';

    else
        return 'F';
}
// 4. Add Student
void addStudent(struct Student students[], int *count)
{
    if (*count >= MAX)
    {
        printf("Student limit reached!\n");
        return;
    }

    int roll;

    printf("Enter roll number: ");

    if (scanf("%d", &roll) != 1)
    {
        printf("Invalid roll number!\n");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        return;
    }

    // Check for duplicate roll number
    if (searchStudent(students, *count, roll) != -1)
    {
        printf("Roll number already exists!\n");
        return;
    }

    students[*count].roll = roll;

    printf("Enter student name: ");
    getchar();

    if (fgets(students[*count].name, 50, stdin) == NULL)
    {
        printf("Could not read name!\n");
        return;
    }

    students[*count].name[
        strcspn(students[*count].name, "\n")
    ] = '\0';

    // Enter marks
    for (int i = 0; i < SUBJECTS; i++)
    {
        do
        {
            printf("Enter marks for subject %d: ", i + 1);

            if (scanf("%f", &students[*count].marks[i]) != 1)
            {
                printf("Invalid input!\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                students[*count].marks[i] = -1;
                continue;
            }

            if (students[*count].marks[i] < 0 ||
                students[*count].marks[i] > 100)
            {
                printf("Marks must be between 0 and 100!\n");
            }

        } while (students[*count].marks[i] < 0 ||
                 students[*count].marks[i] > 100);
    }

    (*count)++;

    printf("Student added successfully!\n");
}


// 5. Display Students
void displayStudents(struct Student students[], int count)
{
    if (count == 0)
    {
        printf("No student records available!\n");
        return;
    }

    printf("\n-------- STUDENT RECORDS --------\n");

    for (int i = 0; i < count; i++)
    {
        float percentage = calculatePercentage(students[i]);

        printf("\nRoll Number: %d\n", students[i].roll);
        printf("Name: %s\n", students[i].name);
        printf("Percentage: %.2f%%\n", percentage);
        printf("Grade: %c\n", calculateGrade(percentage));
    }
}


// 6. Update Student
void updateStudent(struct Student students[], int count)
{
    int roll;

    printf("Enter roll number to update: ");
    scanf("%d", &roll);

    int index = searchStudent(students, count, roll);

    if (index == -1)
    {
        printf("Student not found!\n");
        return;
    }

    printf("Enter new name: ");
    getchar();

    fgets(students[index].name, 50, stdin);

    students[index].name[
        strcspn(students[index].name, "\n")
    ] = '\0';

    for (int i = 0; i < SUBJECTS; i++)
    {
        do
        {
            printf("Enter new marks for subject %d: ", i + 1);

            if (scanf("%f", &students[index].marks[i]) != 1)
            {
                printf("Invalid input!\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                students[index].marks[i] = -1;
                continue;
            }

            if (students[index].marks[i] < 0 ||
                students[index].marks[i] > 100)
            {
                printf("Marks must be between 0 and 100!\n");
            }

        } while (students[index].marks[i] < 0 ||
                 students[index].marks[i] > 100);
    }

    printf("Record updated successfully!\n");
}


// 7. Delete Student
void deleteStudent(struct Student students[], int *count)
{
    int roll;

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    int index = searchStudent(students, *count, roll);

    if (index == -1)
    {
        printf("Student not found!\n");
        return;
    }

    // Shift remaining records
    for (int i = index; i < *count - 1; i++)
    {
        students[i] = students[i + 1];
    }

    (*count)--;

    printf("Student deleted successfully!\n");
}


// 8. Sort Students by Percentage
void sortStudents(struct Student students[], int count)
{
    struct Student temp;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (calculatePercentage(students[j]) <
                calculatePercentage(students[j + 1]))
            {
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("Students sorted by percentage!\n");
}


// 9. Load Students from File
void loadStudents(struct Student students[], int *count)
{
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        return;
    }

    *count = 0;

    while (*count < MAX)
    {
        struct Student s;

        if (fscanf(fp, "%d\n", &s.roll) != 1)
            break;

        if (fgets(s.name, 50, fp) == NULL)
            break;

        s.name[strcspn(s.name, "\n")] = '\0';

        int valid = 1;

        for (int i = 0; i < SUBJECTS; i++)
        {
            if (fscanf(fp, "%f", &s.marks[i]) != 1 ||
                s.marks[i] < 0 || s.marks[i] > 100)
            {
                valid = 0;
                break;
            }
        }

        if (!valid)
            break;

        students[*count] = s;
        (*count)++;
    }

    fclose(fp);
}


// 10. Save Students to File
void saveStudents(struct Student students[], int count)
{
    FILE *fp = fopen("students.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d\n", students[i].roll);
        fprintf(fp, "%s\n", students[i].name);

        for (int j = 0; j < SUBJECTS; j++)
        {
            fprintf(fp, "%.2f ", students[i].marks[j]);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}


// 11. Main Function
int main()
{
    struct Student students[MAX];
    int count = 0;
    int choice;

    // Load previously saved records
    loadStudents(students, &count);

    do
    {
        printf("\n================================\n");
        printf("    STUDENT RECORD MANAGER\n");
        printf("================================\n");

        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students\n");
        printf("7. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                addStudent(students, &count);
                saveStudents(students, count);
                break;

            case 2:
                displayStudents(students, count);
                break;

            case 3:
            {
                int roll;

                printf("Enter roll number to search: ");
                scanf("%d", &roll);

                int index = searchStudent(students, count, roll);

                if (index == -1)
                {
                    printf("Student not found!\n");
                }
                else
                {
                    printf("\nStudent Found!\n");
                    printf("Roll Number: %d\n", students[index].roll);
                    printf("Name: %s\n", students[index].name);

                    float percentage =
                        calculatePercentage(students[index]);

                    printf("Percentage: %.2f%%\n", percentage);
                    printf("Grade: %c\n", calculateGrade(percentage));
                }

                break;
            }

            case 4:
                updateStudent(students, count);
                saveStudents(students, count);
                break;

            case 5:
                deleteStudent(students, &count);
                saveStudents(students, count);
                break;

            case 6:
                sortStudents(students, count);
                saveStudents(students, count);
                break;

            case 7:
                saveStudents(students, count);
                printf("Records saved. Exiting program!\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}
