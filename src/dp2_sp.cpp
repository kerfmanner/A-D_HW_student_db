//
// Created by luka on 11/2/25.
//

#include "../include/dp2_sp.hpp"
#include <ranges>
#include "utils.hpp"



void StudentDB_2Speed::load(const std::string &csv_path) {
    students = read_students_from_csv(csv_path);

    for (auto& student : students) {

        int key = birthday_key(student.m_birth_month, student.m_birth_day);
        birthday_map[key].push_back(&student);

        email_map[student.m_email] = &student;

        group_birth_count[student.m_group][key]++;
    }
}


std::vector<Student *> StudentDB_2Speed::find_by_birth(const int month, const int day) {
    const int key = birthday_key(month, day);
    if (!birthday_map.contains(key)) return {};
    return birthday_map[key];
};


bool StudentDB_2Speed::change_group(const std::string &email, const std::string &new_group) {
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

std::string StudentDB_2Speed::find_group_with_max_same_birthday() {
    int best_count = -1;
    std::string best_group;

    for (const auto& [group, arr] : group_birth_count) {
        // scan 416 birthday counters
        // we can start from 33 because it is the minimal possible key
        for (int i = 33; i < 416; ++i) {
            if (const int c = arr[i]; c > best_count) {
                best_count = c;
                best_group = group;
            }
        }
    }
    return best_group;
}
