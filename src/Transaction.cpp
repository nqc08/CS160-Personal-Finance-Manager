#include "../include/Transaction.h"
#include "../include/DynamicArray.h"
#include "../include/display.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"

#include<iostream>
#include<string>
using namespace std;

DynamicArray<Transaction> transList;

void add_new_transaction(DynamicArray<Transaction> &transList) {
    clearScreen();
    cout << separate;
    cout << "CREATE A NEW TRANSACTION\n";

    cout << "1. Income Transaction\n";
    cout << "2. Expense Transaction\n";
    cout << "Choose your option: ";
    int op; cin >> op;
    if(op == 1) transList.push_back(create_Income_Transaction());
    else transList.push_back(create_Expense_Transaction());

    cout << "CREATE A NEW TRANSACTION: SUCCESS\n";
    cout << separate;
}

Transaction create_Income_Transaction() {
    cout << separate;
    Transaction t;
    cout << "Input the date (dd mm yyyy): "; cin >> t.date.d >> t.date.m >> t.date.y;

    t.isIncome = true;

    cin.ignore();
    string name;
    int idx;
    while(true) {
        cout << "Input the income source's Name: "; getline(cin, name);
        idx = find_index_by_name(inSource, name);
        if(idx != -1) break;
        cout << "This source has not existed yet, please choose another source!\n";
    }
    t.srcID = inSource[idx].id;

    while(true) {
        cout << "Input the wallet's Name: "; getline(cin, name);
        idx = find_index_by_name(walletList, name);
        if(idx != -1) break;
        cout << "This wallet has not existed yet, please choose another wallet!\n"; 
    }
    t.walletID = walletList[idx].id;

    cout << "Input the amount of money: "; cin >> t.amount;
    walletList[idx].balance += t.amount;
    
    cout << "Description: ";
    cin.ignore(); getline(cin, t.note);

    return t;
}

Transaction create_Expense_Transaction() {
    cout << separate;
    Transaction t;
    cout << "Input the date (dd mm yyyy): "; cin >> t.date.d >> t.date.m >> t.date.y;

    t.isIncome = false;

    cin.ignore();
    string name;
    int idx;
    while(true) {
        cout << "Input the expense category's Name: "; getline(cin, name);
        idx = find_index_by_name(expCategory, name);
        if(idx != -1) break;
        cout << "This category has not existed yet, please choose another category!\n";
    }
    t.srcID = expCategory[idx].id;

    while(true) {
        cout << "Input the wallet's Name: "; getline(cin, name);
        idx = find_index_by_name(walletList, name);
        if(idx != -1) break;
        cout << "This wallet has not existed yet, please choose another wallet!\n"; 
    }
    t.walletID = walletList[idx].id;

    cout << "Input the amount of money: "; cin >> t.amount;
    walletList[idx].balance -= t.amount;
    
    cout << "Description: ";
    cin.ignore(); getline(cin, t.note);

    return t;
}

void show_transaction_history(const DynamicArray<Transaction> &transList) {
    clearScreen();
    cout << separate;
    cout << "DISPLAY ALL TRANSACTIONS\n";

    if(transList.size == 0) {
        cout << "There is no transaction\n";
        cout << separate;
        return;
    }

    for(int i = 0; i < transList.size; i++) {
        const Transaction &t = transList[i];

        cout << t.date.d << '/' << t.date.m << '/' << t.date.y << '\n';
        
        if(t.isIncome) {
            int idx = find_index_by_ID(inSource, t.srcID);
            cout << "Income Source: " << inSource[idx].name << '\n';
        }
        else {
            int idx = find_index_by_ID(expCategory, t.srcID);
            cout << "Expense Category: " << expCategory[idx].name << '\n';
        }
        
        int idx = find_index_by_ID(walletList, t.walletID);
        cout << "Wallet: " << walletList[idx].name << ' ';
        if(t.isIncome) cout << '+' << t.amount << '\n';
        else cout << '-' << t.amount << '\n';
        
        cout << t.note << '\n';

        cout << '\n';
    }

    cout << "DISPLAY ALL TRANSACTIONS: SUCCESS\n";
    cout << separate;
}