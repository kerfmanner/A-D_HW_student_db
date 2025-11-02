//
// Created by konov on 11/1/2025.
//

#include "../include/sort_phone.hpp"

#include <algorithm>
#include <array>

static std::array<char,12> normalize_fixed(const std::string& phone) {
    std::array<char,12> out{};
    out.fill('0');

    size_t pos = 0;
    for (char c : phone) {
        if (std::isdigit((unsigned char)c)) {
            if (pos < 12)
                out[pos++] = c;
        }
    }
    return out;
}


void sort_by_phone_builtin(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [](const Student& lhs, const Student& rhs) {
        return lhs.m_phone_number < rhs.m_phone_number;
    });
}




static void radix_sort_indices(
    std::vector<size_t>& idx,
    const std::vector<std::array<char,12>>& keys)
{
    static constexpr int WIDTH = 12;
    static constexpr int BASE  = 10;

    std::vector<size_t> tmp(idx.size());

    for (int pos = WIDTH - 1; pos >= 0; --pos) {

        int count[BASE] = {0};

        for (const size_t id : idx) {
            const char d = keys[id][pos] - '0';
            count[static_cast<unsigned char>(d)]++;
        }

        for (int i = 1; i < BASE; ++i)
            count[i] += count[i - 1];

        for (int i = static_cast<int>(idx.size()) - 1; i >= 0; --i) {
            const size_t id = idx[i];
            const char d = keys[id][pos] - '0';
            tmp[--count[static_cast<unsigned char>(d)]] = id;
        }

        idx.swap(tmp);
    }
}


// radix sort interface
void sort_by_phone_radix(std::vector<Student>& students)
{
    size_t n = students.size();
    if (n == 0) return;

    // normalize
    std::vector<std::array<char,12>> keys;
    keys.reserve(n);
    for (auto& s : students) {
        keys.push_back(normalize_fixed(s.m_phone_number));
    }

    // index array
    std::vector<size_t> idx(n);
    for (size_t i = 0; i < n; ++i)
        idx[i] = i;

    // radix
    radix_sort_indices(idx, keys);

    std::vector<Student> sorted;
    sorted.reserve(n);
    for (size_t i : idx)
        sorted.push_back(students[i]);

    students.swap(sorted);
}