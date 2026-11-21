# Monthly recurring transactions

## Build and run

From the project root:

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o PersonalFinanceManager
./PersonalFinanceManager
```

C++17 is needed by the existing storage module's `<filesystem>` usage.
The application uses C++ structs, the existing manual dynamic array, `std::string`,
and binary files through `<fstream>`. No STL containers or external libraries were added.

## Using the feature

1. Create a wallet and an income source or expense category using the existing menus.
2. Select dashboard option **5. Recurring Transactions**.
3. Select **1. Add monthly transaction**.
4. Enter a unique recurring ID, type, existing wallet and source/category names,
   positive amount, description, start date, and optional end date.
5. The application checks eligibility, saves the data, and returns to the dashboard
   after Enter. Select option 5 then 2 to view configurations.

Example: configure a salary of 5000000 with wallet Cash, source Salary,
start date 1 November 2026, and no end date. The first eligible opening in
November records 5000000. Opening again that month adds nothing. The first
opening in December creates one new transaction.

Generation runs after loading at startup and after using the recurring menu.
Start and end dates are inclusive. Each generated transaction is dated today.
There is no specific payment day or catch-up for months when the application
was not opened. These are simple interpretations of the guide's current-month requirement.
Moving the clock to an earlier month does not cause another generation.

## Structure

`include/RecurringTransaction.h` defines:

| Field | Meaning |
|---|---|
| `id` | Unique configuration ID |
| `transaction` | Existing Transaction struct used as a template |
| `startDate` | First eligible date |
| `endDate` | Last eligible date, when enabled |
| `hasEndDate` | Whether an end date is used |
| `lastGeneratedMonth` | Last processed month; initially 0 |
| `lastGeneratedYear` | Corresponding year; initially 0 |

The template stores IDs for wallets and sources/categories, not names.
Its date is unused; the generated copy receives today's date.
`recurringList` is a `DynamicArray<RecurringTransaction>` using the existing
array implementation. Do not copy a DynamicArray by value; pass it by reference.

## Main functions

- `current_date()`: obtains today's local date using `<ctime>`.
- `add_recurring_transaction(record)`: checks the ID, amount, dates, and referenced
  wallet/source/category, then appends a configuration with empty generation markers.
- `process_recurring_transactions(today)`: loops through records, checks date limits
  and generation markers, creates eligible transactions, and returns their count.
  Accepting a date parameter allows tests to use specific months without changing the clock.
- `record_transaction(transaction, transactions)`: validates an ordinary transaction,
  appends it to the supplied list, and increases or decreases its wallet balance.
  Both manual and recurring transactions use this function.
- `recurring_menu()`: handles the add/view CLI and returns after one action.
- `is_valid_date()` and `compare_dates()`: check calendar dates and compare them.

Processing order:

```text
Load all saved lists
For each recurring configuration:
    Skip if today is before start or after the optional end
    Skip if this month or a later month was already processed
    Copy the template and set its date to today
    Record the transaction and update its wallet
    If successful, mark this month/year as processed
Save all lists together if anything was generated
Show dashboard
```

Missing wallets or sources/categories are skipped and remain unprocessed.
The view identifies deleted references. Historical transactions can still be
viewed after a referenced wallet, source, or category is deleted.

## Storage integration

`saveData()` and `loadData()` now accept transaction and recurring arrays in addition
to wallets, sources, and categories. Update other callers to pass all five arrays.

The binary format is version 2. Old version 1 files still load, with empty
transaction and recurring lists. History already lost by the old implementation
cannot be recovered. Save paths remain `data/finance.dat`, relative to where the
program is launched, so consistently run it from the project root.

Strings and dates are written field by field. Transactions, wallet balances,
and generation markers are written to the same temporary file before renaming
it into place. Loading restores balances directly; it does not replay transactions.
If saving generated entries fails, the application exits with an error rather
than continuing to accept transactions with unsaved monthly markers.

## Verification

Run from the project root (Python 3 and g++ required):

```bash
python3 tests/run_tests.py
```

The runner builds the C++ program and tests inside a temporary directory.
It does not overwrite application data in the repository.

The C++ checks cover calendar validation, invalid configuration data, duplicate
IDs, income and expenses, start/end boundaries, indefinite recurrence, monthly
and yearly changes, repeated processing, moving the clock backwards, skipped
months, missing references, persistence, corrupted files, and version 1 loading.

The CLI checks create a recurring income, reopen the application, verify no
same-month duplicate, inspect configuration names and history, verify ordinary
transaction persistence, and test generation of a pending record at startup.

Known scope limits: no edit/delete recurring menu, no missed-month backfill,
and no statistics module. Older menus still have their existing input-handling
limitations. This module does not change how IDs are manually assigned to
wallets, sources, or categories; do not reuse a deleted entity's ID for a different entity.
