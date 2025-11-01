//
// Created by konov on 11/1/2025.
//

#ifndef HMW_1_MYSORT_HPP
#define HMW_1_MYSORT_HPP

#include <vector>
#include "student.hpp"

std::string normalize_phone(const std::string& phone);

void sort_by_phone_builtin(std::vector<Student>& students);

void sort_by_phone_radix(std::vector<Student>& students);


#endif //HMW_1_MYSORT_HPP