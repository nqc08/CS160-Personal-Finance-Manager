#ifndef DISPLAY_H
#define DISPLAY_H

#include<string>
using namespace std;

extern string separate;

void dashboard();

void menuWallet();

void menuIncomeSource();

void menuExpenseCategory();

void menuTransaction();

void clearScreen();

void ENTER(bool needIgnore);

#endif