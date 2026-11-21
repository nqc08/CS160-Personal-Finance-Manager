#ifndef STORAGE_H
#define STORAGE_H

#include "Wallet.h"
#include "Income.h"
#include "Expense.h"
#include "DynamicArray.h"
#include "RecurringTransaction.h"

enum class LoadStatus {
    SUCCESS,
    NO_FILE,
    INVALID_FILE,
    OPEN_ERROR
};

bool saveData(
    const DynamicArray<Wallet>& wallets,
    const DynamicArray<IncomeSource>& sources,
    const DynamicArray<ExpenseCategory>& categories,
    const DynamicArray<Transaction>& transactions,
    const DynamicArray<RecurringTransaction>& recurring
);

LoadStatus loadData(
    DynamicArray<Wallet>& wallets,
    DynamicArray<IncomeSource>& sources,
    DynamicArray<ExpenseCategory>& categories,
    DynamicArray<Transaction>& transactions,
    DynamicArray<RecurringTransaction>& recurring
);

#endif
