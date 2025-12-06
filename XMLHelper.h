#pragma once 
# include "ITStudent.h"
#include <string>

inline std::string getTagValue(const std::string &xml, const std::string &tag) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";
    size_t start = xml.find(openTag);
    size_t end = xml.find(closeTag);
    if (start == std::string::npos || end == std::string::npos) return " ";
    start += openTag.length();
    return xml.substr(start, end - start);    
}

inline ITStudent parseXML(const std::string &xml) {
    ITStudent student;
    student.name = getTagValue(xml, "name");
    student.id = getTagValue(xml, "id");
    student.programme = getTagValue(xml, "programme");

    size_t pos= 0;
    while ((pos = xml.find("<course>", pos)) != std::string::npos){
        size_t end = xml.find("</course>", pos);
        std::string courseBlock = xml.substr(pos, end - pos);

        Course c;
        c.name = getTagValue(courseBlock, "cname");
        c.mark = std::stoi(getTagValue(courseBlock, "mark"));
        student.courses.push_back(c);

        pos = end + 8;
    }
    return student;
}

inline std::string toXML(const ITStudent &student) {
    std::string xml = "<student>";
    xml += "<name>" + student.name + "</name>";
    xml += "<id>" + student.id + "</id>";
    xml += "<programme>" + student.programme + "</programme>";
    xml += "<courses>";
    for (auto &c : student.courses) {
        xml +="<course><cname>" + c.name + "</cname><mark>" + std::to_string(c.mark) + "</mark></course>";    
    }
    xml += "</courses></student>";
    return xml;    
}