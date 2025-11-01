//
// Created by konov on 11/1/2025.
//

#include "../include/db1.hpp"

#include <ranges>
#include "utils.hpp"

void StudentDB_1::load(const std::string &csv_path) {
    students = read_students_from_csv(csv_path);

    email_map.reserve(students.size());

    for (auto& student : students) {
        // subtask 1
        // Знайти всіх студентів за заданим днем народження (m_birth_month, m_birth_day)
        // m_birth_mouth = 1..12
        // m_birth_day = 1..DAYS DAYS_maximum = 31,
        // 12 * 31 = 372  is an estimation of total number of birthday
        // need to uniquely determine the key for hashmap based on month, day
        // if the maximum of days in a month is 31 than:
        // key = month * 31 + day
        // will be enough, by multiplying by 31 we make space for 31 possible days
        // but better to align it by the degree of 2 so:
        // key = month * 32 + day ~ month << 5 | day, because day < 32
        int key = birthday_key(student.m_birth_month, student.m_birth_day);
        birthday_map[key].push_back(&student);
        // subtask 2
        // Змінити групу студенту за його електронною поштою (m_email)
        // emails are all unique therefore perfect to be used as keys
        email_map[student.m_email] = &student;
        // subtask 3
        // Знайти групу, в якій найбільша кількість студентів мають день народження в
        // один день.
        // for this option let's choose very optimized
        // but memory heavy hash_map of groups of hash_maps of [birthdays]
        group_birth_count[student.m_group][key]++;
    }
}


std::vector<Student *> StudentDB_1::find_by_birth(const int month, const int day) {
    const int key = birthday_key(month, day);
    if (!birthday_map.contains(key)) return {};
    return birthday_map[key];
};


bool StudentDB_1::change_group(const std::string &email, const std::string &new_group) {
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

std::string StudentDB_1::find_group_with_max_same_birthday() {
    int max_group = -1;
    std::string max_group_str;
    for (auto&[fst, snd]: group_birth_count) {
        for (const auto &val: snd | std::views::values) {
            if (val  > max_group) {
                max_group = val ;
                max_group_str = fst;
            }
        }
    }

    return max_group_str;
}

