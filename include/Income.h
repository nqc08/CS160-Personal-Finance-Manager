#ifndef INCOME_H
#define INCOME_H

#include "DynamicArray.h"

#include<string>
using namespace std;

struct IncomeSource {
    string id, name;
};

extern DynamicArray<IncomeSource> inSource;

void add_new_source(DynamicArray<IncomeSource> &inSource);

void rename_source(DynamicArray<IncomeSource> &inSource);

void delete_source(DynamicArray<IncomeSource> &inSource);

int find_index_by_name(DynamicArray<IncomeSource> &inSource, const string &name);

int find_index_by_ID(DynamicArray<IncomeSource> &inSource, const string &id);

void display_source(const DynamicArray<IncomeSource> &inSource);

#endif