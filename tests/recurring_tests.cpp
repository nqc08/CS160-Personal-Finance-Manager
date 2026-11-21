#include "RecurringTransaction.h"
#include "Wallet.h"
#include "Income.h"
#include "Expense.h"
#include "Storage.h"
#include <fstream>
#include <cstdint>
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

int main(int argc, char *argv[]) {
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

    // Seed an unprocessed record to exercise the real application's startup path.
    if (argc == 2 && string(argv[1]) == "--seed-startup") {
        RecurringTransaction pending = make_record("startup", true);
        pending.startDate = Date(1, 1, 2020);
        assert(add_recurring_transaction(pending));
        assert(saveData(walletList, inSource, expCategory, transList, recurringList));
        return 0;
    }

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
    // Save and reload all lists. Loading must not apply balances a second time.
    const int savedTransactions = transList.size;
    const long long savedBalance = walletList[0].balance;
    assert(saveData(walletList, inSource, expCategory, transList, recurringList));
    walletList.size = inSource.size = expCategory.size = transList.size = recurringList.size = 0;
    assert(loadData(walletList, inSource, expCategory, transList, recurringList) == LoadStatus::SUCCESS);
    assert(walletList.size == 1 && inSource.size == 1 && expCategory.size == 1);
    assert(recurringList.size == 2 && transList.size == savedTransactions);
    assert(walletList[0].balance == savedBalance);
    assert(recurringList[0].transaction.note == "Monthly test");
    assert(!recurringList[0].hasEndDate && recurringList[1].hasEndDate);
    assert(process_recurring_transactions(Date(2, 5, 2027)) == 0);
    assert(process_recurring_transactions(Date(1, 6, 2027)) == 1);
    assert(transList.size == savedTransactions + 1);

    // A truncated file is rejected without changing the current in-memory data.
    {
        std::ofstream bad("data/finance.dat", std::ios::binary | std::ios::trunc);
        bad << "PFM";
    }
    assert(loadData(walletList, inSource, expCategory, transList, recurringList) == LoadStatus::INVALID_FILE);
    assert(transList.size == savedTransactions + 1 && recurringList.size == 2);

    // Old version 1 saves remain readable (no transaction/recurring sections).
    {
        std::ofstream old("data/finance.dat", std::ios::binary | std::ios::trunc);
        old.write("PFMDATA1", 8);
        std::uint32_t version = 1, empty = 0;
        old.write(reinterpret_cast<const char*>(&version), sizeof(version));
        for (int i = 0; i < 3; i++) old.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
    }
    assert(loadData(walletList, inSource, expCategory, transList, recurringList) == LoadStatus::SUCCESS);
    assert(walletList.size == 0 && transList.size == 0 && recurringList.size == 0);
    assert(saveData(walletList, inSource, expCategory, transList, recurringList));
    assert(loadData(walletList, inSource, expCategory, transList, recurringList) == LoadStatus::SUCCESS);
    std::cout << "Recurring logic and storage checks passed.\n";
}
