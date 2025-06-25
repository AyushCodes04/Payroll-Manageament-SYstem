📌 Features
 Login System – Secure access for authorized users.
 Add Employee – Insert employee details with department and working hours.
 Delete Employee – Remove employees by ID.
 Display Employees – List all employees in a formatted table.

💸 Generate Payslip – Auto-calculates:
Basic Salary
HRA (20%)
PF (5%)
Overtime Pay (1.5x hourly rate)
Net & Gross Salary

Search Employee – Lookup employee by ID.

File Storage – All data is saved persistently in Employees.txt.

🛠️ Technologies Used
Language: C
File Handling: .txt file for data persistence
Terminal: Cross-platform support for Windows/Linux
Additional Libraries: <stdio.h>, <stdlib.h>, <string.h>, <ctype.h>, conditional use of <windows.h> or <unistd.h>

🧮 Salary Calculation Logic
Hourly Rate depends on department:
HR: ₹200/hour
Finance: ₹250/hour
IT: ₹300/hour
Others: ₹150/hour
Overtime applies for hours above 160/month at 1.5× rate.
