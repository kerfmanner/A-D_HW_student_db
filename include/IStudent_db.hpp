#ifndef HMW_1_INTERFACE_STUDENT_DB_HPP
#define HMW_1_INTERFACE_STUDENT_DB_HPP
#include <string>
#include <vector>
#include "student.hpp"

class Interface_StudentDB {
public:
    virtual ~Interface_StudentDB() = default;

    // Load DB from CSV
    virtual void load(const std::string& csv_path) = 0;

    // Variant V2 : operations
    virtual std::vector<Student*> find_by_birth(int month, int day) = 0;
    virtual bool change_group(const std::string& email, const std::string& new_group) = 0;
    virtual std::string find_group_with_max_same_birthday() = 0;
};
#endif //HMW_1_INTERFACE_STUDENT_DB_HPP