#include <iostream>
#include <occi.h>
#include <string>
#include <utility>
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct ShoppingCart
{
	int product_id;
	double price;
	int quantity;
};

int HRmenu();
void addUser(Statement*, string, string, string&);
void resetPassword(Statement*, string, string, string&);
void loginMenu(string, string, string&,Statement*);
void findEmployee(Statement*, ResultSet*);
void employeeReport(Statement*, ResultSet*);
void addEmployee(Statement*);
void updateEmployee(Statement*);
void removeEmployee(Statement*);
int mainMenu();
int main()
{
	//write your own user and pass
	string username = "";
	string password = "";
	string constr = "";
	Environment* env = nullptr;
	Connection* conn = nullptr;
	Statement* stmt = nullptr;
	ResultSet* rs = nullptr;
	int menu, hrMenu;
	string user_name, user_password, status;
	while (1)
	{


		menu = mainMenu();
		if (menu == 0)
			exit(0);
		else
		{
			cin.ignore();
			cout << "\nEnter Username: ";
			getline(cin, user_name);
			cout << "\nEnter Password: ";
			getline(cin, user_password);
			try
			{
				env = Environment::createEnvironment(Environment::DEFAULT);
				conn = env->createConnection(username, password, constr);
				cout << "Connection is successful\n";
				stmt = conn->createStatement();
				loginMenu(user_name, user_password, status, stmt);
				if (status == "OK")
				{
					while (1)
					{
						hrMenu = HRmenu();
						switch (hrMenu)
						{
						case 0: {exit(0); break; }
						case 1: {
							findEmployee(stmt, rs);
							break;
						}
						case 2: {
							employeeReport(stmt, rs);
							break;
						}
						case 3: {
							addEmployee(stmt);
							break;
						}
						case 4: {
							updateEmployee(stmt);
							break;
						}
						case 5: {
							removeEmployee(stmt);
							break;
						}
						}
					}
				}
				else if (status == "Invalid username")
				{
					int option;
					do
					{
						cout << "*******Username does not exist in Database*******" << endl;
						cout << "1) Register New User" << endl;
						cout << "0) Return to Login menu" << endl;
						cout << "Enter an option(0-1): ";
						cin >> option;
						cin.ignore();
					} while (!(option == 1 || option == 0));
					
					if (option)
					{
						string status, user, pass;
						cout << "Enter username: ";
						getline(cin, user);
						cout << "Enter password: ";
						getline(cin, pass);
						addUser(stmt, user, pass, status);
						if (status == "OK")
						{
							cout << "User successfully added to database!" << endl;
						}
						else
						{
							cout << status << endl;
						}
					}
					
				}
				else if (status == "Incorrect password")
				{
					int option;
					do
					{
						cout << "*******" << status << "*******" << endl;
						cout << "1) Reset the password" << endl;
						cout << "0) Return to Login menu" << endl;
						cout << "Enter an option(0-1): ";
						cin >> option;
						cin.ignore();
					} while (!(option == 1 || option == 0));
					if (option)
					{
						string user, pass, status;
						cout << "Enter username: ";
						getline(cin, user);
						cout << "Enter new password: ";
						getline(cin, pass);
						resetPassword(stmt, user, pass, status);
						if (status == "OK")
							cout << "Password has been reset successfully!" << endl;
						else
							cout << status << endl;
					}
					
				}



			}
			catch (SQLException& sqlExcp)
			{
				cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
			}
			conn->terminateStatement(stmt);
			env->terminateConnection(conn);
			Environment::terminateEnvironment(env);
		}
	}
	return 0;
}


void resetPassword(Statement* stmt, string username, string password, string& status)
{
	stmt->setSQL("BEGIN Update_password(:1, :2, :3); END;");
	stmt->setString(1, username);
	stmt->setString(2, password);
	stmt->registerOutParam(3, Type::OCCISTRING, sizeof(status));
	stmt->executeUpdate();
	status = stmt->getString(3);
}


