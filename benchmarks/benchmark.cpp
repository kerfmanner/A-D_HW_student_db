#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <random>
#include <fstream>

#include "IStudent_db.hpp"
#include "db1.hpp"
#include "db1_sp.hpp"
#include "db2.hpp"
#include "db3.hpp"
#include "db_1_lm.hpp"
#include "db_2_lm.hpp"
#include "dp2_sp.hpp"
#include "utils.hpp"


// construct db
static std::unique_ptr<Interface_StudentDB> make_db(const std::string& name) {
    if (name == "1")    return std::make_unique<StudentDB_1>();
    if (name == "1L")   return std::make_unique<StudentDB_1LessMemory>();
    if (name == "1S")   return std::make_unique<StudentDB_1Speed>();
    if (name == "2")    return std::make_unique<StudentDB_2>();
    if (name == "2L")   return std::make_unique<StudentDB_2LessMemory>();
    if (name == "2S")   return std::make_unique<StudentDB_2Speed>();
    if (name == "3")    return std::make_unique<StudentDB_3>();

    throw std::invalid_argument("Unknown DB type: " + name);
}


// CSV writer
static void append_csv(const std::string& filename,
                       const std::string& db,
                       size_t input_size,
                       size_t value)
{
    std::ofstream f(filename, std::ios::app);
    if (!f.is_open()) {
        std::cerr << "Could not open log file: " << filename << "\n";
        return;
    }
    f << db << "," << input_size << "," << value << "\n";
}



// peak RSS from /proc/self/status
static size_t get_peak_rss_kb()
{
    std::ifstream f("/proc/self/status");
    if (!f.is_open()) return 0;

    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("VmHWM:", 0) == 0) {
            // format:  "VmHWM: <space> #### kB"
            std::string kb = line.substr(6);
            return std::stoul(kb);
        }
    }
    return 0;
}


int main(int argc, char** argv) {

    if (argc < 4) {
        std::cerr << "Usage:\n"
                  << "  ./benchmark --db=<type> --input=<file> --mode=time\n"
                  << "  ./benchmark --db=<type> --input=<file> --mode=memory\n"
                  << "\nDB types: 1, 1L, 1S, 2, 2L, 2S, 3\n";
        return 1;
    }

    std::string db_name;
    std::string input_path;
    std::string mode;

    // parse args
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a.rfind("--db=", 0) == 0) {
            db_name = a.substr(5);
        } else if (a.rfind("--input=", 0) == 0) {
            input_path = a.substr(8);
        } else if (a.rfind("--mode=", 0) == 0) {
            mode = std::string(a.substr(7));
        }
    }

    if (db_name.empty() || input_path.empty() || mode.empty()) {
        std::cerr << "Missing parameters\n";
        return 1;
    }

    auto db = make_db(db_name);


    // mode memory
    if (mode == "memory") {

        // load DB
        db->load(input_path);

        size_t input_size = 0;
        {
            std::vector<Student> tmp = read_students_from_csv(input_path);
            input_size = tmp.size();
        }

        // get memory statistics
        size_t peak_kb = get_peak_rss_kb();

        // write record
        append_csv("memory.csv", db_name, input_size, peak_kb);

        return 0;
    }



    // mode time
    if (mode == "time") {

        db->load(input_path);

        // Load a local student list only for sampling operations
        auto students = read_students_from_csv(input_path);
        size_t input_size = students.size();

        // ratio A:B:C = 100:100:1
        const int A = 100;
        const int B = 100;
        const int C = 1;
        const int Sum = A + B + C;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1, Sum);
        std::uniform_int_distribution<int> idx_dist(0, (int)students.size() - 1);

        size_t ops = 0;
        auto start = std::chrono::steady_clock::now();
        auto end_time = start + std::chrono::seconds(10);

        while (std::chrono::steady_clock::now() < end_time) {

            int x = dist(rng);

            if (x <= A) {
                auto& s = students[idx_dist(rng)];
                db->find_by_birth(s.m_birth_month, s.m_birth_day);
            }
            else if (x <= A + B) {
                auto& s = students[idx_dist(rng)];
                db->change_group(s.m_email, "NEW_GROUP");
            }
            else {
                db->find_group_with_max_same_birthday();
            }

            ++ops;
        }

        append_csv("ops.csv", db_name, input_size, ops);

        return 0;
    }



    // Unknown mode
    std::cerr << "Unknown mode: " << mode << "\n";
    return 1;
}