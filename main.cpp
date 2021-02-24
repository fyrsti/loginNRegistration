#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

enum CMD_CODE
{
	EXIT,
	HELP,
	REGISTER,
	LOGIN
};

map<string, CMD_CODE> cmd_translator{
	pair<string, CMD_CODE>("/help", CMD_CODE::HELP),
	pair<string, CMD_CODE> ("/l", CMD_CODE::LOGIN),
	pair<string, CMD_CODE> ("/r", CMD_CODE::REGISTER),
	pair<string, CMD_CODE>("/exit", CMD_CODE::EXIT),
	pair<string, CMD_CODE>("/q", CMD_CODE::EXIT)
};



template <typename T>
void input(string text, T& destination);


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
				cout << "Not implemented!" << endl;
				break;
			case REGISTER:
				cout << "Not implemented!" << endl;
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