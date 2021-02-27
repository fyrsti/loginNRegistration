#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

template <typename T>
void input(string text, T& destination);
void user_controller(string username);
void admin_controller(string aname);


enum CMD_CODE
{
	EXIT,
	HELP,
	REGISTER,
	LOGIN,
	ALOGIN,
	LOGOUT,
	CHANGEPASS,
	SHOWUSERBASE,
	BAN,
	UNBAN
};


enum VALIDATION_RESPONSE
{
	CORRECT,
	INCORRECT,
	NOT_EXISTED,
};


map<string, int> cmd_translator{
	pair<string, int>("/help", HELP),
	pair<string, int>("/l", LOGIN),
	pair<string, int>("/r", REGISTER),
	pair<string, int>("/exit", EXIT),
	pair<string, int>("/q", EXIT),
	pair<string, int>("/logout", LOGOUT),
	pair<string, int>("/changepassword", CHANGEPASS),
	pair<string, int>("/admin", ALOGIN),
	pair<string, int>("/showbase", SHOWUSERBASE),
	pair<string, int>("/ban", BAN),
	pair<string, int>("/unban", UNBAN)
};


map<string, map<string, string>> db;

map<string, map<string, string>> adb;


int _verify_parameters(string LU, string LP, map<string, map<string, string>>& database)
{
	if (database.find(LU) != database.end())
	{
		if (database.at(LU).at("PASS") == LP)
			return CORRECT;
		return INCORRECT;
	}	
	return NOT_EXISTED;
}


bool _validate_password(string password)
{
	string forbidden{"/\\|/`~?<>\"\'"};
	if (password.size() < 3 || password.size() > 32)
	{
		cout << "Incorrect password length. Should be between from 3 to 32 symbols long." << endl;
		return false;
	}
	for (auto fs : forbidden)
	{
		if (password.at(0) == fs)
		{
			cout << "Passwords can't start with forbidden symbols like " << fs << endl;
			cout << "Forbidden start symbols: " << forbidden << endl;
			return false;
		}
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
		if (_validate_password(password))
			return password;
	}
}


void _create_account(string username, string password)
{
	static int id{ 0 };
	db.emplace(username, map<string, string>{
		pair<string, string>("PASS", password),
		pair<string, string>("ID", to_string(id)),
		pair<string, string>("STATUS", "alive") 
	});
}


string _get_current_password(string username)
{
	return db.at(username).at("PASS");
}


bool _verify_current_password(string current)
{
	string inp;
	input("Input your current password: ", inp);
	if (inp == "/back" || inp != current)
		return false;
	return true;

}


string _ask_new_password()
{
	string returned;
	while (true)
	{
		input("Input your new password: ", returned);
		if (returned == "/back")
			return "/back";
		if (_validate_password(returned))
			return returned;
	}
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
	switch (_verify_parameters(LUsername, LPassword, db))
	{
	case CORRECT:
		user_controller(LUsername);
		break;
	case INCORRECT:
		cout << "Incorrect password!" << endl;
		break;
	case NOT_EXISTED:
		cout << "There's no account with this username." << endl;
		break;
	}
}


void cmd_alogin()
{
	string AUsername, APassword;
	AUsername = _get_login_parameter("[A] >>> ");
	APassword = _get_login_parameter("[A] >>> ");
	if (_verify_parameters(AUsername, APassword, adb) == CORRECT)
		admin_controller(AUsername);
	return;
}


void _visualize_user(string username, vector<string> params)
{
	cout << "- ID:\t" << params.at(1) << endl;
	cout << "- USERNAME:\t" << username << endl;
	cout << "- PASSWORD:\t" << params.at(0) << endl;
	cout << "- STATUS:\t" << params.at(2) << endl;
	cout << "------------------------------\n";
}


vector<string> _unparse_user_parameters(string username)
{
	auto temp = db[username];
	return { temp["PASS"], temp["ID"], temp["STATUS"] };
}


void cmd_showbase()
{
	for (auto user : db)
	{
		_visualize_user(user.first, _unparse_user_parameters(user.first));
	}
}


void _change_status(string username, map<string, map<string, string>>& base, string newstatus)
{
	base[username]["STATUS"] = newstatus;
}


void cmd_ban()
{
	string username;
	input("Target username: ", username);
	switch (_verify_parameters(username, "", db))
	{
	case NOT_EXISTED:
		cout << "User doesn't exist" << endl;
		break;
	default:
		_change_status(username, db, "banned");
		break;
	}
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
	cout << "Great! Registration success!" << endl;
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


bool cmd_changepass(string profile)
{
	string current, newpass;
	current = _get_current_password(profile);
	if (_verify_current_password(current))
	{
		newpass = _ask_new_password();
		if (newpass == "/back")
			return false;
		db[profile]["PASS"] = newpass;
		return true;
	}
	else
	{
		cout << "You inputed an incorrect password!" << endl;
		return false;
	}
}


bool command_exist(string cmd)
{
	return (cmd_translator.find(cmd) != cmd_translator.end() ? true : false);
}


void admin_controller(string aname)
{
	cout << " ----- [ADMIN PANEL HAS STARTED] ----- " << endl;
	string command;
	while (true)
	{
		input("[A] >>> ", command);
		if (command_exist(command))
		{
			switch (cmd_translator.at(command))
			{
			case EXIT:
				exit(0);
			case HELP:
				cmd_help({ EXIT, ALOGIN, SHOWUSERBASE, BAN });
				break;
			case ALOGIN:
				system("cls");
				return;
			case SHOWUSERBASE:
				cmd_showbase();
				break;
			case BAN:
				cmd_ban();
				break;
			default:
				break;
			}
		}
		else
		{
			cout << "Incorrect command! Command list below" << endl;
			cmd_help({ EXIT });
		}
	}
	cout << "Not implemented!" << endl;
}


void user_controller(string username)
{
	system("cls");
	cout << "You are welcome, " << username << endl;
	string command;
	while (true)
	{
		input(username + " >>> ", command);
		if (command_exist(command))
		{
			switch (cmd_translator.at(command))
			{
			case EXIT:
				exit(0);
			case HELP:
				cmd_help({ EXIT, LOGOUT, CHANGEPASS });
				break;
			case CHANGEPASS:
				if (cmd_changepass(username))
				{
					cout << "Great! Your new password is " << db.at(username).at("PASS") << endl;
					break;
				}
				return;
			case LOGOUT:
				system("cls");
				cout << "Bye, " << username << endl;
				return;
			case ALOGIN:
				cmd_alogin();
				break;
			default:
				cout << "Incorrect command!" << endl;
			}
		}
		else
			cout << "Incorrect command! [/help - for more information]" << endl;
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
				cmd_help({REGISTER, LOGIN, EXIT});
				break;
			case REGISTER:
				cmd_register();
				break;
			case LOGIN:
				cmd_login();
				break;
			case ALOGIN:
				cmd_alogin();
				break;
			default:
				cout << "Incorrect command!" << endl;
			}
		}
		else
			cout << "Incorrect command!  [/help - for more information]" << endl;
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
	adb.emplace("fyrsti", map<string, string>{
		pair<string, string>("PASS", "123"),
		pair<string, string>("ID", "0"),
		pair<string, string>("STATUS", "admin")
	});
	cout << "Start message" << endl;
	guest_controller();
	return 0;
}