#pragma once
#include <string>
#include <vector>

struct Course {
    std::string name;
    int mark;
};

class ITStudent {
public:
    std::string name;
    std::string id;
    std::string programme;
    std::vector<Course> courses;

    double calculateAverage() {
        if (courses.empty()) return 0.0;
        int total = 0;
        for (auto &c : courses) total += c.mark;
        return static_cast<double>(total) / courses.size();
    }

    std::string passOrFail() {
        return calculateAverage() >= 50 ? "PASS" : "FAIL";
    }
};