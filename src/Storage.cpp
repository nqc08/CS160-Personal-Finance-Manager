#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <system_error>

#include "../include/Storage.h"
#include "../include/Wallet.h"
#include "../include/Income.h"
#include "../include/Expense.h"
#include "../include/DynamicArray.h"

namespace fs = std::filesystem;

const std::uint32_t MAX_STRING_BYTES = 1024u * 1024u;

bool writeString (std::ofstream& out, const std::string& str) {
    if (str.size() > MAX_STRING_BYTES) return false;
    std::uint32_t length = static_cast<std::uint32_t>(str.size());
    
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    if (!out) return false;

    if (length > 0) {
        out.write(
            str.data(),
            static_cast<std::streamsize>(length)
        );

    }
    return static_cast<bool>(out);
}

bool readString (std::ifstream& in, std::string& str) {
    std::uint32_t length{};
    in.read(
        reinterpret_cast<char*>(&length),
        sizeof(length)
        );
    if (!in) return false;

    if (length > MAX_STRING_BYTES) return false;

    std::string temp(length, '\0');

    if (length > 0) {
        in.read(
            &temp[0],
            static_cast<std::streamsize>(length)
        );
    }
    if (!in) return false;

    str.swap(temp);
    return true;
}

bool writeWallet (std::ofstream& out, const Wallet& w) {
    if (!writeString(out, w.id)) return false;
    if (!writeString(out, w.name)) return false;

    out.write(
        reinterpret_cast<const char*>(&w.balance),
        sizeof(w.balance)
    );

    return static_cast<bool>(out);
}

bool readWallet (std::ifstream& in, Wallet& w) {
    Wallet temp;

    if (!readString(in, temp.id)) return false;
    if (!readString(in, temp.name)) return false;

    in.read(
        reinterpret_cast<char*>(&temp.balance),
        sizeof(temp.balance)
    );

    if (!in) return false;

    w = temp;

    return true;
}

bool writeIncomeSource(std::ofstream& out, const IncomeSource& source) {
    if (!writeString(out, source.id)) return false;
    if (!writeString(out, source.name)) return false;

    return static_cast<bool>(out);
}

bool readIncomeSource(std::ifstream& in, IncomeSource& source) {
    IncomeSource temp;

    if (!readString(in, temp.id)) return false;
    if (!readString(in, temp.name)) return false;

    if (!in) return false;

    source = temp;

    return true;
}


bool writeExpenseCategory(std::ofstream& out, const ExpenseCategory& category) {
    if (!writeString(out, category.id)) return false;
    if (!writeString(out, category.name)) return false;

    return static_cast<bool>(out);
}

bool readExpenseCategory(std::ifstream& in, ExpenseCategory& category) {
    ExpenseCategory temp;

    if (!readString(in, temp.id)) return false;
    if (!readString(in, temp.name)) return false;

    if (!in) return false;

    category = temp;

    return true;
}

const std::uint32_t MAX_ITEMS = 100000;

bool writeCount(std::ofstream& out, int count) {
    if (count < 0 || static_cast<std::uint32_t>(count) > MAX_ITEMS) return false;

    std::uint32_t value = static_cast<std::uint32_t>(count);

    out.write(
        reinterpret_cast<const char*>(&value),
        sizeof(value)
    );

    return static_cast<bool>(out);
}

bool readCount(std::ifstream& in, std::uint32_t& count) {
    in.read(
        reinterpret_cast<char*>(&count),
        sizeof(count)
    );

    if (!in) return false;

    if (count > MAX_ITEMS) return false;

    return true;
}

// Dates and strings are written field by field, never as a raw struct.
bool writeDate(std::ofstream& out, const Date& date) {
    out.write(reinterpret_cast<const char*>(&date.d), sizeof(date.d));
    out.write(reinterpret_cast<const char*>(&date.m), sizeof(date.m));
    out.write(reinterpret_cast<const char*>(&date.y), sizeof(date.y));
    return static_cast<bool>(out);
}

bool readDate(std::ifstream& in, Date& date) {
    in.read(reinterpret_cast<char*>(&date.d), sizeof(date.d));
    in.read(reinterpret_cast<char*>(&date.m), sizeof(date.m));
    in.read(reinterpret_cast<char*>(&date.y), sizeof(date.y));
    return static_cast<bool>(in);
}

