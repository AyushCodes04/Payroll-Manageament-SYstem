#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Clear terminal screen 
void clearScreen(){
    system("cls"); 
}

// Cross-platform sleep function
void sleep_seconds(int seconds){
    #ifdef _WIN32
    Sleep(seconds * 1000);  // Windows uses milliseconds
    #else
    sleep(seconds);         // Unix/Linux uses seconds
    #endif
}

// Structure to hold employee information
struct emp{
    int id;
    char name[50];
    int age;
    char department[50];
    int hoursWorked;
    char doj[50]; 
}s1[100]; 

int EmployeeCount();
int AddEmployee(int employeeCount);
int DeleteEmployee(int employeeCount);
void Display(int employeeCount);
int GeneratePayslips();
void searchEmployee();
int Login();

// Global variable to track employee count
int employeeCount;

int main(){
    // Authenticate user before allowing access
    if(!Login()){
        printf("Access denied. Exiting...\n");
        return 1;
    }

    // Get current employee count from file
    employeeCount = EmployeeCount();
    int choice;

    while(1){
        clearScreen();
        printf("\n========================================\n");
        printf("        Payroll Management System       \n");
        printf("========================================\n");
        printf("\n1. Add Employee");
        printf("\n2. Delete Employee");
        printf("\n3. Display Employees");
        printf("\n4. Generate Payslips");
        printf("\n5. Search Employee");
        printf("\n6. Exit\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                employeeCount = AddEmployee(employeeCount);
                break;
            case 2:
                employeeCount = DeleteEmployee(employeeCount);
                break;
            case 3:
                Display(employeeCount);
                break;
            case 4:
                GeneratePayslips();
                break;
            case 5:
                searchEmployee();
                break;
            case 6:
                printf("Exiting program!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        // Wait for user input before showing menu again
        printf("\nPress Enter to continue...");
        getchar(); getchar();  // Capture newline
    }
}

// Count number of employees in the file
int EmployeeCount(){
    FILE *fp = fopen("Employees.txt", "r");
    if(fp == NULL){
        return 0;
    }
    char line[300];
    int count = 0;
    while(fgets(line, sizeof(line), fp)!= NULL){
        count++;
    }
    fclose(fp);
    return count;
}
// Add a new employee 
int AddEmployee(int employeeCount){
    printf("Enter the following details for the new employee:\n");
    
    printf("ID: ");
    scanf("%d", &s1[employeeCount].id);

    printf("Name: ");
    getchar();
    fgets(s1[employeeCount].name, sizeof(s1[employeeCount].name), stdin);
    s1[employeeCount].name[strcspn(s1[employeeCount].name, "\n")] = '\0'; // Remove newline

    printf("Age: ");
    scanf("%d", &s1[employeeCount].age);

    printf("Department: ");
    getchar();
    fgets(s1[employeeCount].department, sizeof(s1[employeeCount].department), stdin);
    s1[employeeCount].department[strcspn(s1[employeeCount].department, "\n")] = '\0'; // Remove newline

    for(int i = 0; s1[employeeCount].department[i]; i++){
        s1[employeeCount].department[i] = tolower(s1[employeeCount].department[i]);
    }

    printf("Working hours (monthly): ");
    scanf("%d", &s1[employeeCount].hoursWorked);

    printf("Date of Joining (DD-MM-YYYY): ");
    scanf("%s", s1[employeeCount].doj);

    FILE *fp = fopen("Employees.txt", "a");
    if(fp == NULL){
        printf("File not opened\n");
        return employeeCount;
    }
    fprintf(fp, "ID: %d, Name: %s, Age: %d, Department: %s, HoursWorked: %d, DOJ: %s\n",// Write employee detail into file
            s1[employeeCount].id, s1[employeeCount].name, s1[employeeCount].age,
            s1[employeeCount].department, s1[employeeCount].hoursWorked,
            s1[employeeCount].doj);
    fclose(fp);
    printf("Employee added successfully!\n");
    return employeeCount + 1;
}

// Delete employee record 
int DeleteEmployee(int employeeCount){
    int delId, found = 0;
    struct emp temp[100];
    int count = 0;
    char line[300];
    
    FILE *fp = fopen("Employees.txt", "r");
    if(fp == NULL){
        printf("Error opening file.\n");
        return employeeCount;
    }

    // Load existing records into a temp array
    while(fgets(line, sizeof(line), fp) != NULL){  //Reads the data line by line
        int id, age, hoursWorked;
        char name[50], department[50], doj[50];
        if(sscanf(line, "ID: %d, Name: %49[^,], Age: %d, Department: %49[^,], HoursWorked: %d, DOJ: %49[^\n]",
                   &id, name, &age, department, &hoursWorked, doj) == 6) {
            temp[count].id = id;
            strcpy(temp[count].name, name);
            temp[count].age = age;
            strcpy(temp[count].department, department);
            temp[count].hoursWorked = hoursWorked;
            strcpy(temp[count].doj, doj);
            count++;
        }
    }
    fclose(fp);

    if(count == 0){
        printf("No records found.\n");
        return employeeCount;
    }

    printf("Enter Employee ID to delete: ");
    scanf("%d", &delId);

    // Search and remove employee
    for(int i = 0; i < count; i++){
        if (temp[i].id == delId) {
            found = 1;
            for(int j = i; j < count - 1; j++){
                temp[j] = temp[j + 1];
            }
            count--;
            break;
        }
    }

    if(!found){
        printf("Employee ID not found.\n");
        return employeeCount;
    }

    // Rewrite file without the deleted employee
    fp = fopen("Employees.txt", "w");
    if(fp == NULL){
        printf("Error opening file for writing.\n");
        return employeeCount;
    }

    for(int i = 0; i < count; i++){
        fprintf(fp, "ID: %d, Name: %s, Age: %d, Department: %s, HoursWorked: %d, DOJ: %s\n",
                temp[i].id, temp[i].name, temp[i].age, temp[i].department,
                temp[i].hoursWorked, temp[i].doj);
    }
    fclose(fp);
    printf("Employee record deleted successfully.\n");
    return count;
}

// Display all employees
void Display(int employeeCount){
    FILE *fp = fopen("Employees.txt", "r");
    if(fp == NULL){
        printf("File not opened\n");
        return;
    }
    char line[300];
    int count = 0;
    printf("\n----------Employee Records ----------\n");
    printf("%-5s %-15s %-5s %-15s %-15s %-12s\n", "ID", "Name", "Age", "Department", "Hours Worked", "DOJ");
    printf("----------------------------------------------------------------------\n");

    while(fgets(line, sizeof(line), fp) != NULL){
        int id, age, hoursWorked;
        char name[50], department[50], doj[50];

        if(sscanf(line, "ID: %d, Name: %49[^,], Age: %d, Department: %49[^,], HoursWorked: %d, DOJ: %49[^\n]",
                   &id, name, &age, department, &hoursWorked, doj) == 6){
            printf("%-5d %-15s %-5d %-15s %-15d %-12s\n", id, name, age, department, hoursWorked, doj);
            count++;
        }
    }

    if(count == 0){
        printf("No employees found!\n");
    }
    fclose(fp);
}

// Generate and display payslips
int GeneratePayslips(){
    FILE *fp = fopen("Employees.txt", "r");
    if(fp == NULL){
        printf("Error opening file\n");
        return 0;
    }

    // loading effect
    printf("Generating payslips");
    for(int i = 0; i < 3; i++){
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    char line[300];
    int count = 0;

     printf("\n%-5s %-15s %-5s %-12s %-12s %-11s %-11s %-11s %-11s %-14s %-12s %-11s\n",
       "ID", "Name", "Age", "Dept", "DOJ", "Basic", "HRA", "PF", "Gross", "Overtime Pay", "Net");
      while(fgets(line, sizeof(line), fp) != NULL){
        int id, age, hoursWorked;
        char name[50], department[50], doj[50];

        if(sscanf(line, "ID: %d, Name: %49[^,], Age: %d, Department: %49[^,], HoursWorked: %d, DOJ: %49[^\n]",
                   &id, name, &age, department, &hoursWorked, doj) == 6){

            for(int i = 0; department[i]; i++) 
            department[i] = tolower(department[i]);

            float hourlyRate = 0.0f;
            if(strcmp(department, "hr") == 0) hourlyRate = 200.0f;
            else if(strcmp(department, "finance") == 0) hourlyRate = 250.0f;
            else if(strcmp(department, "it") == 0) hourlyRate = 300.0f;
            else hourlyRate = 150.0f;

            int normalHours = hoursWorked > 160 ? 160 : hoursWorked;
            int overtimeHours = hoursWorked > 160 ? hoursWorked - 160 : 0;

            float basicSalary = normalHours * hourlyRate;
            float overtimePay = overtimeHours * hourlyRate * 1.5f;
            float hra = 0.20f * basicSalary;
            float pf = 0.05f * (basicSalary + overtimePay);
            float gross = basicSalary + hra + overtimePay;
            float net = gross - pf;

            printf("%-5d %-15s %-5d %-12s %-12s Rs%-9.2f Rs%-9.2f Rs%-9.2f Rs%-9.2f Rs%-10.2f Rs%-9.2f\n",
                      id, name, age, department, doj,basicSalary, hra, pf, gross, overtimePay, net);    
            count++;
        }
    }
    fclose(fp);
    if(count == 0){
        printf("No employee records found.\n");
    }
    return count;
}

// Search for employee by ID
void searchEmployee(){
    int searchId, found = 0;
    char line[300];

    FILE *fp = fopen("Employees.txt", "r");
    if(fp == NULL){
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Employee ID to search: ");
    scanf("%d", &searchId);
    
    printf("Searching");
    for(int i = 0; i < 3; i++){
        printf(".");
        fflush(stdout);
        sleep_seconds(1);
    }
    printf("\n");

    while(fgets(line, sizeof(line), fp) != NULL){
        int id, age, hoursWorked;
        char name[50], department[50], doj[50];

        if(sscanf(line, "ID: %d, Name: %49[^,], Age: %d, Department: %49[^,], HoursWorked: %d, DOJ: %49[^\n]",
                   &id, name, &age, department, &hoursWorked, doj) == 6){
            if(id == searchId){
                printf("\nEmployee Found!\n");
                printf("ID: %d\nName: %s\nAge: %d\nDepartment: %s\nHours Worked: %d\nDate of Joining: %s\n",
                       id, name, age, department, hoursWorked, doj);
                found = 1;
                break;
            }
        }
    }

    if(!found){
        printf("Employee with ID %d not found.\n", searchId);
    }

    fclose(fp);
}

// Simple login validation
int Login(){
    const char correctUsername[] = "admin";
    const char correctPassword[] = "12345";
    char username[50], password[50];

    printf("------ Login ------\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if(strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0){
        printf("Login successful. Welcome, %s!\n", username);
        return 1;
    }
    else{
        printf("Login failed! Invalid credentials.\n");
        return 0;
    }
}