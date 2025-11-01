//
// Created by konov on 11/1/2025.
//

#ifndef HMW_1_DB1_HPP
#define HMW_1_DB1_HPP


#pragma once

#include "IStudent_db.hpp"
#include <unordered_map>

class StudentDB_Hash final : public Interface_StudentDB {
public:
    void load(const std::string& csv_path) override;

    std::vector<Student*> find_by_birth(int month, int day) override;
    bool change_group(const std::string& email, const std::string& new_group) override;
    std::string find_group_with_max_same_birthday() override;

private:
    // storage
    std::vector<Student> students;

    std::unordered_map<std::string, Student*> email_map;
    // month/day index → students
    // key = month * 100 + day
    std::unordered_map<int, std::vector<Student*>> birthday_map;

    // group → (birthday → count)
    std::unordered_map<std::string, std::unordered_map<int,int>> group_birth_count;
};
#endif //HMW_1_DB1_HPP