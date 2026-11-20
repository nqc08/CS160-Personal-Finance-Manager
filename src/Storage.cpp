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
    if (count < 0 || count > MAX_ITEMS) return false;

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

bool saveData(
    const DynamicArray<Wallet>& wallets,
    const DynamicArray<IncomeSource>& sources,
    const DynamicArray<ExpenseCategory>& categories
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
    const std::uint32_t version = 1;
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
    DynamicArray<ExpenseCategory>& categories
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

    if (!in || version != 1) return fail();

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

    if (in.peek() != std::ifstream::traits_type::eof()) return fail();

    if (in.bad()) return fail();

    wallets.swap(tempWallets);
    sources.swap(tempSources);
    categories.swap(tempCategories);

    return LoadStatus::SUCCESS;
}