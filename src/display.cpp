#include<iostream>
#include<string>
#include "../include/display.h"

string separate = "---------------------------------------------------------------------------------\n";

void dashboard() {
    cout << separate;
    cout << "MENU\n";
    cout << "0. EXIT\n";
    cout << "1. Wallet\n";
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