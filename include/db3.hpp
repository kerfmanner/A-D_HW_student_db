//
// Created by konov on 11/1/2025.
//

#ifndef HMW_1_DB3_HPP
#define HMW_1_DB3_HPP

#include "IStudent_db.hpp"
#include <vector>
#include <string>

class StudentDB_Sorted : public Interface_StudentDB {
public:
    void load(const std::string& csv_path) override;

    std::vector<Student*> find_by_birth(int month, int day) override;
    bool change_group(const std::string& email, const std::string& new_group) override;
    std::string find_group_with_max_same_birthday() override;

private:
    // Primary storage
    std::vector<Student> students;

    // Sorted views / indexes
    std::vector<Student*> by_email;    // sorted by email
    std::vector<Student*> by_birth;    // sorted by (month, day)

    // Optional: group birthday array or recompute counts
    // This method is intentionally slow — recomputes when needed
};

#endif //HMW_1_DB3_HPP