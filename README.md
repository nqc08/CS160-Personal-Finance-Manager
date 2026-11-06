# C++ Personal Finance Manager

A lightweight, high-performance command-line interface (CLI) application for personal finance management, built entirely from scratch in C++ without relying on STL containers for core data structures.

---

## ✨ Features

* **Custom Memory Management:** Implements a generic, template-based `DynamicArray<T>` structure with manual memory handling (`new`/`delete`) and deep-copy semantics.
* **Wallet Management:** Create, update, rename, and delete wallets with custom tracking IDs and balance updates.
* **Income & Expense Tracking:** Categorize and record transactions seamlessly.
* **User-Friendly CLI:** Interact with core entities using intuitive names while keeping robust unique IDs under the hood.
* **Safe Input Handling:** Robust input buffer management to prevent stream locking and infinite loops during user interactions.

---

## 📂 Project Structure

```text
Personal_Finance_Manager/
├── include/
│   ├── DynamicArray.h              # Template Dynamic Array (Use pointer) and template<typename>
│   ├── Wallet.h                    # Manage wallet, wallet list and functions
│   ├── Income.h                    # Manage income, income source, and functions
│   ├── Expense.h                   # Manage expense, expense category, and functions
│   ├── Transaction.h               # Manage transactions and functions
│   ├── RecurringTransaction.h      # Manage recurring transtion and functions
│   ├── Storage.h                   # Binary File I/O operations
│   ├── Statistics.h                # Statistics & Reporting calculations
│   ├── display.h                   # Display format
└── src/
│   ├── main.cpp                    # Entry point of the application    
│   ├── display.cpp        
│   ├── Wallet.cpp                  
│   ├── Income.cpp                  
│   ├── Expense.cpp                 
│   ├── Transaction.cpp             
│   ├── RecurringTransaction.cpp    
│   ├── Storage.cpp                 
│   ├── Statistics.cpp
└── CONTRIBUTING.md                 # Structure of the application
└── README.md                       # Brief description and instructions
```

## 🛠️ Running the Program

### Prerequisites
* A C++ compiler supporting **C++11** or higher (e.g., `g++`, `clang++`, or MSVC).

### Compilation & Execution (Bash Commands)
Open your terminal in the root directory of the project and run the following bash commands to compile and execute the program:

```
bash
# Compile all source files together and include the header path
g++ -std=c++11 src/*.cpp -Iinclude -o FinanceManager

# Run the executable:
# On Linux / macOS:
./FinanceManager

# On Windows (Command Prompt / PowerShell):
FinanceManager.exe
```


## 🚀 Usage Guide
* Launch the application to view the main dashboard menu.
* Select options to manage your wallets (e.g., adding a cash or bank account).
* Record your financial transactions, which will automatically update your active wallet balances.
* Follow the interactive CLI prompts to rename or delete entries securely using user-friendly names.


## 📝 License
This project is open-source and available under the MIT License.