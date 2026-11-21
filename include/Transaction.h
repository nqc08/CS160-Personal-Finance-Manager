#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "DynamicArray.h"

#include<string>
using namespace std;

struct Date {
    int d, m, y;
    Date() : d(0), m(0), y(0) {}
    Date(int d, int m, int y) : d(d), m(m), y(y) {}
};  
struct Transaction {
    Date date;
    bool isIncome;
    string srcID, walletID, note;
    long long amount;
};

extern DynamicArray<Transaction> transList;

bool is_valid_date(const Date &date);
int compare_dates(const Date &a, const Date &b);
bool record_transaction(const Transaction &transaction, DynamicArray<Transaction> &transactions);

void add_new_transaction(DynamicArray<Transaction> &transList);

Transaction create_Income_Transaction();

Transaction create_Expense_Transaction();

void show_transaction_history(const DynamicArray<Transaction> &transList);

#endif