# Homework Project №1: Sorting and Data Structures

-------------------

## Project Setting
#### Author: Luka Konovalov

#### Variant:
-   V2;
-   A:B:C = 100:100:1
-   S7

#### Project structure:
<details>
<summary>Click here</summary>

```text
├── benchmarks
│   ├── benchmark.cpp
│   └── sorting_benchmark.cpp
├── benchmark_test.sh
├── CMakeLists.txt
├── create_smaller_csv.py
├── dataset
│   ├── 100000.csv
│   ├── 10000.csv
│   ├── 1000.csv
│   ├── 100.csv
│   └── 10_test.csv
├── include
│   ├── db1.hpp
│   ├── db_1_lm.hpp
│   ├── db1_sp.hpp
│   ├── db2.hpp
│   ├── db_2_lm.hpp
│   ├── db3.hpp
│   ├── dp2_sp.hpp
│   ├── IStudent_db.hpp
│   ├── sort_phone.hpp
│   ├── student.hpp
│   └── utils.hpp
├── README.md
├── results
│   ├── memory.csv
│   ├── ops.csv
│   ├── plots.ipynb
│   ├── sort.csv
│   ├── sorted_10_students.csv
│   └── unsorted_10_students.csv
├── sort_bench.sh
├── sort.csv
└── src
    ├── db1.cpp
    ├── db_1_lm.cpp
    ├── db1_sp.cpp
    ├── db2.cpp
    ├── db_2_lm.cpp
    ├── db3.cpp
    ├── dp2_sp.cpp
    ├── main.cpp
    ├── sort_phone.cpp
    ├── student.cpp
```
</details> 
----------------------

### DBs data structures

#### Database Implementations Overview

All database backends implement inteface:

```cpp
class Interface_StudentDB {
public:
    virtual ~Interface_StudentDB() = default;
    virtual void load(const std::string& csv_path) = 0;
    virtual std::vector<Student*> find_by_birth(int month, int day) = 0;
    virtual bool change_group(const std::string& email,
                              const std::string& new_group) = 0;
    virtual std::string find_group_with_max_same_birthday() = 0;
};
```

### Summary of Implementations

| DB | Email lookup | Birthday lookup | Stats form | Speed     | Memory    |
|----|--------------|-----------------|------------|-----------|-----------|
| DB1 | hash | hash | nested hash | very good | good      |
| DB1Speed | hash | hash | array | best      | weakest   |
| DB1LessMemory | hash | hash | vec<uint16_t> | good      | good      |
| DB2 | tree | tree | nested tree | good      | good      |
| DB2Speed | tree | tree | array | very good | weakest   |
| DB2LessMemory | tree | tree | vec<uint16_t> | trade-off | very good |
| DB3 | sorted | sorted | none | weakest   | best      |

### Storage Structures

| DB | Student Storage | Email Index | Birthday Index | Group Stats |
|----|-----------------|-------------|----------------|-------------|
| DB1 | vector | unordered_map | unordered_map | unordered_map<str, unordered_map<int,int>> |
| DB1Speed | vector | unordered_map | unordered_map | unordered_map<str, array<int,416>> |
| DB1LessMemory | vector | unordered_map | unordered_map | unordered_map<str, vector<uint16_t>> |
| DB2 | vector | map | map | map<str, map<int,int>> |
| DB2Speed | vector | map | map | map<str, array<int,416>> |
| DB2LessMemory | vector | map | map | map<str, vector<uint16_t>> |
| DB3 | vector | sorted vector* | sorted vector* | computed |

### Results
Result are in the result directory, graphs are in the jupyter notebook(**PS they use logarithmic scale).