bool writeTransaction(std::ofstream& out, const Transaction& t) {
    int type = t.isIncome ? 1 : 0;
    if (!writeDate(out, t.date)) return false;
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    if (!writeString(out, t.srcID) || !writeString(out, t.walletID) || !writeString(out, t.note)) return false;
    out.write(reinterpret_cast<const char*>(&t.amount), sizeof(t.amount));
    return static_cast<bool>(out);
}

bool readTransaction(std::ifstream& in, Transaction& t) {
    int type = -1;
    if (!readDate(in, t.date)) return false;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    if (!in || (type != 0 && type != 1)) return false;
    t.isIncome = type == 1;
    if (!readString(in, t.srcID) || !readString(in, t.walletID) || !readString(in, t.note)) return false;
    in.read(reinterpret_cast<char*>(&t.amount), sizeof(t.amount));
    return in && t.amount > 0 && is_valid_date(t.date);
}

bool writeRecurring(std::ofstream& out, const RecurringTransaction& r) {
    // Give the template a valid date in the file; generation uses today's date.
    Transaction t = r.transaction;
    t.date = r.startDate;
    if (!writeString(out, r.id) || !writeTransaction(out, t)) return false;
    if (!writeDate(out, r.startDate) || !writeDate(out, r.endDate)) return false;
    int hasEnd = r.hasEndDate ? 1 : 0;
    out.write(reinterpret_cast<const char*>(&hasEnd), sizeof(hasEnd));
    out.write(reinterpret_cast<const char*>(&r.lastGeneratedMonth), sizeof(r.lastGeneratedMonth));
    out.write(reinterpret_cast<const char*>(&r.lastGeneratedYear), sizeof(r.lastGeneratedYear));
    return static_cast<bool>(out);
}

bool readRecurring(std::ifstream& in, RecurringTransaction& r) {
    if (!readString(in, r.id) || !readTransaction(in, r.transaction)) return false;
    if (!readDate(in, r.startDate) || !readDate(in, r.endDate)) return false;
    int hasEnd = -1;
    in.read(reinterpret_cast<char*>(&hasEnd), sizeof(hasEnd));
    in.read(reinterpret_cast<char*>(&r.lastGeneratedMonth), sizeof(r.lastGeneratedMonth));
    in.read(reinterpret_cast<char*>(&r.lastGeneratedYear), sizeof(r.lastGeneratedYear));
    if (!in || (hasEnd != 0 && hasEnd != 1) || r.id.empty() || !is_valid_date(r.startDate)) return false;
    r.hasEndDate = hasEnd == 1;
    if (r.hasEndDate && (!is_valid_date(r.endDate) || compare_dates(r.endDate, r.startDate) < 0)) return false;
    bool neverGenerated = r.lastGeneratedMonth == 0 && r.lastGeneratedYear == 0;
    bool validMonth = r.lastGeneratedMonth >= 1 && r.lastGeneratedMonth <= 12 && r.lastGeneratedYear >= 1;
    return neverGenerated || validMonth;
}

