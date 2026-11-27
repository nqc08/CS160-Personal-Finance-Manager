#ifndef STATISTICS_H
#define STATISTICS_H

#include "DynamicArray.h"
#include "Transaction.h"

struct PeriodSummary{
    long long totalIncome;
    long long totalExpense;
    long long netBalance;

    PeriodSummary() : totalIncome(0), totalExpense(0), netBalance(0){}
};
PeriodSummary calculate_time_stats(
    const DynamicArray<Transaction> &transactions,
    const Date &startDate,
    const Date &endDate
);
void print_summary(PeriodSummary& s);
void show_time_stats();
bool read_date_range(Date& startDate, Date& endDate);
void show_wallet_stats();
#endif