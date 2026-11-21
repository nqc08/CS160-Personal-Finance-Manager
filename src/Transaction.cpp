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

void add_new_transaction(DynamicArray<Transaction> &) {
    clearScreen();
    cout << separate;
    cout << "CREATE A NEW TRANSACTION\n";

    cout << "1. Income Transaction\n";
    cout << "2. Expense Transaction\n";
    cout << "Choose your option: ";
    int op; cin >> op;
    if (op != 1 && op != 2) {
        cout << "Invalid option.\n";
        return;
    }
    if (walletList.size == 0 || (op == 1 ? inSource.size == 0 : expCategory.size == 0)) {
        cout << "Create a wallet and an income source or expense category first.\n";
        return;
    }
    Transaction t = op == 1 ? create_Income_Transaction() : create_Expense_Transaction();
    if (!record_transaction(t)) {
        cout << "Invalid date, amount, wallet, or source/category.\n";
        return;
    }

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
            cout << "Income Source: " << (idx == -1 ? "[Deleted] " + t.srcID : inSource[idx].name) << '\n';
        }
        else {
            int idx = find_index_by_ID(expCategory, t.srcID);
            cout << "Expense Category: " << (idx == -1 ? "[Deleted] " + t.srcID : expCategory[idx].name) << '\n';
        }
        
        int idx = find_index_by_ID(walletList, t.walletID);
        cout << "Wallet: " << (idx == -1 ? "[Deleted] " + t.walletID : walletList[idx].name) << ' ';
        if(t.isIncome) cout << '+' << t.amount << '\n';
        else cout << '-' << t.amount << '\n';
        
        cout << t.note << '\n';

        cout << '\n';
    }

    cout << "DISPLAY ALL TRANSACTIONS: SUCCESS\n";
    cout << separate;
}

bool is_valid_date(const Date &date) {
    if (date.y < 1 || date.m < 1 || date.m > 12 || date.d < 1) return false;
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = date.y % 400 == 0 || (date.y % 4 == 0 && date.y % 100 != 0);
    if (leap) days[1] = 29;
    return date.d <= days[date.m - 1];
}

int compare_dates(const Date &a, const Date &b) {
    if (a.y != b.y) return a.y < b.y ? -1 : 1;
    if (a.m != b.m) return a.m < b.m ? -1 : 1;
    if (a.d != b.d) return a.d < b.d ? -1 : 1;
    return 0;
}

bool record_transaction(const Transaction &t) {
    if (!is_valid_date(t.date) || t.amount <= 0) return false;
    int wallet = find_index_by_ID(walletList, t.walletID);
    int source = t.isIncome ? find_index_by_ID(inSource, t.srcID)
                            : find_index_by_ID(expCategory, t.srcID);
    if (wallet == -1 || source == -1) return false;
    transList.push_back(t);
    if (t.isIncome) walletList[wallet].balance += t.amount;
    else walletList[wallet].balance -= t.amount;
    return true;
}
