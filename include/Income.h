#pragma once

#include<string>
#include "DynamicArray.h"
using namespace std;

struct IncomeSource {
    string id, name;
};

extern DynamicArray<IncomeSource> inSource;

void add_new_source(DynamicArray<IncomeSource> &inSource);

void rename_source(DynamicArray<IncomeSource> &inSource);

void delete_source(DynamicArray<IncomeSource> &inSource);

int find_index_by_name(DynamicArray<IncomeSource> &inSource, const string &name);

void display_source(const DynamicArray<IncomeSource> &inSource);