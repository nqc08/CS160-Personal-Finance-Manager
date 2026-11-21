#ifndef RECURRING_TRANSACTION_H
#define RECURRING_TRANSACTION_H

#include "Transaction.h"

struct RecurringTransaction {
    string id;
    Transaction transaction; // Template; its date is replaced when generated.
    Date startDate;
    Date endDate;
    bool hasEndDate;
    int lastGeneratedMonth;
    int lastGeneratedYear;

    RecurringTransaction() : hasEndDate(false), lastGeneratedMonth(0), lastGeneratedYear(0) {
        transaction.isIncome = false;
        transaction.amount = 0;
    }
};

extern DynamicArray<RecurringTransaction> recurringList;

Date current_date();
bool add_recurring_transaction(const RecurringTransaction &record);
int process_recurring_transactions(const Date &today);
void recurring_menu();

#endif
