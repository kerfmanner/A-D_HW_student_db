#include <iostream>
#include <vector>

#include "student.hpp"
#include "utils.hpp"

int main() {
    std::vector<Student> students;

    students = read_students_from_csv("D:\\programming\\algos\\hmw_1\\studenttest.csv");

    std::cout << students.size() << std::endl;

    return 0;
}
