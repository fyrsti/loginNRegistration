#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

template <typename T>
void input(string text, T& destination);

enum CMD_CODE
{
	EXIT,
	HELP,
	REGISTER,
	LOGIN
};

map<string, int> cmd_translator{
	pair<string, int>("/help", HELP),
	pair<string, int> ("/l", LOGIN),
	pair<string, int> ("/r", REGISTER),
	pair<string, int>("/exit", EXIT),
	pair<string, int>("/q", EXIT)
};


map<string, string> db;


void cmd_register()
{
	string username, password;

	while (true)
	{
		cout << "[/back - return to the previous instruction]" << endl;
		while (true)
		{
			input("new username: ", username);
			if (username == "/back")
				return;
			else if (db.find(username) != db.end())
				cout << "This username is already taken!" << endl;
			else
				break;
		}

		while (true)
		{
			input("new password: ", password);
			if (password == "/back")
				break;
			if (password.size() >= 3 && password.size() <= 32)
			{
				db.emplace(username, password);
				cout << "-- Success! --" << endl;
				return;
			}
			else
				cout << "Password must be from 3 to 32 symbols long" << endl;
		}
	}
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
				break;
			case HELP:
				cmd_help(vector<int>{REGISTER, LOGIN, EXIT});
				break;
			case REGISTER:
				cmd_register();
				break;
			case LOGIN:
				cout << "Not implemented!" << endl;
				break;
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