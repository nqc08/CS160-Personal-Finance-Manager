#include "../include/display.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"
#include "../include/Transaction.h"

#include<iostream>
#include<string>
using namespace std;

int main() {
    while(true) {
        dashboard();

        int op; 
        cout << "Choose your option: "; cin >> op;
        
        if(op == 0) break;

        if(op == 1) {
            while(true) {
                menuWallet();

                int opWallet; 
                cout << "Choose your option: "; cin >> opWallet;

                if(opWallet == 0) break;

                switch (opWallet) {
                    case 1: add_new_wallet(walletList); ENTER(1); break;
                    case 2: rename_wallet(walletList); ENTER(0); break; // there has been an cin.ignore() already after running this function
                    case 3: delete_wallet(walletList); ENTER(0); break;
                    case 4: display_wallets(walletList); ENTER(1); break;
                }
            }
        }

        if(op == 2) {
            while(true) {
                menuIncomeSource();

                int opSrc;
                cout << "Choose your option: "; cin >> opSrc;

                if(opSrc == 0) break;

                switch (opSrc) {
                    case 1: add_new_source(inSource); ENTER(0); break; // there has been an cin.ignore() already after running this function
                    case 2: rename_source(inSource); ENTER(0); break; 
                    case 3: delete_source(inSource); ENTER(0); break;
                    case 4: display_source(inSource); ENTER(1); break;
                }
            }
        }

        if(op == 3) {
            while(true) {
                menuExpenseCategory();

                int opCate;
                cout << "Choose your option: "; cin >> opCate;

                if(opCate == 0) break;

                switch (opCate) {
                    case 1: add_new_category(expCategory); ENTER(0); break; 
                    case 2: rename_category(expCategory); ENTER(0); break; 
                    case 3: delete_category(expCategory); ENTER(0); break;
                    case 4: display_category(expCategory); ENTER(1); break;
                }
            }
        }

        if(op == 4) {
            while(true) {
                menuTransaction();

                int opTrans;
                cout << "Choose your option: "; cin >> opTrans;

                if(opTrans == 0) break;

                switch (opTrans) {
                    case 1: add_new_transaction(transList); ENTER(0); break;
                    case 2: show_transaction_history(transList); ENTER(1); break;
                }
            }
        }
    }

    return 0;
}