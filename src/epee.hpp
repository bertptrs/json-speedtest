#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <serialization/keyvalue_serialization.h>

// I want to properly measure the compile time, size, and such for every
// implementation, but epee can only be implemented in an intrusive way.
// Workaround: give epee its own everything.

struct EpeeSamplePart {
    int a;
    int b;

    BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(a)
        KV_SERIALIZE_OPT(b, 42)
    END_KV_SERIALIZE_MAP()
};

struct EpeeSampleStruct {
    bool is_valid;
    std::string name;
    std::string value;
    std::vector<EpeeSamplePart> parts;

    BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(is_valid)
        KV_SERIALIZE(name)
        KV_SERIALIZE(value)
        KV_SERIALIZE(parts)
    END_KV_SERIALIZE_MAP()
};

EpeeSampleStruct deserialize_epee(std::string_view data);
std::string serialize_epee(const EpeeSampleStruct &data);
