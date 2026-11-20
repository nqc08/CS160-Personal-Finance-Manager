#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "DynamicArray.h"

#include<string>
using namespace std;

struct Date {
    int d, m, y;
    Date() {}
    Date(int d, int m, int y) : d(d), m(m), y(y) {}
};  
struct Transaction {
    Date date;
    bool isIncome;
    string srcID, walletID, note;
    long long amount;
};

extern DynamicArray<Transaction> transList;

void add_new_transaction(DynamicArray<Transaction> &transList);

Transaction create_Income_Transaction();

Transaction create_Expense_Transaction();

void show_transaction_history(const DynamicArray<Transaction> &transList);

#endif