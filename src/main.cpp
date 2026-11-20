#include<iostream>
#include<string>
#include "../include/display.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"
#include "../include/Storage.h"
using namespace std;

int main() {

    LoadStatus status = loadData(
        walletList,
        inSource,
        expCategory
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
            expCategory
        )) {
            cerr << "Warning: Failed to save data!\n";
            return false;
        }

        return true;
    };

    while(true) {
        clearScreen();
        dashboard();

        int op; 
        cout << "Choose your option: "; cin >> op;
        
        if(op == 0) break;

        if(op == 1) {
            while(true) {
                clearScreen();
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
                        break; // there has been an cin.ignore() already after running this function
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
                clearScreen();
                menuIncomeSource();

                int opSrc;
                cout << "Choose your option: "; cin >> opSrc;

                if(opSrc == 0) break;

                switch (opSrc) {
                    case 1:
                        add_new_source(inSource);
                        saveCurrentData();
                        ENTER(0);
                        break; // there has been an cin.ignore() already after running this function
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
                clearScreen();
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
    }

    if (!saveCurrentData()) return 1;

    return 0;
}