bool saveData(
    const DynamicArray<Wallet>& wallets,
    const DynamicArray<IncomeSource>& sources,
    const DynamicArray<ExpenseCategory>& categories,
    const DynamicArray<Transaction>& transactions,
    const DynamicArray<RecurringTransaction>& recurring
) {
    const fs::path tempPath = "data/finance.tmp";
    const fs::path finalPath = "data/finance.dat";

    std::ofstream out(tempPath, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return false;

    auto fail = [&]() -> bool {
        out.close();
        std::error_code ec;
        fs::remove(tempPath, ec);
        return false;
    };

    const char magic[8] = {'P', 'F', 'M', 'D', 'A', 'T', 'A', '1'};
    out.write(magic, sizeof(magic));
    const std::uint32_t version = 2;
    out.write(
        reinterpret_cast<const char*>(&version),
        sizeof(version)
    );
    if (!out) return fail();

    if (!writeCount(out, wallets.size)) return fail();
    for (int i{}; i < wallets.size; i++) {
        if (!writeWallet(out, wallets[i])) return fail();
    }

    if (!writeCount(out, sources.size)) return fail();
    for (int i{}; i < sources.size; i++) {
        if (!writeIncomeSource(out, sources[i])) return fail();
    }

    if (!writeCount(out, categories.size)) return fail();
    for (int i{}; i < categories.size; i++) {
        if (!writeExpenseCategory(out, categories[i])) return fail();
    }

    if (!writeCount(out, transactions.size)) return fail();
    for (int i = 0; i < transactions.size; i++) {
        if (!writeTransaction(out, transactions[i])) return fail();
    }
    if (!writeCount(out, recurring.size)) return fail();
    for (int i = 0; i < recurring.size; i++) {
        if (!writeRecurring(out, recurring[i])) return fail();
    }

    out.flush();
    
    if (!out) return fail();

    out.close();

    if (!out) {
        std::error_code ec;
        fs::remove(tempPath, ec);
        return false;
    }

    std::error_code ec;

    fs::rename(tempPath, finalPath, ec);

    if (ec) {
        std::error_code cleanupError;
        fs::remove(tempPath, cleanupError);

        return false;
    }

    return true;
}

LoadStatus loadData(
    DynamicArray<Wallet>& wallets,
    DynamicArray<IncomeSource>& sources,
    DynamicArray<ExpenseCategory>& categories,
    DynamicArray<Transaction>& transactions,
    DynamicArray<RecurringTransaction>& recurring
) {
    const fs::path dataDir = "data";
    const fs::path filePath = dataDir / "finance.dat";

    std::error_code ec;

    bool dirExists = fs::exists(dataDir, ec);

    if (ec) return LoadStatus::OPEN_ERROR;
    if (!dirExists) {
        fs::create_directories(dataDir, ec);
        if (ec) return LoadStatus::OPEN_ERROR;
        return LoadStatus::NO_FILE;
    }

    bool isDir = fs::is_directory(dataDir, ec);
    if (ec || !isDir) return LoadStatus::OPEN_ERROR;

    bool fileExists = fs::exists(filePath, ec);
    if (ec) return LoadStatus::OPEN_ERROR;
    if (!fileExists) return LoadStatus::NO_FILE;

    bool isRegularFile = fs::is_regular_file(filePath, ec);
    if (ec || !isRegularFile) return LoadStatus::OPEN_ERROR;

    std::ifstream in(filePath, std::ios::binary);

    if (!in.is_open()) return LoadStatus::OPEN_ERROR;

    DynamicArray<Wallet> tempWallets;
    DynamicArray<IncomeSource> tempSources;
    DynamicArray<ExpenseCategory> tempCategories;
    DynamicArray<Transaction> tempTransactions;
    DynamicArray<RecurringTransaction> tempRecurring;

    auto fail = [&]() -> LoadStatus {
        return LoadStatus::INVALID_FILE;
    };

    const char expectedMagic[8] = {'P', 'F', 'M', 'D', 'A', 'T', 'A', '1'};
    char magic[8];
    in.read(magic, sizeof(magic));

    if (!in) return fail();
    for (int i{}; i < 8; ++i) if (magic[i] != expectedMagic[i]) return fail();

    std::uint32_t version{};

    in.read(
        reinterpret_cast<char*>(&version),
        sizeof(version)
    );

    if (!in || (version != 1 && version != 2)) return fail();

    std::uint32_t count{};

    if (!readCount(in, count)) return fail();
    for (std::uint32_t i{}; i < count; ++i) {
        Wallet w;
        if (!readWallet(in, w)) return fail();
        tempWallets.push_back(w);
    }

    if (!readCount(in, count)) return fail();
    for (std::uint32_t i{}; i < count; ++i) {
        IncomeSource source;
        if (!readIncomeSource(in, source)) return fail();
        tempSources.push_back(source);
    }

    if (!readCount(in, count)) return fail();
    for (std::uint32_t i{}; i < count; ++i) {
        ExpenseCategory category;
        if (!readExpenseCategory(in, category)) return fail();
        tempCategories.push_back(category);
    }

    // Version 1 files contain only wallets, sources, and categories.
    if (version == 2) {
        if (!readCount(in, count)) return fail();
        for (std::uint32_t i = 0; i < count; i++) {
            Transaction t;
            if (!readTransaction(in, t)) return fail();
            tempTransactions.push_back(t);
        }
        if (!readCount(in, count)) return fail();
        for (std::uint32_t i = 0; i < count; i++) {
            RecurringTransaction r;
            if (!readRecurring(in, r)) return fail();
            for (int j = 0; j < tempRecurring.size; j++) {
                if (tempRecurring[j].id == r.id) return fail();
            }
            tempRecurring.push_back(r);
        }
    }

    if (in.peek() != std::ifstream::traits_type::eof()) return fail();

    if (in.bad()) return fail();

    wallets.swap(tempWallets);
    sources.swap(tempSources);
    categories.swap(tempCategories);
    transactions.swap(tempTransactions);
    recurring.swap(tempRecurring);

    return LoadStatus::SUCCESS;
}