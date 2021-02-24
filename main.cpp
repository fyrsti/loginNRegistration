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

map<string, int> cmd_translator{
	pair<string, int>("/help", HELP),
	pair<string, int> ("/l", LOGIN),
	pair<string, int> ("/r", REGISTER),
	pair<string, int>("/exit", EXIT),
	pair<string, int>("/q", EXIT)
};


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
				cmd_help(vector<int>{REGISTER, LOGIN, EXIT});
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