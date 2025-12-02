#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <mutex>
#include <random>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <filesystem>
#include "Buffer.h"

class ITstudent {
    public:
    std::string name;
    std::string studentID;
    std::string programme;
    std::map<std::string, int>courses;

    ITstudent() = default;
    ITstudent(const std::string& n, const std::string& id,const std::string& prog, const std::map<std::string,int>& c)
       : name(n), studentID(id), programme(prog), courses(c) {}

    double average() const {
        if (courses.empty()) return 0.0;
        int sum = 0;
        for (auto& kv : courses ) sum += kv.second;
        return static_cast<double>(sum) /courses.size();
    }

    bool passed() const { return average() >= 50.0; }
};

/*class Buffer {
    private:
        std::queue<int> q;
        const size_t MAX_SIZE = 10;
        std::mutex mtx;
        std::condition_variable notFull;
        std::condition_variable notEmpty;

    public:
        void put(int item) {
            std::unique_lock<std::mutex> lock(mtx);
            notFull.wait(lock, [this]{return q.size() < MAX_SIZE; });
            q.push(item);
            std::cout << "[Buffer] put " << item << " (size=" << q.size() << ")\n";
            notEmpty.notify_one();
        }

        int take() {
            std::unique_lock<std::mutex> lock(mtx);
            notEmpty.wait(lock, [this]{return !q.empty(); });
            int item = q.front(); q.pop();
            std::cout << "[Buffer] take " << item << " (size=" << q.size() << ")\n";
            notFull.notify_one();
            return item;
        }
};*/

std::string randomID8(std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 9);
    std::string id(8, '0');
    for (int i = 0; i < 8; ++i) id[i] = char('0' + dist(rng));
    return id;
}

ITstudent makeRandomStudent(int i, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0,100);
    std::map<std::string,int> courses = {
        {"Programming", dist(rng)},
        {"Databases", dist(rng)},
        {"Networks", dist(rng)},
        {"Algorithms", dist(rng)},
        {"OperatingSystems", dist(rng)}
    };
    return ITstudent("Student" + std::to_string(i),randomID8(rng), "BSc Computer Science", courses);
}

void writeXML(const ITstudent& s, const std::string& path) {
    std::ofstream out(path);
    out << "<ITstudent>\n";
    out << " <Name>" << s.name << "</Name>\n";
    out << " <ID>" << s.studentID << "</ID>\n";
    out << " <Programme>" << s.programme << "</Programme>\n";
    out << " <Courses>\n";
    for (auto& kv : s.courses) {
        out << "     <Course name=\"" << kv.first << "\">" << kv.second << "</Course>\n";
    }
    out << " </Courses>\n";
    out << "</ITstudent>\n";
}

void runProducer(Buffer& buffer, const std::string& sharedDir,int count) {
    std::filesystem::create_directories(sharedDir);
    std::mt19937 rng(std::random_device{}());
    for (int i = 0; 1 <= count; ++i) {
        ITstudent s = makeRandomStudent(i, rng);
        std::string filename = sharedDir + "/student" + std::to_string(i) + ".xml";
        writeXML(s, filename);
        buffer.put(i);
        std::cout << "[Producer] wrote " << filename << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

ITstudent parseXML(const std::string& path) {
    std::ifstream in(path);
    std::string xml((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());

    auto extract = [](const std::string src, const std::string& tag) -> std::string {
        auto a = src.find("<" + tag + ">");
        auto b = src.find("</" + tag + ">");
        if (a == std::string::npos || b == std::string::npos) return"";
        a += tag.size() + 2;
        return src.substr(a, b - a);
    };

    std::string name = extract(xml, "Name");
    std::string id = extract(xml, "ID");
    std::string programme = extract(xml, "Programme");

    std::map<std::string,int> courses;
    size_t pos = 0;
    while (true) {
        auto cstart = xml.find("<Course", pos);
        if (cstart == std::string::npos) break;
        auto nameAttrStart = xml.find("name=\"", cstart);
        if (nameAttrStart == std::string::npos) break;
        nameAttrStart += 6;
        auto nameAttrEnd = xml.find("\"", nameAttrStart);
        std::string cname = xml.substr(nameAttrStart, nameAttrEnd - nameAttrStart);

        auto valStart = xml.find(">", nameAttrEnd) + 1;
        auto valEnd = xml.find("</Course>", valStart);
        int mark = std:: stoi(xml.substr(valStart, valEnd - valStart));

        courses[cname] = mark;
        pos = valEnd + 9;
    }

    return ITstudent(name, id, programme, courses);
}

void runConsumer(Buffer& buffer, const std::string& sharedDir, int count) {
    int consumed = 0;
    while (consumed < count){
        int fileNum = buffer.take();
        std::string path = sharedDir + "/student" + std::to_string(fileNum) + ".xml";
        if(!std::filesystem::exists(path)) {
            std::cerr << "[Consumer] missing file " << path << "\n";
            continue;
        }
        ITstudent s = parseXML(path);

        std::cout << "[Consumer] student" << fileNum << ".xml\n";
        std::cout << " Name: " << s.name << "\n";
        std::cout << " ID: " << s.studentID << "\n";
        std::cout << " Programme: " << s.programme << "\n";
        std::cout << " Courses:\n";
        for (auto& kv : s.courses) {
            std::cout << "    " << kv.first << ": " <<kv.second << "\n";
        }
        std::cout << "  Average: " << s.average() << "\n";
        std::cout << "  Result: " << (s.passed() ? "Pass" : "Fail") << "\n";

        std::filesystem::remove(path);
        std::cout << "   [Consumer] delete " << path << "\n\n";
        consumed++;
    }
}


int main() {

    Buffer buffer;
    std::string sharedDir = "shared";

    std::thread prod([&]{ runProducer(buffer, sharedDir, 10); });
    std::thread cons([&]{ runConsumer(buffer, sharedDir, 10); });

    prod.join();
    cons.join();

    return 0;
}