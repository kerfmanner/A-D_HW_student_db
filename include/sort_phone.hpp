#ifndef HMW_1_MYSORT_HPP
#define HMW_1_MYSORT_HPP

#include <vector>
#include "student.hpp"

struct NormalPhoneEntries {
    Student m_student;
    std::string normal_phone;
};


void sort_by_phone_builtin(std::vector<Student>& students);

void radix_sort_phone_entries(std::vector<NormalPhoneEntries>& entries);
void sort_by_phone_radix(std::vector<Student>& students);


#endif //HMW_1_MYSORT_HPP