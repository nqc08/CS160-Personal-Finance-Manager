#include<iostream>
#include<string>
#include "../include/Income.h"
#include "../include/DynamicArray.h"
#include "../include/display.h"
using namespace std;

DynamicArray<IncomeSource> inSource;

void add_new_source(DynamicArray<IncomeSource> &inSource) {
    cout << separate;
    cout << "CREATE A NEW INCOME SOURCE\n";

    IncomeSource s;
    cout << "Input source's ID: "; cin >> s.id;
    cin.ignore();
    while(true) {
        cout << "Input a source's Name: "; getline(cin, s.name);
        if(find_index_by_name(inSource, s.name) == -1) break;

        cout << "Name existed, please choose another Name!\n";
    }

    inSource.push_back(s);

    cout << "CREATE A NEW INCOME SOURCE: SUCCESS\n";
    cout << separate;
}

void rename_source(DynamicArray<IncomeSource> &inSource) {
    cout << separate;
    cout << "RENAME AN INCOME SOURCE\n";

    string name;
    cin.ignore();
    while(true) {
        cout << "Input a source's Name: "; getline(cin, name);
        if(find_index_by_name(inSource, name) != -1) break;

        cout << "This source has not existed yet, please choose another source!\n";
    }
    int idx = find_index_by_name(inSource, name);
    
    string newName;
    while(true) {
        cout << "Input a new Name: "; getline(cin, newName);
        if(find_index_by_name(inSource, newName) == -1) break;

        cout << "This Name has existed already, please choose another Name!\n";
    }

    inSource[idx].name = newName;

    cout << "RENAME AN INCOME SOURCE: SUCCESS\n";
    cout << separate;
}

void delete_source(DynamicArray<IncomeSource> &inSource) {
    cout << separate;
    cout << "DELETE AN INCOME SOURCE\n";

    if(inSource.size == 0) {
        cout << "There is no source\n";
        cout << separate;
        return;
    }

    string name;
    cin.ignore();
    while(true) {
        cout << "Input the source's Name that you want to delete: "; getline(cin, name);
        if(find_index_by_name(inSource, name) != -1) break;

        cout << "There is no source with this name, please choose another source!\n";
    }

    int idx = find_index_by_name(inSource, name);
    inSource.remove(idx);

    cout << "DELETE AN INCOME SOURCE: SUCCESS\n";
    cout << separate;
}

int find_index_by_name(DynamicArray<IncomeSource> &inSource, const string &name) {
    for(int i = 0; i < inSource.size; i++) {
        if(inSource[i].name == name) return i;
    }
    return -1;
}

void display_source(const DynamicArray<IncomeSource> &inSource) {
    cout << separate;
    cout << "DISPLAY ALL INCOME SOURCES\n";

    if(inSource.size == 0) {
        cout << "There is no source\n";
        cout << separate;
        return;
    }

    for(int i = 0; i < inSource.size; i++) {
        const IncomeSource &s = inSource[i];
        cout << i + 1 << ". " << s.name << " (ID: " << s.id << ")\n";
    }

    cout << "DISPLAY ALL INCOME SOURCES: SUCCESS\n";
    cout << separate;
}