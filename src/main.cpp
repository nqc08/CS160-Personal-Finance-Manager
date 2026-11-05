#include<iostream>
#include<string>
#include "../include/display.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
using namespace std;

int main() {
    while(true) {
        clearScreen();
        dashboard();

        int op; 
        cout << "Choose your option: "; cin >> op;
        
        if(op == 0) break;

        if(op == 1) {
            while(true) {
                clearScreen();
                displayWallet();

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
                clearScreen();
                displayIncomeSource();

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
    }

    return 0;
}