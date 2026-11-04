#pragma once

#include<iostream>
#include<string>
#include "DynamicArray.h"
using namespace std;

struct Wallet {
    string id, name;
    double balance;

    bool operator == (const Wallet &other) const { return id == other.id; }
};

extern DynamicArray<Wallet> walletList;

void add_new_wallet(DynamicArray<Wallet> &walletList);

void rename_wallet(DynamicArray<Wallet> &walletList);

void delete_wallet(DynamicArray<Wallet> &walletList);

int find_index_by_name(DynamicArray<Wallet> &walletList, const string &name);

void display_wallets(const DynamicArray<Wallet> &walletList);

long long total_balance(const DynamicArray<Wallet> &walletList);
