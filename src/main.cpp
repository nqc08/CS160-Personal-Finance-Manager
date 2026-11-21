#include "../include/display.h"
#include "../include/DynamicArray.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"
#include "../include/Transaction.h"
#include "../include/Storage.h"
#include "../include/RecurringTransaction.h"
#include <limits>

#include<iostream>
#include<string>
using namespace std;

int main() {
    LoadStatus status = loadData(
        walletList,
        inSource,
        expCategory,
        transList,
        recurringList
    );

    switch (status) {
        case LoadStatus::SUCCESS:
            cout << "Data loaded successfully!\n";
            break;

        case LoadStatus::NO_FILE:
            cout << "First launch detected.\n";
            cout << "Starting with empty data.\n";
            break;

        case LoadStatus::INVALID_FILE:
            cerr << "Save file is corrupted or invalid!\n";
            return 1;
            
        case LoadStatus::OPEN_ERROR:
            cerr << "Cannot access the data directory or save file!\n";
            return 1;
    }

    auto saveCurrentData = [&]() -> bool {
        if (!saveData(
            walletList,
            inSource,
            expCategory,
            transList,
            recurringList
        )) {
            cerr << "Warning: Failed to save data!\n";
            return false;
        }

        return true;
    };

    int generated = process_recurring_transactions(current_date());
    // Persist transactions and monthly markers together before accepting more input.
    if (generated > 0) {
        if (!saveCurrentData()) return 1;
        cout << "Created " << generated << " recurring transaction(s).\n";
        ENTER(0);
    }

    while(true) {
        dashboard();

        int op; 
        cout << "Choose your option: ";
        if (!(cin >> op)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if(op == 0) break;

        if(op == 1) {
            while(true) {
                menuWallet();

                int opWallet; 
                cout << "Choose your option: "; cin >> opWallet;

                if(opWallet == 0) break;

                switch (opWallet) {
                    case 1: 
                        add_new_wallet(walletList);
                        saveCurrentData();
                        ENTER(1);
                        break;
                    case 2: 
                        rename_wallet(walletList);
                        saveCurrentData();
                        ENTER(0);
                        break; 
                    case 3: 
                        delete_wallet(walletList);
                        saveCurrentData();
                        ENTER(0);
                        break;
                    case 4:
                        display_wallets(walletList);
                        ENTER(1);
                        break;
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
                    case 1:
                        add_new_source(inSource);
                        saveCurrentData();
                        ENTER(0);
                        break; 
                    case 2:
                        rename_source(inSource);
                        saveCurrentData();
                        ENTER(0);
                        break; 
                    case 3:
                        delete_source(inSource);
                        saveCurrentData();
                        ENTER(0);
                        break;
                    case 4:
                        display_source(inSource);
                        ENTER(1);
                        break;
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
                    case 1:
                        add_new_category(expCategory);
                        saveCurrentData();
                        ENTER(0);
                        break; 
                    case 2: 
                        rename_category(expCategory);
                        saveCurrentData();
                        ENTER(0);
                        break; 
                    case 3:
                        delete_category(expCategory);
                        saveCurrentData();
                        ENTER(0);
                        break;
                    case 4: 
                        display_category(expCategory);
                        ENTER(1);
                        break;
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
                    case 1:
                        add_new_transaction(transList);
                        if (!saveCurrentData()) return 1;
                        ENTER(0);
                        break;
                    case 2: show_transaction_history(transList); ENTER(1); break;
                }
            }
        }
        if (op == 5) {
            recurring_menu();
            int added = process_recurring_transactions(current_date());
            if (!saveCurrentData()) return 1;
            if (added > 0) cout << "Created " << added << " recurring transaction(s).\n";
            ENTER(1);
        }
    }

    if (!saveCurrentData()) return 1;

    return 0;
}