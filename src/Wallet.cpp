#include<iostream>
#include<string>
#include "../include/Wallet.h"
#include "../include/DynamicArray.h"
#include "../include/display.h"
using namespace std;

DynamicArray<Wallet> walletList;

void add_new_wallet(DynamicArray<Wallet> &walletList) {
    cout << separate;
    cout << "CREATE A NEW WALLET\n";

    Wallet w;
    cout << "Input wallet's ID: "; cin >> w.id;
    cin.ignore();
    while(true) {
        cout << "Input a wallet's Name: "; getline(cin, w.name);
        if(find_index_by_name(walletList, w.name) == -1) break;

        cout << "Name existed, please choose another Name!\n";
    }
    cout << "Input initial balance: "; cin >> w.balance;

    walletList.push_back(w);

    cout << "CREATE A NEW WALLET: SUCCESS\n";
    cout << separate;
}

void rename_wallet(DynamicArray<Wallet> &walletList) {
    cout << separate;
    cout << "RENAME A WALLET\n";

    string name;
    cin.ignore();
    while(true) {
        cout << "Input a wallet's Name: "; getline(cin, name);
        if(find_index_by_name(walletList, name) != -1) break;

        cout << "This wallet has not existed yet, please choose another wallet!\n";
    }
    int idx = find_index_by_name(walletList, name);
    
    string newName;
    while(true) {
        cout << "Input a new Name: "; getline(cin, newName);
        if(find_index_by_name(walletList, newName) == -1) break;

        cout << "This Name has existed already, please choose another Name!\n";
    }

    walletList[idx].name = newName;

    cout << "RENAME A WALLET: SUCCESS\n";
    cout << separate;
}

void delete_wallet(DynamicArray<Wallet> &walletList) {
    cout << separate;
    cout << "DELETE A WALLET\n";

    if(walletList.size == 0) {
        cout << "There is no wallet\n";
        cout << separate;
        return;
    }

    string name;
    cin.ignore();
    while(true) {
        cout << "Input the wallet's Name that you want to delete: "; getline(cin, name);
        if(find_index_by_name(walletList, name) != -1) break;

        cout << "There is no wallet with this name, please choose another wallet!\n";
    }

    int idx = find_index_by_name(walletList, name);
    walletList.remove(idx);

    cout << "DELETE A WALLET: SUCCESS\n";
    cout << separate;
}

int find_index_by_name(DynamicArray<Wallet> &walletList, const string &name) {
    for(int i = 0; i < walletList.size; i++) {
        if(walletList[i].name == name) return i;
    }
    return -1;
}

void display_wallets(const DynamicArray<Wallet> &walletList) {
    cout << separate;
    cout << "DISPLAY ALL WALLETS\n";

    if(walletList.size == 0) {
        cout << "There is no wallet\n";
        cout << separate;
        return;
    }

    for(int i = 0; i < walletList.size; i++) {
        const Wallet &w = walletList[i];
        cout << i + 1 << ". " << w.name << " (ID: " << w.id << ") - Balance: " << w.balance << '\n';
    }

    cout << "DISPLAY ALL WALLET: SUCCESS\n";
    cout << separate;
}

long long total_balance(const DynamicArray<Wallet> &walletList) {
    long long tot = 0;
    for(int i = 0; i < walletList.size; i++) {
        tot += walletList[i].balance;
    }
    return tot;
}