#include "../include/Statistics.h"
#include "../include/display.h"
#include "../include/Wallet.h"
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
void print_summary(PeriodSummary& s){
    cout << "Total Income: " << s.totalIncome << "\n";
    cout << "Total Expense: " << s.totalExpense << "\n";
    cout << "Net Balance: " << s.netBalance << "\n";
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
void calculate_wallet_stats(
    const DynamicArray<Transaction> &transactions,
    const DynamicArray<Wallet> &wallets,
    const Date &startDate,
    const Date &endDate,
    DynamicArray<PeriodSummary> &out
){
    out.size = 0;
    for (int i = 0; i < wallets.size; i++){
        out.push_back(PeriodSummary());
    }
    if (!is_valid_date(startDate) || !is_valid_date(endDate) || compare_dates(startDate, endDate) > 0) return;
    for (int i = 0; i < transactions.size; i++){
        const Transaction &t = transactions[i];
        if (!is_in_date_range(t.date, startDate, endDate)) continue;
        for (int w = 0; w < wallets.size; w++){
            if (wallets[w].id == t.walletID){
                if (t.isIncome) out[w].totalIncome += t.amount;
                else out[w].totalExpense += t.amount;
                break;
            }
        }
    }
    for (int w = 0; w < out.size; w++){
        out[w].netBalance = out[w].totalIncome - out[w].totalExpense;
    }
}
bool read_date_range(Date& startDate, Date& endDate)
{
    cout << "From date (dd mm yyyy): ";
    while (!(cin >> startDate.d >> startDate.m >> startDate.y) || !is_valid_date(startDate)){
        clear_line();
        cout << "\nInvalid start date. Please input the start date (dd mm yyyy) again: ";
    }

    cout << "To date (dd mm yyyy): ";
    while (!(cin >> endDate.d >> endDate.m >> endDate.y) || !is_valid_date(endDate)){
        clear_line();
        cout << "\nInvalid end date. Please input the end date (dd mm yyyy) again: ";
    }

    if (compare_dates(startDate, endDate) > 0){
        clear_line();
        cout << "Start date must be before or equal to end date.\n";
        ENTER(false);
        return 0;
    }
    return 1;
}
void show_time_stats(){
    clearScreen();
    cout << separate;        
    cout << "TIME-BASED STATISTICS\n";
    Date startDate, endDate;
    if (!read_date_range(startDate, endDate)) return;

    PeriodSummary s = calculate_time_stats(transList, startDate, endDate);
        
    cout << separate;
    cout << "From " << startDate.d << "/" << startDate.m << "/" << startDate.y 
         << " to " << endDate.d << "/" << endDate.m << "/" << endDate.y << "\n";
    cout << "Total Income: " << s.totalIncome << "\n";
    cout << "Total Expense: " << s.totalExpense << "\n";
    cout << "Net Balance: " << s.netBalance << "\n";
    cout << separate;
    ENTER(1);
}
void show_wallet_stats(){
    clearScreen();
    cout << separate;
    cout << "TIME & WALLET-BASED STATISTICS\n";
    if (walletList.size == 0){
        cout << "There is no wallet.\n";
        ENTER(1);
        return;
    }
    Date startDate, endDate;
    if (!read_date_range(startDate, endDate)) return;
    
    clear_line();
    cout << "Input wallet's Name you want: ";
    string name;
    getline(cin, name);
    int only = -1;
    if (!name.empty()){
        only = find_index_by_name(walletList, name);
        if (only == -1){
            cout << "This wallet does not exist.\n";
            ENTER(0);
            return;
        }
    }
    DynamicArray<PeriodSummary> result;
    calculate_wallet_stats(transList, walletList, startDate, endDate, result);

    cout << separate;
     cout << "From " << startDate.d << "/" << startDate.m << "/" << startDate.y 
         << " to " << endDate.d << "/" << endDate.m << "/" << endDate.y << "\n";
    for (int i = 0; i < walletList.size; i++){
        if (only != -1 && i != only) continue;
        cout << separate;
        cout << "Wallet: " << walletList[i].name << " (ID: " << walletList[i].id << ")\n";
        print_summary(result[i]);
    }
    cout << separate;
    ENTER(0);
}