//
// Created by konov on 11/1/2025.
//

#ifndef HMW_1_DB2_HPP
#define HMW_1_DB2_HPP


#include "IStudent_db.hpp"
#include <map>
#include <vector>
#include <string>

class StudentDB_2 final : public Interface_StudentDB {
public:
    void load(const std::string& csv_path) override;

    std::vector<Student*> find_by_birth(int month, int day) override;
    bool change_group(const std::string& email, const std::string& new_group) override;
    std::string find_group_with_max_same_birthday() override;

private:
    // storage
    std::vector<Student> students;

    // email -> student
    std::map<std::string, Student*> email_map;
    // month/day index → students
    // key = month * 32 + day = month << 5 | day, because day < 32
    std::map<int, std::vector<Student*>> birthday_map;

    std::map<std::string, std::map<int,int>> group_birth_count;
};


#endif //HMW_1_DB2_HPP