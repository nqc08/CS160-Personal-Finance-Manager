#include "../include/RecurringTransaction.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"
#include <ctime>
#include <iostream>
#include <limits>
using namespace std;

DynamicArray<RecurringTransaction> recurringList;

Date current_date() {
    time_t now = time(0);
    tm *local = localtime(&now);
    return Date(local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

bool add_recurring_transaction(const RecurringTransaction &r) {
    if (r.id.empty() || r.transaction.amount <= 0 || !is_valid_date(r.startDate)) return false;
    if (r.hasEndDate && (!is_valid_date(r.endDate) || compare_dates(r.endDate, r.startDate) < 0)) return false;
    for (int i = 0; i < recurringList.size; i++) {
        if (recurringList[i].id == r.id) return false;
    }
    if (find_index_by_ID(walletList, r.transaction.walletID) == -1) return false;
    int source = r.transaction.isIncome ? find_index_by_ID(inSource, r.transaction.srcID)
                                        : find_index_by_ID(expCategory, r.transaction.srcID);
    if (source == -1) return false;
    RecurringTransaction copy = r;
    copy.lastGeneratedMonth = 0;
    copy.lastGeneratedYear = 0;
    recurringList.push_back(copy);
    return true;
}

int process_recurring_transactions(const Date &today) {
    if (!is_valid_date(today)) return 0;
    int generated = 0;
    for (int i = 0; i < recurringList.size; i++) {
        RecurringTransaction &r = recurringList[i];
        if (compare_dates(today, r.startDate) < 0) continue;
        if (r.hasEndDate && compare_dates(today, r.endDate) > 0) continue;
        if (r.lastGeneratedYear > today.y ||
            (r.lastGeneratedYear == today.y && r.lastGeneratedMonth >= today.m)) continue;
        Transaction t = r.transaction;
        t.date = today;
        // Missing wallets or sources/categories are skipped without marking the month.
        if (record_transaction(t)) {
            r.lastGeneratedMonth = today.m;
            r.lastGeneratedYear = today.y;
            generated++;
        }
    }
    return generated;
}

// Each action returns to the dashboard. No extra edit/delete features are required.
void recurring_menu() {
    cout << "\nRECURRING TRANSACTIONS\n1. Add monthly transaction\n2. View recurring transactions\n0. Back\nChoose: ";
    int option;
    if (!(cin >> option)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;
    }
    if (option == 0) return;
    if (option == 2) {
        if (recurringList.size == 0) cout << "No recurring transactions.\n";
        for (int i = 0; i < recurringList.size; i++) {
            const RecurringTransaction &r = recurringList[i];
            const Transaction &t = r.transaction;
            int w = find_index_by_ID(walletList, t.walletID);
            int s = t.isIncome ? find_index_by_ID(inSource, t.srcID) : find_index_by_ID(expCategory, t.srcID);
            string source = "[Deleted] " + t.srcID;
            if (s != -1) source = t.isIncome ? inSource[s].name : expCategory[s].name;
            cout << r.id << " | " << (t.isIncome ? "Income" : "Expense") << " | " << t.amount
                 << " | " << (w == -1 ? "[Deleted] " + t.walletID : walletList[w].name)
                 << " | " << source << " | " << t.note << '\n';
            cout << "Start: " << r.startDate.d << '/' << r.startDate.m << '/' << r.startDate.y;
            if (r.hasEndDate) cout << " | End: " << r.endDate.d << '/' << r.endDate.m << '/' << r.endDate.y;
            else cout << " | No end date";
            cout << " | Last generated: " << r.lastGeneratedMonth << '/' << r.lastGeneratedYear << '\n';
        }
        return;
    }
    if (option != 1) { cout << "Invalid option.\n"; return; }
    RecurringTransaction r;
    int type, end;
    string walletName, sourceName;
    cout << "Recurring ID: ";
    if (!(cin >> r.id)) return;
    cout << "Type (1 = income, 2 = expense): ";
    if (!(cin >> type) || (type != 1 && type != 2)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid type.\n"; return;
    }
    r.transaction.isIncome = type == 1;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Wallet name: "; getline(cin, walletName);
    cout << (type == 1 ? "Income source name: " : "Expense category name: "); getline(cin, sourceName);
    int w = find_index_by_name(walletList, walletName);
    int s = type == 1 ? find_index_by_name(inSource, sourceName) : find_index_by_name(expCategory, sourceName);
    if (w == -1 || s == -1) { cout << "Wallet or source/category does not exist.\n"; return; }
    r.transaction.walletID = walletList[w].id;
    r.transaction.srcID = type == 1 ? inSource[s].id : expCategory[s].id;
    cout << "Amount: ";
    if (!(cin >> r.transaction.amount)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount.\n"; return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Description: "; getline(cin, r.transaction.note);
    cout << "Start date (dd mm yyyy): ";
    if (!(cin >> r.startDate.d >> r.startDate.m >> r.startDate.y)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid date.\n"; return;
    }
    cout << "Has end date? (1 = yes, 0 = no): ";
    if (!(cin >> end) || (end != 0 && end != 1)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid option.\n"; return;
    }
    r.hasEndDate = end == 1;
    if (r.hasEndDate) {
        cout << "End date (dd mm yyyy): ";
        if (!(cin >> r.endDate.d >> r.endDate.m >> r.endDate.y)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid date.\n"; return;
        }
    }
    if (add_recurring_transaction(r)) cout << "Recurring transaction added.\n";
    else cout << "Not added: check ID uniqueness, positive amount, and valid start/end dates.\n";
}
