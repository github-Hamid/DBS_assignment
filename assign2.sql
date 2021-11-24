/*CREATE TABLE login_table (
username VARCHAR(255) NOT NULL,
password VARCHAR(255) NOT NULL
);*/
SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE reg_username(user_name IN login_table.username%TYPE, pass IN login_table.password%TYPE,
status OUT VARCHAR) AS
user login_table.username%TYPE;
BEGIN
SELECT 
   username
INTO
   user
FROM
   login_table
WHERE
   username = user_name;
IF SQL%FOUND THEN
   status := 'the username is exists on database.';
END IF;
EXCEPTION
WHEN NO_DATA_FOUND
   THEN
INSERT INTO login_table 
VALUES (user_name, pass);
   status := 'OK';  
   COMMIT;
WHEN OTHERS
  THEN
DBMS_OUTPUT.PUT_LINE('Error!');
END reg_username;




CREATE OR REPLACE PROCEDURE Update_password(user_name IN login_table.username%TYPE, pass IN login_table.password%TYPE,
status OUT VARCHAR2) AS
usr login_table.username%TYPE;
BEGIN
SELECT
   username
INTO
   usr
FROM
   login_table
WHERE
   username = user_name;
IF SQL%FOUND
  THEN
UPDATE login_table
SET  
    password = pass
WHERE
    username = user_name;
    status := 'OK';
END IF;   
COMMIT;
EXCEPTION
WHEN NO_DATA_FOUND 
   THEN
 status := 'the username does not exist in database';
WHEN OTHERS
   THEN
DBMS_OUTPUT.PUT_LINE('ERROR!');   
END Update_password;




CREATE OR REPLACE PROCEDURE Find_employee(num IN employees.employee_id%TYPE, cur OUT SYS_REFCURSOR, status OUT INTEGER) AS
emp_id employees.employee_id%TYPE;
BEGIN
SELECT
  employee_id
INTO
  emp_id
FROM
  employees
WHERE
   employee_id = num; 
    status := 1;
OPEN cur FOR
SELECT
     first_name, 
     last_name,
     email,
     phone,
     job_title
FROM
     employees
WHERE
     employee_id = num;
         
EXCEPTION
WHEN NO_DATA_FOUND
   THEN
status := 0;   
WHEN OTHERS
    THEN
    DBMS_OUTPUT.PUT_LINE('ERROR!');    
END Find_employee;


/*CREATE OR REPLACE PROCEDURE Employees_report(num IN employees.manager_id%TYPE, cur OUT SYS_REFCURSOR) AS
BEGIN
OPEN cur FOR
SELECT
   first_name,
   last_name, 
   job_title
FROM
   employees
WHERE
   manager_id = num;
EXCEPTION
WHEN OTHERS
   THEN
   DBMS_OUTPUT.PUT_LINE('ERROR!'); 
END Employees_report;*/


CREATE OR REPLACE PROCEDURE Add_employee(emp_id IN employees.employee_id%TYPE , fn IN employees.first_name%TYPE, 
ln IN employees.last_name%TYPE, mail IN employees.email%TYPE, phoneNum IN employees.phone%type, hireDate IN employees.email%TYPE,
 mID IN employees.employee_id%TYPE, jobTitle IN employees.job_title%type) AS
BEGIN
IF mID <> 0
  THEN
INSERT INTO
employees VALUES ( 
emp_id, fn, ln, mail, phoneNum, TO_DATE(hireDate,'MM/DD/YYYY'), mID, jobTitle);
ELSE
INSERT INTO
  employees(employee_id, first_name, last_name, email, phone, hire_date, job_title)
VALUES(emp_id, fn, ln, mail, phoneNum, TO_DATE(hireDate,'MM/DD/YYYY'), jobTitle);  
END IF;
COMMIT;
END add_employee;

CREATE OR REPLACE PROCEDURE Update_employee(emp_id IN employees.employee_id%TYPE , fn IN employees.first_name%TYPE, 
ln IN employees.last_name%TYPE, mail IN employees.email%TYPE, phoneNum IN employees.phone%type, hireDate IN employees.email%TYPE,
 mID IN employees.manager_id%TYPE, jobTitle IN employees.job_title%type, numSelected IN employees.employee_id%TYPE) AS

BEGIN
CASE numSelected
WHEN 1 THEN
UPDATE employees
SET first_name = fn
WHERE employee_id = emp_id;
WHEN 2 THEN
UPDATE employees
SET last_name = ln
WHERE employee_id = emp_id;
WHEN 3 THEN
UPDATE employees
SET email = mail
WHERE employee_id = emp_id;
WHEN 4 THEN
UPDATE employees
SET phone = phoneNum
WHERE employee_id = emp_id;
WHEN 5 THEN
UPDATE employees
SET hire_date = TO_DATE(hireDate,'MM/DD/YYYY')
WHERE employee_id = emp_id;
WHEN 6 THEN
UPDATE employees
SET manager_id = mID
WHERE employee_id = emp_id;
WHEN 7 THEN
UPDATE employees
SET job_title = jobTitle
WHERE employee_id = emp_id;
END CASE;
COMMIT;
END Update_employee;


CREATE OR REPLACE PROCEDURE remove_employee(num IN employees.employee_id%TYPE) AS
BEGIN
DELETE FROM
   employees
WHERE
   employee_id = num;
COMMIT;   
EXCEPTION
WHEN OTHERS
   THEN
    DBMS_OUTPUT.PUT_LINE('ERROR!');
END remove_employee;


/*CREATE OR REPLACE PROCEDURE Check_user_login(user IN login_table.username%TYPE, pass IN login_table.password%TYPE,
status OUT  VARCHAR2) AS
user_name login_table.username%TYPE := '';
user_password login_table.password%TYPE := '';
step1 NUMBER := 0;
BEGIN
SELECT 
  username
INTO
  user_name
FROM
  login_table
WHERE
  username = user;
step1 := 1;
SELECT
   password
INTO
   user_password
FROM
   login_table
WHERE
   username = user AND password = pass;
status := 'OK';
EXCEPTION
WHEN NO_DATA_FOUND
   THEN
IF step1 = 0
   THEN
   status := 'Invalid username';
ELSIF step1 = 1
    THEN
    status := 'Incorrect password';
END IF;
WHEN OTHERS
    THEN
    DBMS_OUTPUT.PUT_LINE('ERROR!');
END Check_user_login;*/



