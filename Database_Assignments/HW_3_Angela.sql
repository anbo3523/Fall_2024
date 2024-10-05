-- #1
-- Create ClassAssignment Database Schema
CREATE DATABASE ClassAssignment;

-- #2
-- Create Project Table
CREATE TABLE Project (
	project_num INT NOT NULL PRIMARY KEY,
    project_code CHAR(4),
    project_title VARCHAR(45),
    first_name VARCHAR(45),
    last_name VARCHAR(45),
    project_Budget DECIMAL(5, 2)
);

-- #3 
-- Modify project_num to auto increment
ALTER TABLE Project Modify project_num INT not NULL AUTO_INCREMENT;

-- Set the auto-increment at 10
ALTER TABLE Project AUTO_INCREMENT = 10;

-- #4
-- project_budget datatype update
ALTER TABLE Project MODIFY project_budget DECIMAL(10,2);

-- #5
-- Insert values without entering project_num
INSERT INTO  Project (project_code, project_title, first_name, last_name, project_budget)
VALUES ('PC01', 'DIA', 'John', 'Smith', 10000.99);

INSERT INTO Project (project_code, project_title, first_name, last_name, project_budget)
VALUES ('PC02', 'CHF', 'Tim', 'Cook', 12000.50);

INSERT INTO Project (project_code, project_title, first_name, last_name, project_budget)
VALUES ('PC03', 'AST', 'Rhonda', 'Smith', 8000.40);

-- #6
-- Create PayRoll table
CREATE TABLE PayRoll (
	employee_num INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    job_id INT NOT NULL,
    job_desc VARCHAR(40),
    emp_pay DECIMAL(10,2)
);

-- #7
-- i. Add constraint on emp_pay so that only value > 10,000 can be inserted.
ALTER TABLE PayRoll
ADD CONSTRAINT chk_emp_pay CHECK (emp_pay > 10000);

-- ii. Add constraint on job_desc so that default value is 'Data Analyst'
ALTER TABLE PayRoll
MODIFY job_desc VARCHAR(40) DEFAULT 'Data Analyst';

-- iii. Add a column pay_date to table
ALTER TABLE PayRoll
ADD COLUMN pay_date DATE AFTER job_desc;

-- #8
-- Add foreign key constraint in PayRoll table
ALTER TABLE PayRoll
ADD CONSTRAINT fk_job_project
FOREIGN KEY (job_id) REFERENCES Project(project_num);

-- #9
-- Insert values into PayRoll
INSERT INTO PayRoll (job_id, pay_date, emp_pay)
VALUES (10, CURRENT_DATE, 12000.99);

INSERT INTO PayRoll (job_id, pay_date, emp_pay)
VALUES (11, CURRENT_DATE, 14000.99);

INSERT INTO PayRoll (job_id, pay_date, emp_pay)
VALUES (12, CURRENT_DATE, 16000.99);

-- #10 
-- Update emp_pay for employee_num = 2 with 10% pay increase
UPDATE PayRoll
SET emp_pay = emp_pay * 1.10
WHERE employee_num = 2;

-- #11
-- Create Project_backup table from project table using bulk insert statement on for last_name Smith
CREATE TABLE Project_backup LIKE Project;
INSERT INTO Project_backup
SELECT * FROM Project
WHERE last_name = 'Smith';

-- #12
-- Create View as PayRoll_View from PayRoll table. ONLY contain job_id, job_desc, pay_date for job_id > 10
CREATE VIEW PayRoll_View AS
SELECT job_id, job_desc, pay_date
FROM PayRoll
WHERE job_id > 10;

-- #13
-- create index for pay_date
CREATE INDEX idx_pay_date ON PayRoll(pay_date);

-- #14
-- Delete data within Project table but keep table structure
TRUNCATE TABLE Project_backup;

-- #15
-- Delete script to delete a row from Project table where project_num = 10.
-- This will fail. We receive an error due to foreign key constraints. project_num is referenced in another table, PayRoll, through FK relationship.
-- Deleting this row would break the FK relationship so MySQL prevents the deletion. 
DELETE FROM Project
WHERE project_num = 10;

-- #16
-- In order to resolve the issue we are experiencing in #15, we will want to first delete the dependent records from PayRoll table.
-- Then, we can perform the row deletion in Project table.
-- Step 1: Delete dependent records in PayRoll
DELETE FROM PayRoll
WHERE job_id = 10;

-- Step 2: Delete the row from Project table
DELETE FROM Project
WHERE project_num = 10;