int mainMenu()
{
	int menu;
	do
	{
		cout << "********Main Menu********" << endl;
		cout << "1) login" << endl;
		cout << "0) exit" << endl;
		cout << "Enter an option(0-1): ";
		cin >> menu;
	} while (!(menu == 1 || menu == 0));
	return menu;
}

void addUser(Statement* stmt, string user, string pass, string& status)
{
	stmt->setSQL("BEGIN reg_username(:1, :2, :3); END;");
	stmt->setString(1, user);
	stmt->setString(2, pass);
	stmt->registerOutParam(3, Type::OCCISTRING, sizeof(status));
	stmt->executeUpdate();
	status = stmt->getString(3);
}

void removeEmployee(Statement* stmt)
{
	int id;
	cout << "Enter employee id to update: ";
	cin >> id;
	stmt->setSQL("BEGIN remove_employee(:1); END;");
	stmt->setInt(1, id);
	try
	{
		stmt->executeUpdate();
		cout << "employee number " << id << " removed successfully!" << endl;
	}
	catch (SQLException& sqlExcp)
	{

		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();

	}

}

void updateEmployee(Statement* stmt)
{
	string fn = "", ln = "", email = "", phone = "", date = "", jobTitle = "";
	int id, option = 10, mid = 0, num = 0;
	cout << "Enter employee id to update: ";
	cin >> id;
	while (option != 0)
	{
		do
		{
			cout << "****Choose the option to update****" << endl;
			cout << "1) First Name" << endl;
			cout << "2) Last Name" << endl;
			cout << "3) Email address" << endl;
			cout << "4) Phone number" << endl;
			cout << "5) Hire date" << endl;
			cout << "6) Manager Id" << endl;
			cout << "7) Job title" << endl;
			cout << "0) Confirm update" << endl;
			cin >> option;
			cin.ignore();
		} while (!(option <= 7 && option >= 0));
		switch (option)
		{
		case 0: {; break; }
		case 1: {
			num = 1;
			cout << "Enter new First Name: ";
			getline(cin, fn);
			break;
		}
		case 2: {
			num = 2;
			cout << "Enter new Last Name: ";
			getline(cin, ln);
			break;
		}
		case 3: {
			num = 3;
			cout << "Enter new Email address: ";
			getline(cin, email);
			break;
		}
		case 4: {
			num = 4;
			cout << "Enter new phone number: ";
			getline(cin, phone);
			break;
		}
		case 5: {
			num = 5;
			cout << "Enter new hire date(MM/DD/YYYY): ";
			getline(cin, date);
			break;
		}
		case 6: {
			num = 6;
			cout << "Enter new Manager id number: ";
			cin >> mid;
			cin.ignore();
			break;
		}
		case 7: {
			num = 7;
			cout << "Enter new job title: ";
			getline(cin, jobTitle);
			break;
		}
		
		}
		if (option != 0)
		{
			stmt->setSQL("BEGIN Update_employee(:1, :2, :3, :4, :5, :6, :7, :8, :9); END;");
			stmt->setInt(1, id);
			stmt->setString(2, fn);
			stmt->setString(3, ln);
			stmt->setString(4, email);
			stmt->setString(5, phone);
			stmt->setString(6, date);
			stmt->setInt(7, mid);
			stmt->setString(8, jobTitle);
			stmt->setInt(9, num);
			try
			{
				stmt->executeUpdate();
				cout << "Employee number " << id << " successfully updated." << endl;
			}
			catch (SQLException& sqlExcp)
			{
			
				cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();

            }
		}
		cout << "Employee number " << id << " updated successfully!" << endl;
	}
	
	
}

