# Bank Management System

A console-based Bank Management System built with C++ that manages client records, processes account transactions, and supports user authentication and permission-based access control. The project uses text files for data persistence and demonstrates procedural programming, file handling, and modular application design.

## Features

### Client Management
- Display all client records.
- Add new clients with duplicate account number validation.
- Update client information.
- Delete clients with confirmation.
- Search for clients by account number.
- Store client information in a text file.

### Transactions
- Deposit funds into client accounts.
- Withdraw funds with a balance limit check.
- Display individual client balances.
- Calculate and display the total balance across all clients.
- Save balance changes to the data file.

### User Management
- List existing users.
- Add users with unique usernames.
- Update user passwords and permissions.
- Delete users, with protection against deleting the `Admin` account.
- Search for users by username.

### Authentication and Permissions
- Login using a username and password.
- Logout and return to the login screen.
- Assign full or customized permissions to users.
- Use bitwise permission flags to control access to application features.
- Display an access-denied message when a user attempts to open a restricted menu.

## Technologies Used

- **Language:** C++
- **IDE:** Microsoft Visual Studio 2022
- **Data Storage:** Text files using file streams (`fstream` and `ifstream`/`ofstream`)
- **Core Concepts:** Structures, vectors, functions, enums, string manipulation, file handling, and bitwise operators
- **Version Control:** Git and GitHub

## Data Storage

The application uses text files to store client and user records.

- `Clients.txt` — stores account numbers, PIN codes, client names, phone numbers, and account balances.
- `Users.txt` — stores usernames, passwords, and permission values.

Records use `#//#` as a field separator.

The permission value `-1` represents full access. Other permission values are combinations of individual bit flags that determine which features a user can access.

## Build and Run

1. Install Microsoft Visual Studio 2022 with the C++ desktop development workload.
2. Open the solution file in Visual Studio.
3. Select the `Debug x64` configuration.
4. Build the solution, or choose **Build → Rebuild Solution**.
5. Run the application with **Ctrl + F5**.
6. Make sure the application can access `Users.txt` and `Clients.txt` from its working directory.

## Demo Accounts

The included `Users.txt` file contains sample accounts for testing:

| Username | Password | Access |
|---|---|---|
| `Admin` | `1234` | Full access |
| `User1` | `1000` | Selected permissions |

The `User1` account demonstrates restricted access to application features.

## Project Structure

```text
Bank-Management-System/
├── Banck-System/
│   ├── Banck-System.cpp
│   ├── Banck-System.vcxproj
│   ├── Banck-System.vcxproj.filters
│   ├── Users.txt
│   └── Clients.txt
├── README.md
├── Banck-System.sln
├── LICENSE
└── .gitignore
```

## Important Security Note

This project is an educational exercise and is not suitable for real banking operations. The current implementation stores passwords in plain text and does not provide production-grade authentication or security.

A production system would require secure password hashing, stronger input validation, reliable transaction handling, and appropriate protection of sensitive financial data.

## Future Improvements

- Hash passwords securely instead of storing them in plain text.
- Improve input validation and transaction amount checks.
- Remove existing compiler warnings.
- Add transaction history and audit logging.
- Replace text-file storage with a database.
- Add automated tests for account transactions and permission enforcement.

## 📄 License

This project is licensed under the **MIT License** – feel free to use and modify it.