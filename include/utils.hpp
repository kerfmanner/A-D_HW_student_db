//
// Created by konov on 11/1/2025.
//

#ifndef HMW_1_UTILS_HPP
#define HMW_1_UTILS_HPP
#include <sstream>
#include <string>
#include <vector>
#include "student.hpp"
std::vector<std::string> split_line(std::istringstream &str, const std::string &delim);
Student get_student_from_line(std::istringstream &str);
std::vector<Student> read_students_from_csv(const std::string& path);

#endif //HMW_1_UTILS_HPP