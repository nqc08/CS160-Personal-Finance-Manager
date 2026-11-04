#include<iostream>
#include<string>
#include "../include/display.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
using namespace std;

int main() {
    cout << separate;
    cout << "PERSONAL FINANCE MANAGER\n";
    cout << "Total Balance: " << total_balance(walletList) << '\n';
    if(walletList.size == 0) {
        cout << "There is no wallet\n";
    }
    else {
        for(int i = 0; i < walletList.size; i++) {
            const Wallet &w = walletList[i];
            cout << i + 1 << ". " << w.name << " (ID: " << w.id << ") - Balance: " << w.balance << '\n';
        }
    }
    cout << separate;

    while(true) {
        dashboard();

        int op; 
        cout << "Choose your option: "; cin >> op;
        
        if(op == 0) break;

        if(op == 1) {
            while(true) {
                displayWallet();

                int opWallet; 
                cout << "Choose your option: "; cin >> opWallet;

                if(opWallet == 0) break;

                switch (opWallet) {
                    case 1: add_new_wallet(walletList); break;
                    case 2: rename_wallet(walletList); break;
                    case 3: delete_wallet(walletList); break;
                    case 4: display_wallets(walletList); break;
                }
            }
        }

        if(op == 2) {
            while(true) {
                displayIncomeSource();

                int opSrc;
                cout << "Choose your option: "; cin >> opSrc;

                if(opSrc == 0) break;

                switch (opSrc) {
                    case 1: add_new_source(inSource); break;
                    case 2: rename_source(inSource); break;
                    case 3: delete_source(inSource); break;
                    case 4: display_source(inSource); break;
                }
            }
        }
    }

    return 0;
}