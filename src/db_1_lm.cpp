//
// Created by konov on 11/1/2025.
//

#include "../include/db_1_lm.hpp"
#include <ranges>
#include "utils.hpp"


void StudentDB_1LessMemory::load(const std::string &csv_path) {
    students = read_students_from_csv(csv_path);

    email_map.reserve(students.size());

    for (auto& student : students) {
        int key = birthday_key(student.m_birth_month, student.m_birth_day);
        birthday_map[key].push_back(&student);
        email_map[student.m_email] = &student;
        // Now lets store an array of fixed size under each group in the ordered_map
        // we already have keys = month * 32 + birth_day
        // and the minimum key is 32 * 1 + 1 = 33
        // the maximum key = 32*12 + 31 = 415
        // so the total size need to store keys is 416
        auto& vec = group_birth_count[student.m_group];
        if (vec.empty()) {
            vec.resize(MAX_BDAY, 0);
        }
        vec[key]++;
    }
}


std::vector<Student *> StudentDB_1LessMemory::find_by_birth(const int month, const int day) {
    const int key = birthday_key(month, day);
    if (!birthday_map.contains(key)) return {};
    return birthday_map[key];
};


bool StudentDB_1LessMemory::change_group(const std::string &email, const std::string &new_group) {
    if (!email_map.contains(email)) return false;
    Student *student = email_map[email];

    if (student->m_group == new_group) {
        return true;
    }

    const int key = birthday_key(student -> m_birth_month, student->m_birth_day);


    auto& oldv = group_birth_count[student -> m_group];
    oldv[key]--;

    auto& newv = group_birth_count[new_group];
    if (newv.empty()) newv.resize(MAX_BDAY, 0);
    newv[key]++;

    return true;
}

std::string StudentDB_1LessMemory::find_group_with_max_same_birthday() {
    std::string best;
    int mx = -1;

    for (auto& [group, counts] : group_birth_count) {
        for (auto cnt : counts) {
            if (cnt > mx) {
                mx = cnt;
                best = group;
            }
        }
    }
    return best;
}
