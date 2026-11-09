//Лабораторна робота 6 варіант 2
//Харченко Поліна К-28
#if defined(_WIN32)
#include <windows.h>
#endif
#include <iostream>
#include <charconv>
#include <exception>
#include <format>
#include <stdexcept>
#include <string_view>
#include "generator.h"

using namespace std;

const int DEFAULT_COUNT = 20;

int parseCount(int argc, char *argv[])
{
    if (argc < 2)
        return DEFAULT_COUNT;
    if (argc > 2)
        throw invalid_argument("Too many arguments. Usage: coroutines [count]");
    string_view text(argv[1]);
    int count = 0;
    auto [end, error] = from_chars(text.data(), text.data() + text.size(), count);
    if (error != errc() or end != text.data() + text.size() or count <= 0)
        throw invalid_argument("Count must be a positive integer");
    return count;
}

int main(int argc, char *argv[])
{
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
#endif
    try
    {
        Task task = generate(parseCount(argc, argv));
        int suspensions = 0;
        while (!task.done())
        {
            task.resume();
            if (!task.done())
            {
                ++suspensions;
                cout << "  Сопрограму призупинено (різниця менша за 16), відновлюємо\n";
            }
        }
        cout << format("Сопрограму завершено. Кількість призупинень: {}\n", suspensions);
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
