#ifndef EXPENSE_H
#define EXPENSE_H

#include<string>
#include "DynamicArray.h"
using namespace std;

struct ExpenseCategory {
    string id, name;
};

extern DynamicArray<ExpenseCategory> expCategory;

void add_new_category(DynamicArray<ExpenseCategory> &expCategory);

void rename_category(DynamicArray<ExpenseCategory> &expCategory);

void delete_category(DynamicArray<ExpenseCategory> &expCategory);

int find_index_by_name(DynamicArray<ExpenseCategory> &expCategory, const string &name);

void display_category(const DynamicArray<ExpenseCategory> &expCategory);

#endif