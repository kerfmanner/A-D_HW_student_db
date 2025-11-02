#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>

#include "sort_phone.hpp"
#include "utils.hpp"

static void append_csv(const std::string& filename,
                       const std::string& method,
                       size_t input_size,
                       size_t ms)
{
    std::ofstream f(filename, std::ios::app);
    if (!f.is_open()) {
        std::cerr << "Could not open log file: " << filename << "\n";
        return;
    }
    f << method << "," << input_size << "," << ms << "\n";
}


int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Usage:\n"
                  << "  ./sort_bench --input=<file> --method=builtin\n"
                  << "  ./sort_bench --input=<file> --method=radix\n";
        return 1;
    }

    std::string method;
    std::string input_path;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a.rfind("--method=", 0) == 0) {
            method = a.substr(9);
        } else if (a.rfind("--input=", 0) == 0) {
            input_path = a.substr(8);
        }
    }

    if (method.empty() || input_path.empty()) {
        std::cerr << "Missing parameters\n";
        return 1;
    }

    auto students = read_students_from_csv(input_path);
    size_t input_size = students.size();

    auto start = std::chrono::steady_clock::now();

    if (method == "builtin") {
        sort_by_phone_builtin(students);
    }
    else if (method == "radix") {
        sort_by_phone_radix(students);
    }
    else {
        std::cerr << "Unknown method: " << method << "\n";
        return 1;
    }

    auto end = std::chrono::steady_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    append_csv("sort.csv", method, input_size, ms);

    std::cout << "method=" << method
              << ", input=" << input_size
              << ", time_ms=" << ms << "\n";

    return 0;
}
