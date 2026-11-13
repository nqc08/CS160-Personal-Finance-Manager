#include<iostream>
#include<string>
#include "../include/Expense.h"
#include "../include/DynamicArray.h"
#include "../include/display.h"
using namespace std;

DynamicArray<ExpenseCategory> expCategory;

void add_new_category(DynamicArray<ExpenseCategory> &expCategory) {
    cout << separate;
    cout << "CREATE A NEW EXPENSE CATEGORY\n";

    ExpenseCategory s;
    cout << "Input category's ID: "; cin >> s.id;
    cin.ignore();
    while(true) {
        cout << "Input a category's Name: "; getline(cin, s.name);
        if(find_index_by_name(expCategory, s.name) == -1) break;

        cout << "Name existed, please choose another Name!\n";
    }

    expCategory.push_back(s);

    cout << "CREATE A NEW EXPENSE CATEGORY: SUCCESS\n";
    cout << separate;
}

void rename_category(DynamicArray<ExpenseCategory> &expCategory) {
    cout << separate;
    cout << "RENAME AN EXPENSE CATEGORY\n";

    string name;
    cin.ignore();
    while(true) {
        cout << "Input a category's Name: "; getline(cin, name);
        if(find_index_by_name(expCategory, name) != -1) break;

        cout << "This category has not existed yet, please choose another category!\n";
    }
    int idx = find_index_by_name(expCategory, name);
    
    string newName;
    while(true) {
        cout << "Input a new Name: "; getline(cin, newName);
        if(find_index_by_name(expCategory, newName) == -1) break;

        cout << "This Name has existed already, please choose another Name!\n";
    }

    expCategory[idx].name = newName;

    cout << "RENAME AN EXPENSE CATEGORY: SUCCESS\n";
    cout << separate;
}

void delete_category(DynamicArray<ExpenseCategory> &expCategory) {
    cout << separate;
    cout << "DELETE AN EXPENSE CATEGORY\n";

    if(expCategory.size == 0) {
        cout << "There is no category\n";
        cout << separate;
        return;
    }

    string name;
    cin.ignore();
    while(true) {
        cout << "Input the category's Name that you want to delete: "; getline(cin, name);
        if(find_index_by_name(expCategory, name) != -1) break;

        cout << "There is no category with this name, please choose another category!\n";
    }

    int idx = find_index_by_name(expCategory, name);
    expCategory.remove(idx);

    cout << "DELETE AN EXPENSE CATEGORY: SUCCESS\n";
    cout << separate;
}

int find_index_by_name(DynamicArray<ExpenseCategory> &expCategory, const string &name) {
    for(int i = 0; i < expCategory.size; i++) {
        if(expCategory[i].name == name) return i;
    }
    return -1;
}

void display_category(const DynamicArray<ExpenseCategory> &expCategory) {
    cout << separate;
    cout << "DISPLAY ALL EXPENSE CATEGORIES\n";

    if(expCategory.size == 0) {
        cout << "There is no category\n";
        cout << separate;
        return;
    }

    for(int i = 0; i < expCategory.size; i++) {
        const ExpenseCategory &s = expCategory[i];
        cout << i + 1 << ". " << s.name << " (ID: " << s.id << ")\n";
    }

    cout << "DISPLAY ALL EXPENSE CATEGORIES: SUCCESS\n";
    cout << separate;
}