//
// Created by konov on 11/1/2025.
//

#include "../include/db2.hpp"
#include <ranges>
#include "utils.hpp"

void StudentDB_2::load(const std::string &csv_path) {
    students = read_students_from_csv(csv_path);

    for (auto& student : students) {

        int key = birthday_key(student.m_birth_month, student.m_birth_day);
        birthday_map[key].push_back(&student);

        email_map[student.m_email] = &student;

        group_birth_count[student.m_group][key]++;
    }
}


std::vector<Student *> StudentDB_2::find_by_birth(const int month, const int day) {
    const int key = birthday_key(month, day);
    if (!birthday_map.contains(key)) return {};
    return birthday_map[key];
};


bool StudentDB_2::change_group(const std::string &email, const std::string &new_group) {
    if (!email_map.contains(email)) return false;
    Student *student = email_map[email];

    if (student->m_group == new_group) {
        return true;
    }

    const int key = birthday_key(student -> m_birth_month, student->m_birth_day);

    group_birth_count[student->m_group][key]--;
    student->m_group = new_group;
    group_birth_count[student->m_group][key]++;
    return true;
}

std::string StudentDB_2::find_group_with_max_same_birthday() {
    int max_group = -1;
    std::string max_group_str;
    for (auto&[group, brd_cnt]: group_birth_count) {
        for (const auto &cnt: brd_cnt | std::views::values) {
            if (cnt  > max_group) {
                max_group = cnt ;
                max_group_str = group;
            }
        }
    }

    return max_group_str;
}
