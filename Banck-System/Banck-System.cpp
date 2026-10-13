#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eShowManageUsersMenue = 7, eLogout = 8 };

enum enManageUsersOptions { eUsersList = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue = 6 };

enum enPermissions
{
    eAllPerm = -1, eListClientsPerm = 1, eAddNewClientPerm = 2, eDeleteCleintPerm = 4,
    eUpdateClientPerm = 8, eFindClientPerm = 16, eTransactionsMenuePerm = 32, eManageUsersPerm = 64
};


struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};

struct stUsers
{
    string UserName;
    string Password;
    int Permissions = 0;
    bool MarkForDelete = false;
};

stUsers CurrentUser;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
short ReadPermissions();
void ShowManageUsersMenue();
void Login();


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

string ConvertUserRecordToLine(stUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions) + Seperator;

    return stUserRecord;
}

stUsers ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
    vector <string> vUsersData = SplitString(Line, Seperator);
    stUsers User;

    User.UserName = vUsersData.at(0);
    User.Password = vUsersData.at(1);
    User.Permissions = stoi(vUsersData.at(2)); // cast string to int(short).

    return User;
}

vector <stUsers> LoadUsersDataFromFile(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in); // Read Mode

    vector <stUsers> vUsersData;

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            vUsersData.push_back(User);
        }
        MyFile.close();
    }
    return vUsersData;
}

bool UserExistByUserName(string UserName, stUsers& DesiredUser, string FileName)
{
    vector<stUsers> vUsers = LoadUsersDataFromFile(FileName);

    for (stUsers User : vUsers)
    {
        if (User.UserName == UserName)
        {
            DesiredUser = User;
            return true;
        }
    }
    return false;
}

void ShowAccessDeniedMessage()
{
    cout << "\n-----------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You Don't Have Permission To Do This,\n";
    cout << "Please Contact Your Admin.\n";
    cout << "\n-----------------------------------\n";
}

bool CheckAccessPermission(enPermissions Permission)
{
    if (CurrentUser.Permissions == enPermissions::eAllPerm)
    {
        return true;
    }
    if ((CurrentUser.Permissions & Permission) == Permission)
        return true;
    else
        return false;
}

