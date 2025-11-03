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
            std::string kb = line.substr(6);
            return std::stoul(kb);
        }
    }
    return 0;
}


// precomputed operation representation
enum class OpType { FIND_BIRTH, CHANGE_GROUP, FIND_MAX };

struct Op {
    OpType type;
    int student_idx;
};


int main(int argc, char** argv) {

    std::cout << "[0] Starting benchmark executable\n";

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

    std::cout << "[1] Parsing CLI arguments\n";

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

    std::cout << "[2] Creating DB instance: " << db_name << "\n";
    auto db = make_db(db_name);


    // memory mode
    if (mode == "memory") {

        std::cout << "[3] Memory mode selected\n";
        std::cout << "[4] Loading DB input\n";
        db->load(input_path);

        size_t input_size = 0;
        {
            std::cout << "[5] Loading CSV to measure input size\n";
            auto tmp = read_students_from_csv(input_path);
            input_size = tmp.size();
        }

        std::cout << "[6] Measuring peak RSS\n";
        size_t peak_kb = get_peak_rss_kb();
        // same ratio as before (100, 100, 1)
        std::cout << "[7] Memory stats: " << peak_kb << "\n";
        std::cout << "[8] Writing memory stats to CSV\n";
        append_csv("memory.csv", db_name, input_size, peak_kb);

        std::cout << "[9] Done (memory mode)\n";
        return 0;
    }



    // time mode
    if (mode == "time") {

        std::cout << "[3] Time mode selected\n";

        std::cout << "[4] Loading DB input\n";
        db->load(input_path);

        std::cout << "[5] Loading students from CSV\n";
        auto students = read_students_from_csv(input_path);
        size_t input_size = students.size();
        if (input_size == 0) {
            std::cerr << "No input data.\n";
            return 1;
        }

        constexpr int A = 100;
        constexpr int B = 100;
        constexpr int C = 1;
        constexpr int Sum = A + B + C;

        std::cout << "[6] Precomputing operations list\n";

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1, Sum);
        std::uniform_int_distribution<int> idx_dist(0, static_cast<int>(input_size) - 1);

        constexpr size_t MAX_PRE_OPS = 2000000ULL;
        std::vector<Op> ops;
        ops.reserve(MAX_PRE_OPS);

        for (size_t i = 0; i < MAX_PRE_OPS; ++i) {
            int x = dist(rng);
            Op op{};

            if (x <= A) {
                op.type = OpType::FIND_BIRTH;
                op.student_idx = idx_dist(rng);
            }
            else if (x <= A + B) {
                op.type = OpType::CHANGE_GROUP;
                op.student_idx = idx_dist(rng);
            }
            else {
                op.type = OpType::FIND_MAX;
                op.student_idx = -1;
            }

            ops.push_back(op);
        }

        std::cout << "[7] Starting timed execution (10s)\n";

        size_t op_count = 0;
        auto start = std::chrono::steady_clock::now();
        auto end_time = start + std::chrono::seconds(10);

        while (std::chrono::steady_clock::now() < end_time) {

            const Op& op = ops[op_count % ops.size()];

            switch (op.type) {
                case OpType::FIND_BIRTH: {
                    auto& s = students[op.student_idx];
                    db->find_by_birth(s.m_birth_month, s.m_birth_day);
                } break;

                case OpType::CHANGE_GROUP: {
                    auto& s = students[op.student_idx];
                    db->change_group(s.m_email, "NEW_GROUP");
                } break;

                case OpType::FIND_MAX:
                    db->find_group_with_max_same_birthday();
                    break;
            }

            ++op_count;
        }

        std::cout << "[8] Benchmark finished, ops = " << op_count << "\n";
        std::cout << "[9] Writing results to CSV\n";

        append_csv("ops.csv", db_name, input_size, op_count);

        std::cout << "[10] Done (time mode)\n";
        return 0;
    }


    std::cerr << "Unknown mode: " << mode << "\n";
    return 1;
}