void addEmployee(Statement* stmt)
{
	try
	{
		string fn, ln, mail, phone, date, jt;
		int id, mid = 0;
		cout << "Enter employee id number: ";
		cin >> id;
		cin.ignore();
		cout << "Enter emplyee first name: ";
		getline(cin, fn);
		cout << "Enter emplyee last name: ";
		getline(cin, ln);
		cout << "Enter emplyee Email address: ";
		getline(cin, mail);
		cout << "Enter emplyee phone number: ";
		getline(cin, phone);
		cout << "Enter emplyee hire date(MM/DD/YYYY): ";
		getline(cin, date);
		cout << "Enter employee's manager id number(If there is no manager, Enter 0): ";
		cin >> mid;
		cin.ignore();
		cout << "Enter emplyee job Title: ";
		getline(cin, jt);
		stmt->setSQL("BEGIN Add_employee(:1, :2, :3, :4, :5, :6, :7, :8); END;");
		stmt->setInt(1, id);
		stmt->setString(2, fn);
		stmt->setString(3, ln);
		stmt->setString(4, mail);
		stmt->setString(5, phone);
		stmt->setString(6, date);
		stmt->setInt(7, mid);
		stmt->setString(8, jt);
	    stmt->executeUpdate();
		cout << "Employee number " << id << " successfully added." << endl;
	}
	catch (SQLException& sqlExcp)
		{
		if (sqlExcp.getErrorCode() == 1)
		{
			cout << "Error: Employee id number must be unique!" << endl;
		}
		else
		{
			cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
		}
	}
	
}

void employeeReport(Statement* stmt, ResultSet* rs)
{
	int empId;
	cout << "Enter employee Id number: ";
	cin >> empId;
	stmt->setSQL("BEGIN Employees_report(:1, :2); END;");
	stmt->setInt(1, empId);
	stmt->registerOutParam(2, Type::OCCICURSOR);
	stmt->executeUpdate();
	rs = stmt->getCursor(2);
	if (!rs->next())
	{
		cout << "There is no employee reports to employee number " << empId << endl;
	}
	else
	{
		int counter = 1;
		do
		{
			cout << "***********" << endl;
			cout << "First Name: " << rs->getString(1) << endl;
			cout << "Last Name: " << rs->getString(2) << endl;
			cout << "Job Title: " << rs->getString(3) << endl;
		} while (rs->next());
	}
}

void findEmployee(Statement* stmt, ResultSet* rs)
{
	int status;
	int empId;
	cout << "Enter employee Id number: ";
	cin >> empId;
	stmt->setSQL("BEGIN Find_employee(:1, :2, :3); END;");
	stmt->setInt(1, empId);
	stmt->registerOutParam(2, Type::OCCICURSOR);
	stmt->registerOutParam(3, Type::OCCIINT, sizeof(status));
	stmt->executeUpdate();
	status = stmt->getInt(3);
	if(status)
	rs = stmt->getCursor(2);
	if (status)
	{
		cout << "**** Employee number " << empId << " ****" << endl;
		rs->next();
		cout << "First Name: "  << rs->getString(1) << endl;
		cout << "Last Name: " << rs->getString(2) << endl;
		cout << "Email: " << rs->getString(3) << endl;
		cout << "Phone Number: " << rs->getString(4) << endl;
		cout << "Job Title: " << rs->getString(5) << endl;
		
	}
	else
	{
		cout << "Can not find employee number " << empId <<endl;
	}
	
}


void loginMenu(string user, string pass, string& status, Statement* stmt)
{
	stmt->setSQL("BEGIN Check_user_login(:1, :2, :3); END;");
	stmt->setString(1, user);
	stmt->setString(2, pass);
	stmt->registerOutParam(3, Type::OCCISTRING, sizeof(status));
	stmt->executeUpdate();
	status = stmt->getString(3);

}


int HRmenu()
{
	int option;
	do
	{
		cout << "************HR Menu************" << endl;
		cout << "1) Find Employee\n";
		cout << "2) Employee Report\n";
		cout << "3) Add Employee\n";
		cout << "4) Update Employee\n";
		cout << "5) Remove Employee\n";
		cout << "0) Exit\n";
		cin >> option;
	} while (!(option <= 5 && option >= 0));
	return option;
}