stUsers ReadNewUser()
{
    stUsers User;

    cout << "Enter UserName? ";

    getline(cin >> ws, User.UserName);

    while (UserExistByUserName(User.UserName, User, UsersFileName))
    {
        cout << "User with [" << User.UserName << "] already exists, enter another username? ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";

    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enPermissions::eListClientsPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

void PrintUserCard(stUsers User)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nUser Name           : " << User.UserName;
    cout << "\nPassword            : " << User.Password;
    cout << "\nPermissions         : " << User.Permissions;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

bool MarkUserFroDelete(string UserName, vector<stUsers>& vUsers)
{
    for (stUsers& User : vUsers)
    {
        if (User.UserName == UserName)
        {
            User.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <stUsers> SaveUsersDataToFile(string FileName, vector<stUsers>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); //Overwrite

    if (MyFile.is_open())
    {
        string DataLine = "";
        for (stUsers User : vUsers)
        {
            //we only write records that are not marked for delete.  
            if (User.MarkForDelete == false)
            {
                DataLine = ConvertUserRecordToLine(User);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

void AddNewUser()
{
    stUsers User = ReadNewUser();

    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
    char AddMore = 'y';
    do
    {
        cout << "Adding New User\n";

        AddNewUser();

        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;
    } while (AddMore == 'y' || AddMore == 'Y');
}

string ReadUserName()
{
    string UserName = "";
    cout << "Enter UserName? ";
    getline(cin >> ws, UserName);

    return UserName;
}

string ReadPassword()
{
    string Password = "";
    cout << "Enter Password? ";
    getline(cin >> ws, Password);

    return Password;
}

bool DeleteUser(string UserName, vector<stUsers>& vUsers)
{
    if (UserName == "Admin")
    {
        cout << "You Can't Delete This User.\n";
        return false;
    }

    stUsers User;
    if (UserExistByUserName(UserName, User, UsersFileName))
    {
        PrintUserCard(User);

        char Answer;
        cout << "\n\nAre you sure you want to delete this user? Y/N ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkUserFroDelete(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //Refresh Clients
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
        return false;
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

stUsers ChangeUserRecord(string UserName)
{
    stUsers User;
    User.UserName = UserName;

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

bool UpdateUser(string UserName, vector<stUsers>& vUsers)
{
    stUsers User;
    if (UserExistByUserName(UserName, User, UsersFileName))
    {
        PrintUserCard(User);

        char Answer = 'y';
        cout << "\n\nAre you sure you want to update this user? Y/N ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUsers& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
        return false;
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

void PrintUserRecordLine(stUsers User)
{
    cout << "| " << setw(30) << left << User.UserName;
    cout << "| " << setw(15) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permissions;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }
        return false;
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enPermissions::eDeleteCleintPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enPermissions::eUpdateClientPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }


    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enPermissions::eAddNewClientPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }


    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enPermissions::eFindClientPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance.
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

void ShowAllUsersScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUser List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "__________________\n" << endl;

    cout << "| " << left << setw(30) << "User Name";
    cout << "| " << left << setw(15) << "Password";
    cout << "| " << left << setw(12) << "Permission";
    cout << "\n_______________________________________________________";
    cout << "__________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (stUsers& User : vUsers)
        {

            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "__________________\n" << endl;

}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-----------------------------------\n";

    vector<stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    string UserName = ReadUserName();

    DeleteUser(UserName, vUsers);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    string UserName = ReadUserName();
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    UpdateUser(UserName, vUsers);
}

bool FindUser(string UserName, vector<stUsers>& vUsers)
{
    stUsers User;
    if (UserExistByUserName(UserName, User, UsersFileName))
    {
        PrintUserCard(User);
        return true;
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUsers& CurrentUser)
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUsers User : vUsers)
    {
        if (User.UserName == UserName && User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    string UserName = ReadUserName();
    vector<stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    FindUser(UserName, vUsers);
}

short ReadPermissions()
{
    short Permissions = 0;

    char Answer = 'Y';
    cout << "\n\nDo You Want to give full access? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return -1;
    }

    cout << "\n\nDo You Want to give access to\n\n";

    cout << "\nShow Client List? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eListClientsPerm;
    }

    cout << "\nAdd New Client? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eAddNewClientPerm;
    }

    cout << "\nDelete Client? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eDeleteCleintPerm;
    }

    cout << "\nUpdate Client? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eUpdateClientPerm;
    }

    cout << "\nFind Client? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eFindClientPerm;
    }

    cout << "\nTransactions? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eTransactionsMenuePerm;
    }

    cout << "\nManage Users? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissions::eManageUsersPerm;
    }

    return Permissions;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenue();
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eShowMainMenue:
    {
        ShowMainMenue();
    }
    }

}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enPermissions::eTransactionsMenuePerm))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");

    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    PerfromTranactionsMenueOption(
        (enTransactionsMenueOptions)ReadTransactionsMenueOption()
    );
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromManageUsersOption(enManageUsersOptions ManageUsersOption)
{
    switch (ManageUsersOption)
    {
    case enManageUsersOptions::eUsersList:
    {
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersOptions::eAddNewUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eMainMenue:
        ShowMainMenue();
        break;
    }

}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermission(enPermissions::eManageUsersPerm))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromManageUsersOption((enManageUsersOptions)ReadManageUsersOption());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
        case enMainMenueOptions::eListClients:
        {
            system("cls");
            ShowAllClientsScreen();
            GoBackToMainMenue();
            break;
        }
        case enMainMenueOptions::eAddNewClient:
        {
            system("cls");
            ShowAddNewClientsScreen();
            GoBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eDeleteClient:
        {
            system("cls");
            ShowDeleteClientScreen();
            GoBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eUpdateClient:
        {
            system("cls");
            ShowUpdateClientScreen();
            GoBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eFindClient:
        {
            system("cls");
            ShowFindClientScreen();
            GoBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eShowTransactionsMenue:
        {
            system("cls");
            ShowTransactionsMenue();
            GoBackToMainMenue();
            break;
        }
        
        case enMainMenueOptions::eShowManageUsersMenue:
        {
            system("cls");
            ShowManageUsersMenue();
            GoBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eLogout:
        {
            system("cls");
            Login();
            break;
        }
    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool LoadUserInfo(string UserName, string Password)
{
    if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;

    do
    {
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFaild)
        {
            cout << "\nInvalid UserName/Password!\n\n";
        }

        string UserName = ReadUserName();
        string Password = ReadPassword();

        LoginFaild = !LoadUserInfo(UserName, Password);

    } while (LoginFaild);

    ShowMainMenue();

}


int main()
{
    Login();

    system("pause>0");
    return 0;
}