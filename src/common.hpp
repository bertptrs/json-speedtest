#pragma once

#include <vector>
#include <string>

const extern std::string_view SAMPLE_DATA;

struct SamplePart {
    int a, b;
};

struct SampleStruct {
    bool is_valid;
    std::string name;
    std::string value;
    std::vector<SamplePart> parts;
};
