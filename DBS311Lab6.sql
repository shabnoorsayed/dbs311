-- ***********************
-- Name: Shabnoor Sayed
-- ID: 133649194
-- Date: 11/20/2020
-- Purpose: Lab 6 DBS311
-- ***********************

--1 The company wants to calculate what the employees’ annual salary would be:
--Do NOT change any salaries in the table.
--Assume that the starting salary or sometimes called base salary was $10,000.
--Every year of employment after that, the salary increases by 5%.
--
--Write a stored procedure named calculate_salary which gets an employee ID from the user and for that employee, calculates the salary based on the number of years the employee has been working in the company.  (Use a loop construct the calculation of the salary).
--The procedure calculates and prints the salary.
--Sample output:
--First Name: first_name 
--Last Name: last_name
--Salary: $9999,99
--If the employee does not exist, the procedure displays a proper message.


CREATE OR replace PROCEDURE Calculate_salary(employee_id IN NUMBER) 
IS 
  salary    NUMBER := 10000; 
  yearcount NUMBER; 
  firstname VARCHAR(20); 
  lastname  VARCHAR(20); 
  i         INT := 0; 
BEGIN 
    SELECT Trunc(To_char(SYSDATE - employees.hire_date) / 365) 
    INTO   yearcount 
    FROM   employees 
    WHERE  employees.employee_id = Calculate_salary.employee_id; 

    SELECT employees.first_name 
    INTO   firstname 
    FROM   employees 
    WHERE  employees.employee_id = Calculate_salary.employee_id; 

    SELECT employees.last_name 
    INTO   lastname 
    FROM   employees 
    WHERE  employees.employee_id = Calculate_salary.employee_id; 

    LOOP 
        salary := salary * 1.05; 

        i := i + 1; 

        EXIT WHEN i = yearcount; 
    END LOOP; 

    dbms_output.Put_line('First Name: ' 
                         ||firstname); 

    dbms_output.Put_line('Last Name: ' 
                         ||lastname); 

    dbms_output.Put_line('Salary: ' 
                         ||salary); 
EXCEPTION 
  WHEN no_data_found THEN 
             dbms_output.Put_line('No Data Found'); 
END; 

/ 
/*Sample Execution*/ 
BEGIN 
    Calculate_salary(0); 
END; 

--//Question2//
--2 Write a stored procedure named employee_works_here to print the employee_id, employee Last name and department name.
--
--This is sample output
--Employee #		Last Name		Department Name
--9999			Able			Manufacturing
--9998			Notsoable		Shipping
--If the value of the department name is null or does not exist, display “no department”.
--The value of employee ID ranges from your Oracle id's last 2 digits  (ex: dbs311_203g37 would use 37)
--to employee 105.
-- (NOTE: Check manually and not in the procedure, to see if your number is in the employee table. If not pick the first employee number higher that does exist)
--
--Since you are looping there will be missing employee numbers. At that stage you can get out of the loop that displays the data about each employee.

CREATE PROCEDURE employee_works_here 
IS
    employee_id employees.employee_id % type;
    last_name employees.last_name % type;
    department_name departmets.department_name % type;
BEGIN
   FOR i IN 1..9 loop 
   SELECT
      employees.employee_id,
      employees.last_name,
      departmets.department_name INTO 
      employee_id,
      last_name,
      department_name 
   FROM
      employees  
      INNER JOIN
         departments  
         ON (employees.department_id = departmets.department_id) 
   WHERE
      employees.employee_id = i;
IF department_name IS NULL 
THEN
   department_name := 'no department';
END IF;
dbms_output.put_line('Employee Number: ' || employee_id);
dbms_output.put_line('Last Name: ' || last_name);
dbms_output.put_line('Department Name: ' || department_name);
dbms_output.put_line('');
END
loop;
exception 
WHEN
   others 
THEN
   dbms_output.put_line('Error Occured');
END;

BEGIN
   employee_works_here();
END;