#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

template <typename T>
void input(string text, T& destination);
void user_controller(string username);

enum CMD_CODE
{
	EXIT,
	HELP,
	REGISTER,
	LOGIN,
	LOGOUT,
	CHANGEPASS
};

map<string, int> cmd_translator{
	pair<string, int>("/help", HELP),
	pair<string, int>("/l", LOGIN),
	pair<string, int>("/r", REGISTER),
	pair<string, int>("/exit", EXIT),
	pair<string, int>("/q", EXIT),
	pair<string, int>("/logout", LOGOUT),
	pair<string, int>("/changepassword", CHANGEPASS)
};


map<string, string> db;


bool _verify_parameters(string LU, string LP)
{
	if (db.find(LU) != db.end())
	{
		if (db.at(LU) == LP)
		{
			cout << " ---- Success ---- " << endl;
			return true;
		}
		cout << "Incorrect password!" << endl;
		return false;
	}
	else
	{
		cout << "There's no account with this username." << endl;
		return false;
	}
}


string _get_login_parameter(string text)
{
	string returned;
	input(text, returned);
	if (returned == "/back")
		return "0";
	else
		return returned;
}



void cmd_login()
{
	string LUsername, LPassword;
	while (true)
	{
		LUsername = _get_login_parameter("Login username: ");
		if (LUsername == "0")
			return;
		LPassword = _get_login_parameter("Login password: ");
		if (LPassword != "0")
			break;
	}
	if (_verify_parameters(LUsername, LPassword))
		user_controller(LUsername);
}


string _get_username()
{
	string username;
	while (true)
	{
		input("New username: ", username);
		if (username == "/back")
			return "/back";
		if (db.find(username) != db.end())
			cout << "This username is already taken!" << endl;
		else
			return username;
	}
}


string _get_password()
{
	string password;
	while (true)
	{
		input("New password: ", password);
		if (password == "/back")
			return "12";
		if (password.size() >= 3 && password.size() <= 32)
			return password;
		else
			cout << "Password must be from 3 to 32 symbols long" << endl;
	}
}


void _create_account(string username, string password)
{
	db.emplace(username, password);
}


void cmd_register()
{
	string username, password;
	while (true)
	{
		username = _get_username();
		if (username == "/back")
			return;
		password = _get_password();
		if (password.size() >= 3)
			break;
	}
	_create_account(username, password);
}


void cmd_help(vector<int> b)
{
	for (auto p : cmd_translator)
	{
		for (auto code : b)
		{
			if (p.second == code)
				cout << "- " << p.first << endl;
		}
	}
}

bool command_exist(string cmd)
{
	return (cmd_translator.find(cmd) != cmd_translator.end() ? true : false);
}


void user_controller(string username)
{
	system("cls");
	cout << "You are welcome, " << username << endl;
	string command;
	while (true)
	{
		input(username + ">>> ", command);
		if (command_exist(command))
		{
			switch (cmd_translator.at(command))
			{
			case EXIT:
				exit(0);
			case HELP:
				cmd_help({ LOGOUT, CHANGEPASS });
				break;
			case LOGOUT:
				cout << "Not implemented!" << endl;
				break;
			case CHANGEPASS:
				cout << "Not implemented!" << endl;
				break;
			default:
				cout << "Incorrect command!" << endl;
			}
		}
		else
			cout << "Incorrect command!" << endl;
	}
}


void guest_controller()
{
	string command;
	while (true)
	{
		input(">>> ", command);
		if (command_exist(command))
		{
			switch (cmd_translator.at(command))
			{
			case EXIT:
				exit(0);
			case HELP:
				cmd_help(vector<int>{REGISTER, LOGIN, EXIT});
				break;
			case REGISTER:
				cmd_register();
				break;
			case LOGIN:
				cmd_login();
				break;
			default:
				cout << "Incorrect command!" << endl;
			}
		}
		else
			cout << "Incorrect command!" << endl;
	}
}


template <typename T>
void input(string text, T& destination)
{
	cout << text;
	cin >> destination;
}


int main()
{
	guest_controller();
	return 0;
}