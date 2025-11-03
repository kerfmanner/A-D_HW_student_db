#ifndef HMW_1_UTILS_HPP
#define HMW_1_UTILS_HPP
#include <sstream>
#include <string>
#include <vector>
#include "student.hpp"
std::vector<std::string> split_line(std::istringstream &str, const std::string &delim);
Student get_student_from_line(std::istringstream &str);
std::vector<Student> read_students_from_csv(const std::string& path);
void write_students_to_csv(const std::vector<Student> &students, const std::string& path);
int birthday_key(int month, int day);

#endif //HMW_1_UTILS_HPP