#include "../include/display.h"
#include "../include/Wallet.h"

#include<iostream>
#include<string>
using namespace std;

string separate = "---------------------------------------------------------------------------------\n";

void clearScreen() {
    cout << "\033[2J\033[3J\033[1;1H";
}

void ENTER(bool needIgnore) {
    if(needIgnore) cin.ignore(); 
    cout << "ENTER to continue";
    cin.get(); 
}

void dashboard() {
    clearScreen();
    cout << separate;
    cout << "PERSONAL FINANCE MANAGER\n";
    cout << "Total Balance: " << total_balance(walletList) << '\n';
    if(walletList.size == 0) {
        cout << "There is no wallet\n";
    }
    else {
        for(int i = 0; i < walletList.size; i++) {
            const Wallet &w = walletList[i];
            cout << i + 1 << ". " << w.name << " (ID: " << w.id << ") - Balance: " << w.balance << '\n';
        }
    }
    cout << separate;
    cout << "MENU\n";
    cout << "0. EXIT\n";
    cout << "1. Wallet\n";
    cout << "2. Income Source\n";
    cout << "3. Expense Category\n";
    cout << "4. Transaction\n";
    cout << "5. Recurring Transactions\n";
    cout << "6. Statistics and Reporting\n";
    cout << separate;
}

void menuWallet() {
    clearScreen();
    cout << separate;
    cout << "Wallet options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Create a new wallet\n";
    cout << "2. Rename a wallet\n";
    cout << "3. Delete a wallet\n";
    cout << "4. Display wallets\n";
    cout << separate;
}

void menuIncomeSource() {
    clearScreen();
    cout << separate;
    cout << "Income Source options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Create a new source\n";
    cout << "2. Rename a source\n";
    cout << "3. Delete a source\n";
    cout << "4. Display all sources\n";
    cout << separate;
}

void menuExpenseCategory() {
    clearScreen();
    cout << separate;
    cout << "Expense Category options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Create a new category\n";
    cout << "2. Rename a category\n";
    cout << "3. Delete a category\n";
    cout << "4. Display all categories\n";
    cout << separate;
}

void menuTransaction() {
    clearScreen();
    cout << separate;
    cout << "Transaction options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Add a new transaction\n";
    cout << "2. Show transaction history\n";
    cout << separate;
}
void menuStatistics() {
    clearScreen();
    cout << separate;
    cout << "STATISTICS & REPORTING\n";
    cout << "0. Dashboard\n";
    cout << "1. Time_based Statistics\n";
    cout << "2. Time & Wallet-based Statistics\n";
    cout << "3. Years-based Overview\n";
    cout << "4. Annual Income Breakdown\n";
    cout << "5. Annual Expense Breakdown\n";
    cout << separate;
}