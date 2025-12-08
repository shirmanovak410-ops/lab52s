#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "lab4.h"
int main() {
    std::vector<Student> students;
    Student Anna = {
        "Anna",
        "SM11-1",
        {5, 5, 5, 3, 5},
        {"Math", "Russian", "Physics", "Biology", "PE"},
    };

    Student Ivan = {
        "Ivan",
        "IU8-12",
        {4, 4, 3, 5, 4},
        {"Math", "Russian", "Physics", "Biology", "PE"},
    };

    Student Polina = {
        "Polina",
        "RK2-15",
        {3, 5, 4, 2, 4},
        {"Math", "Russian", "Physics", "Biology", "PE"},
    };

    students = {Anna, Ivan, Polina};

    int task;
    std::cout << "select a task";
    std::cin >> task;

    switch (task) {
        case 1: {
            SortByName(students);
            std::cout << students[0].Name << " " << students[1].Name << " " << students[2].Name << std::endl;
            break;
        }
        case 2: {
            SortByRating(students);
            std::cout << students[0].Name << " " << students[1].Name << " " << students[2].Name << std::endl;
            break;
        }
        case 3: std::cout << CountTwoness(students) << std::endl; break;
        case 4: std::cout << CountExcellent(students) << std::endl; break;
        case 5: std::cout << VectorMathExcellent(students).size() << std::endl; break;
        case 6: for (int i = 0; i < GroupsId(students).size(); i++) std::cout << GroupsId(students)[i] << std::endl; break;
        case 7: {
            std::vector<Group> groups = Groups(students);
            for (int i = 0; i < groups.size(); i++)
                std::cout << groups[i].Id << " " << groups[i].Students.size() << std::endl;
            break;
        }
        default: std::cout << "Invalid task";
    }

    return 0;
}
float mean(const std::vector<unsigned>& Ratings) {
    float sum = 0;
    for (int i = 0; i < Ratings.size(); i++)
        sum += Ratings[i];
    return sum / (float)Ratings.size();
}

// задание 1
void SortByName(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.Name < b.Name;
        });
}
// задание 2
void SortByRating(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return mean(a.Ratings) < mean(b.Ratings);
        });
}
// задание 3
size_t CountTwoness(const std::vector<Student>& students) {
    size_t rez = 0;
    for (int n = 0; n < students.size(); n++) {
        bool two = false;
        for (int i = 0; i < students[n].Ratings.size(); i++) {
            if (students[n].Ratings[i] == 2) {
                two = true;
                break;
            }
        }
        if (two) rez += 1;
    }
    return rez;
}

// задание 4
size_t CountExcellent(const std::vector<Student>& students) {
    size_t rez = 0;
    for (int n = 0; n < students.size(); n++) {
        bool excellent = true;
        for (int i = 0; i < students[n].Ratings.size(); i++) {
            if (students[n].Ratings[i] != 5) {
                excellent = false;
                break;
            }
        }
        if (excellent) rez += 1;
    }
    return rez;
}

// задание 5
std::vector<Student> VectorMathExcellent(const std::vector<Student>& students) {
    std::vector<Student> rez;
    for (int i = 0; i < students.size(); i++) {
        auto id = std::find(students[i].Subjects.begin(), students[i].Subjects.end(), "Math");
        if (id != students[i].Subjects.end() && students[i].Ratings[std::distance(students[i].Subjects.begin(), id)] == 5)
            rez.push_back(students[i]);
    }
    return rez;
}

// задание 6
std::vector<std::string> GroupsId(const std::vector<Student>& students) {
    std::vector<std::string> groups;
    for (int i = 0; i < students.size(); i++) {
        if (std::find(groups.begin(), groups.end(), students[i].GroupId) == groups.end())
            groups.push_back(students[i].GroupId);
    }
    return groups;
}

// задание 7
std::vector<Group> Groups(const std::vector<Student>& students) {
    std::vector<Group> GROUPS;
    std::vector<std::string> groups = GroupsId(students);
    for (int i = 0; i < groups.size(); i++) {
        std::vector<Student> group_members;
        for (int j = 0; j < students.size(); j++) {
            if (students[j].GroupId == groups[i]) group_members.push_back(students[j]);
        }
        Group group = {
            groups[i],
            group_members
        };
        GROUPS.push_back(group);
    }
    return GROUPS;
}