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
void statistics_menu();
#endif