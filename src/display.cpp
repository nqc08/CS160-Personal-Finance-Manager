#include<iostream>
#include<string>
#include "../include/display.h"
#include "../include/Wallet.h"

string separate = "---------------------------------------------------------------------------------\n";

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void ENTER(bool needIgnore) {
    if(needIgnore) cin.ignore(); 
    cout << "ENTER to continue";
    cin.get(); 
}

void dashboard() {
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
    cout << separate;
}

void displayWallet() {
    cout << separate;
    cout << "Wallet options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Create a new wallet\n";
    cout << "2. Rename a wallet\n";
    cout << "3. Delete a wallet\n";
    cout << "4. Display wallets\n";
    cout << separate;
}

void displayIncomeSource() {
    cout << separate;
    cout << "Income Source options:\n";
    cout << "0. Dashboard\n";
    cout << "1. Create a new source\n";
    cout << "2. Rename a source\n";
    cout << "3. Delete a source\n";
    cout << "4. Display all sources\n";
    cout << separate;
}