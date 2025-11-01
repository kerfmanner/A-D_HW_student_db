//
// Created by konov on 11/1/2025.
//

#include "../include/db3.hpp"
#include "utils.hpp"
#include <algorithm>
#include <unordered_map>
#include <ranges>


void StudentDB_3::load(const std::string& csv_path) {
    students = read_students_from_csv(csv_path);

    by_email.clear();
    by_birth.clear();

    by_email.reserve(students.size());
    by_birth.reserve(students.size());

    for (auto& s : students) {
        by_email.push_back(&s);
        by_birth.push_back(&s);
    }

    // sort by email
    std::sort(by_email.begin(), by_email.end(),
              [](const Student* a, const Student* b) {
                  return a->m_email < b->m_email;
              });

    // sort by birthday key
    std::sort(by_birth.begin(), by_birth.end(),
              [](const Student* a, const Student* b) {
                  const int ka = birthday_key(a->m_birth_month, a->m_birth_day);
                  const int kb = birthday_key(b->m_birth_month, b->m_birth_day);
                  return ka < kb;
              });
}


std::vector<Student*> StudentDB_3::find_by_birth(const int month, const int day) {
    const int key = birthday_key(month, day);

    auto it = std::lower_bound(
        by_birth.begin(), by_birth.end(), key,
        [](const Student* s, const int k) {
            return birthday_key(s->m_birth_month, s->m_birth_day) < k;
        }
    );

    std::vector<Student*> result;
    for (; it != by_birth.end(); ++it) {
        if (birthday_key((*it)->m_birth_month, (*it)->m_birth_day) != key)
            break;
        result.push_back(*it);
    }
    return result;
}


bool StudentDB_3::change_group(const std::string& email,
                               const std::string& new_group)
{
    const auto it = std::lower_bound(by_email.begin(), by_email.end(), email,
        [](const Student* s, const std::string& e) {
            return s->m_email < e;
        });

    if (it == by_email.end()) return false;
    if ((*it)->m_email != email) return false;

    Student* s = *it;
    s->m_group = new_group;
    return true;
}


std::string StudentDB_3::find_group_with_max_same_birthday() {
    // temporary map
    std::unordered_map<std::string, std::unordered_map<int,int>> temp;

    int best_cnt = 0;
    std::string best_group;

    for (const auto& s : students) {
        int key = birthday_key(s.m_birth_month, s.m_birth_day);
        if (const int val = ++temp[s.m_group][key]; val > best_cnt) {
            best_cnt = val;
            best_group = s.m_group;
        }
    }

    return best_group;
}
