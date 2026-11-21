"""Build and run checks in temporary folders, preserving real application data."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
common = [str(root / 'src' / name) for name in (
    'RecurringTransaction.cpp', 'Transaction.cpp', 'Wallet.cpp',
    'Income.cpp', 'Expense.cpp', 'display.cpp', 'Storage.cpp')]

with tempfile.TemporaryDirectory(prefix='pfm-recurring-') as temporary:
    folder = Path(temporary)
    flags = ['g++', '-std=c++17', '-Wall', '-Wextra', '-I' + str(root / 'include')]
    tests = folder / 'tests'
    app = folder / 'app'
    subprocess.run(flags + [str(root / 'tests/recurring_tests.cpp')] + common + ['-o', str(tests)], check=True)
    # loadData creates the data directory, just as it does during application startup.
    (folder / 'data').mkdir()
    subprocess.run([str(tests)], cwd=folder, check=True)
    subprocess.run(flags + [str(root / 'src/main.cpp')] + common + ['-o', str(app)], check=True)

    cli = folder / 'cli'
    cli.mkdir()
    def run_cli(commands):
        result = subprocess.run([str(app)], cwd=cli, input=commands, text=True,
                                capture_output=True, timeout=10, check=True)
        return result.stdout

    # Create master records, then a recurring income eligible immediately.
    output = run_cli('1\n1\nW1\nCash\n1000\n\n0\n'
                     '2\n1\nS1\nSalary\n\n0\n'
                     '5\n1\nR1\n1\nCash\nSalary\n100\nMonthly pay\n1 1 2020\n0\n\n0\n')
    assert 'Recurring transaction added.' in output
    assert 'Created 1 recurring transaction(s).' in output
    assert 'Total Balance: 1100' in output

    # Reopening must preserve history and must not generate again this month.
    output = run_cli('4\n2\n\n0\n0\n')
    assert 'Data loaded successfully!' in output
    assert 'Created 1 recurring transaction(s).' not in output
    assert output.count('Monthly pay') == 1
    assert 'Total Balance: 1100' in output

    # Viewing the configuration shows the saved monthly marker and wallet/source names.
    output = run_cli('5\n2\n\n0\n')
    assert 'R1 | Income | 100 | Cash | Salary | Monthly pay' in output
    assert 'No end date' in output

    # A normal transaction also survives restart through the shared storage.
    output = run_cli('4\n1\n1\n21 11 2026\nSalary\nCash\n25\nNormal income\n\n0\n0\n')
    assert 'CREATE A NEW TRANSACTION: SUCCESS' in output
    output = run_cli('4\n2\n\n0\n0\n')
    assert output.count('Normal income') == 1
    assert output.count('Monthly pay') == 1
    assert 'Total Balance: 1125' in output
    print('CLI creation, restart, history, and duplicate-prevention checks passed.')
