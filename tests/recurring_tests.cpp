#include "RecurringTransaction.h"
#include "Wallet.h"
#include "Income.h"
#include "Expense.h"
#include <cassert>
#include <iostream>

RecurringTransaction make_record(const string &id, bool income) {
    RecurringTransaction r;
    r.id = id;
    r.transaction.isIncome = income;
    r.transaction.walletID = "W1";
    r.transaction.srcID = income ? "S1" : "C1";
    r.transaction.amount = 100;
    r.transaction.note = "Monthly test";
    r.startDate = Date(15, 11, 2026);
    return r;
}

int main() {
    Wallet w; w.id = "W1"; w.name = "Cash"; w.balance = 1000;
    walletList.push_back(w);
    IncomeSource s; s.id = "S1"; s.name = "Salary"; inSource.push_back(s);
    ExpenseCategory c; c.id = "C1"; c.name = "Food"; expCategory.push_back(c);

    assert(is_valid_date(Date(29, 2, 2024)));
    assert(!is_valid_date(Date(29, 2, 2026)));
    assert(!is_valid_date(Date(31, 4, 2026)));
    assert(!is_valid_date(Date(1, 13, 2026)));
    assert(!is_valid_date(Date(0, 1, 2026)));
    assert(compare_dates(Date(31, 12, 2026), Date(1, 1, 2027)) < 0);

    RecurringTransaction income = make_record("R1", true);
    assert(add_recurring_transaction(income));
    assert(!add_recurring_transaction(income)); // Duplicate ID.
    RecurringTransaction invalid = make_record("bad", true);
    invalid.transaction.amount = 0;
    assert(!add_recurring_transaction(invalid));
    invalid.transaction.amount = -1;
    assert(!add_recurring_transaction(invalid));
    invalid = make_record("bad", true);
    invalid.startDate = Date(31, 11, 2026);
    assert(!add_recurring_transaction(invalid));
    invalid = make_record("bad", true);
    invalid.hasEndDate = true;
    invalid.endDate = Date(14, 11, 2026);
    assert(!add_recurring_transaction(invalid));
    invalid = make_record("bad", true);
    invalid.transaction.walletID = "missing";
    assert(!add_recurring_transaction(invalid));
    invalid = make_record("bad", true);
    invalid.transaction.srcID = "missing";
    assert(!add_recurring_transaction(invalid));

    assert(process_recurring_transactions(Date(14, 11, 2026)) == 0);
    assert(process_recurring_transactions(Date(15, 11, 2026)) == 1);
    assert(transList.size == 1 && walletList[0].balance == 1100);
    assert(transList[0].date.d == 15 && transList[0].isIncome);
    assert(process_recurring_transactions(Date(21, 11, 2026)) == 0);
    assert(process_recurring_transactions(Date(1, 12, 2026)) == 1);
    assert(process_recurring_transactions(Date(1, 1, 2027)) == 1);
    assert(process_recurring_transactions(Date(1, 12, 2026)) == 0); // Clock moved backwards.
    assert(process_recurring_transactions(Date(31, 2, 2027)) == 0);

    // An expense can run on its end date, but not after it.
    RecurringTransaction expense = make_record("R2", false);
    expense.hasEndDate = true;
    expense.endDate = Date(1, 1, 2027);
    assert(add_recurring_transaction(expense));
    assert(process_recurring_transactions(Date(1, 1, 2027)) == 1);
    assert(!transList[transList.size - 1].isIncome);
    assert(walletList[0].balance == 1200);
    assert(process_recurring_transactions(Date(2, 1, 2027)) == 0);

    // A missing wallet/source is skipped and the month remains unprocessed.
    walletList[0].id = "temporarily-missing";
    assert(process_recurring_transactions(Date(1, 2, 2027)) == 0);
    assert(recurringList[0].lastGeneratedMonth == 1);
    walletList[0].id = "W1";
    inSource[0].id = "temporarily-missing";
    assert(process_recurring_transactions(Date(1, 2, 2027)) == 0);
    inSource[0].id = "S1";
    assert(process_recurring_transactions(Date(1, 2, 2027)) == 1);
    assert(process_recurring_transactions(Date(1, 5, 2027)) == 1); // No backfill.
    assert(process_recurring_transactions(Date(1, 5, 2027)) == 0);
    std::cout << "Recurring logic checks passed.\n";
}
