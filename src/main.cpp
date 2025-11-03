#include <iostream>
#include <vector>

#include "student.hpp"
#include "utils.hpp"
#include "db1.hpp"
#include "sort_phone.hpp"

int main() {
    std::vector<Student> students = read_students_from_csv("../dataset/10_test.csv");
    StudentDB_1 db;
    db.load("../dataset/10_test.csv");
    // exists
    for (auto st : db.find_by_birth(8, 13)) {
        std::cout << "month: " << st->m_birth_month << " day: " << st->m_birth_day<<  std::endl;
    };
    // doesn't exist

    if (db.find_by_birth(8, 12).empty()) {
        std::cout << "No student with birthday month 8 day 12\n";
    }
    std::cout << students[1].m_group + "\n";

    // email exists
    db.change_group(students[1].m_email, "VET-47");
    std::cout << students[1].m_group + "\n";

    //email doesn't exist
    db.change_group("big_black_plane@dg.net", "VET-47");
    // nothing

    std::cout << db.find_group_with_max_same_birthday() + "\n";

    // move person from XBO-96 some else
    db.change_group("andriy.kovalchuk@student.org", "VET-47");

    std::cout << db.find_group_with_max_same_birthday() + "\n";

    sort_by_phone_radix(students);

    write_students_to_csv(students, "../results/sorted_10_students.csv");
}
