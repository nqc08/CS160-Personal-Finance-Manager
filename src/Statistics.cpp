#include "../include/Statistics.h"
#include "../include/display.h"
#include <iostream>
#include <limits>
using namespace std;
static bool is_in_date_range(const Date &d, const Date &start, const Date &end){
    return compare_dates(d, start) >= 0 && compare_dates(d, end) <= 0;
}
static void clear_line(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
static void pause_screen(bool needIgnore){
    if(needIgnore) cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "ENTER to continue";
    cin.get();
}
PeriodSummary calculate_time_stats(
    const DynamicArray<Transaction> &transactions,
    const Date &startDate,
    const Date &endDate
){
    PeriodSummary res;
    if (!is_valid_date(startDate) || !is_valid_date(endDate) || compare_dates(startDate, endDate) > 0){
        return res;
    }
    for (int i = 0; i < transactions.size; i++){
        const Transaction &t = transactions[i];
        if (is_in_date_range(t.date, startDate, endDate)){
            if (t.isIncome) res.totalIncome += t.amount;
            else res.totalExpense += t.amount;
        }
    }
    res.netBalance = res.totalIncome - res.totalExpense;
    return res;
}
void statistics_menu(){
    clearScreen();
    cout << separate;
    cout << "STATISTICS & REPORTING\n";
    cout << "0. Dashboard\n";
    cout << "1. Time_based Statistics\n";
    cout << separate;
    cout << "Choose your option: ";
    
    int option;
    if (!(cin >> option)){
        clear_line();
        cout << "Invalid input.\n";
        pause_screen(false);
        return;
    }

    if (option == 0) return;

    if (option == 1){
        clearScreen();
        cout << separate;
        cout << "TINE-BASED STATISTICS\n";

        Date startDate, endDate;
        cout << "From date (dd mm yyyy): ";
        if (!(cin >> startDate.d >> startDate.m >> startDate.y) || !is_valid_date(startDate)){
            clear_line();
            cout << "Invalid start date.\n";
            pause_screen(false);
            return;
        }

        cout << "To date (dd mm yyyy): ";
        if (!(cin >> endDate.d >> endDate.m >> endDate.y) || !is_valid_date(endDate)){
            clear_line();
            cout << "Invalid end date.\n";
            pause_screen(false);
            return;
        }

        if (compare_dates(startDate, endDate) > 0){
            clear_line();
            cout << "Start date mus be before or equal to end date.\n";
            pause_screen(false);
            return;
        }

        PeriodSummary s = calculate_time_stats(transList, startDate, endDate);
        
        cout << separate;
        cout << "From " << startDate.d << "/" << startDate.m << "/" << startDate.y 
             << " to " << endDate.d << "/" << endDate.m << "/" << endDate.y << "\n";
        cout << "Total Income: " << s.totalIncome << "\n";
        cout << "Total Expense: " << s.totalExpense << "\n";
        cout << "Net Balance: " << s.netBalance << "\n";
        cout << separate;

        pause_screen(true);
        return;
    }
    cout << "Invalid option.\n";
    pause_screen(true);
}