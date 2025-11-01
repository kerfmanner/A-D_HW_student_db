//
// Created by konov on 11/1/2025.
//

#include "../include/utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::string> split_line(std::istringstream &str, const char
    &delim) {
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(str, token, delim)) {
        tokens.push_back(token);
    }

    return tokens;
}

Student get_student_from_line(std::string &line) {
    std::istringstream line_stream(line);
    std::vector<std::string> tokens = split_line(line_stream, ',');

    if (tokens.size() != 9) {
        std::cerr << "Wrong number of tokens " <<  tokens.size() << std::endl;
        throw std::invalid_argument("Wrong number of tokens in the csv");
    }

    std::string m_name = tokens[0];
    std::string m_surname = tokens[1];
    std::string m_email = tokens[2];
    int m_birth_year = std::stoi(tokens[3]);
    int m_birth_month = std::stoi(tokens[4]);
    int m_birth_day = std::stoi(tokens[5]);
    std::string m_group = tokens[6];
    float m_rating = std::stof(tokens[7]);
    std::string m_phone_number = tokens[8];

    Student student{m_name, m_surname, m_email, m_birth_year,
            m_birth_month, m_birth_day, m_group, m_rating, m_phone_number};

    return student;

}

std::vector<Student> read_students_from_csv(const std::string &path) {
    std::vector<Student> students;
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::invalid_argument("File not found");
    }

    std::string line;
    // skip first line with column names
    std::getline(file, line);

    while (std::getline(file, line)) {
        students.push_back(get_student_from_line(line));
    }

    return students;
}
