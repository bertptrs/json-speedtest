//
// Created by bert on 20/12/2019.
//

#include <rapidjson/document.h>
#include <algorithm>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "rapidjson.hpp"

namespace {

    template<typename ValueType>
    void deserialize_key(const rapidjson::Value &value, const char key[], ValueType &target) {
        target = value[key].Get<ValueType>();
    }

    template<typename T>
    T deserialize(const rapidjson::Value &value) {
        return value.Get<T>();
    };

    template<>
    SamplePart deserialize<SamplePart>(const rapidjson::Value &value) {
        SamplePart part{};
        deserialize_key(value, "a", part.a);

        if (value.HasMember("b")) {
            deserialize_key(value, "b", part.b);
        } else {
            part.b = 42;
        }

        return part;
    }

    template<>
    SampleStruct deserialize<SampleStruct>(const rapidjson::Value &value) {
        SampleStruct data;
        deserialize_key(value, "is_valid", data.is_valid);
        deserialize_key(value, "name", data.name);
        deserialize_key(value, "value", data.value);

        auto parts = value["parts"].GetArray();

        std::transform(parts.begin(), parts.end(), std::back_inserter(data.parts), [](const rapidjson::Value &value) {
            return deserialize<SamplePart>(value);
        });

        return data;
    }

    template<typename Allocator>
    rapidjson::Value serialize(const SamplePart &data, Allocator &allocator) {
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("a", data.a, allocator);
        value.AddMember("b", data.b, allocator);

        return value;
    }

    template<typename Allocator>
    rapidjson::Value serialize(const SampleStruct &data, Allocator &allocator) {
        rapidjson::Value value;

        value.SetObject();
        value.AddMember("is_valid", data.is_valid, allocator);
        value.AddMember("name", data.name, allocator);
        value.AddMember("parts", rapidjson::Value().SetArray(), allocator);

        for (auto& part : data.parts) {
            value["parts"].GetArray().PushBack(serialize(part, allocator), allocator);
        }

        return value;
    }
}

SampleStruct deserialize_rapidjson(std::string_view data) {
    rapidjson::Document d;
    d.Parse(data.data(), data.length());

    return deserialize<SampleStruct>(d);
}

std::string serialize_rapidjson(const SampleStruct &data) {
    rapidjson::MemoryPoolAllocator<> allocator;
    auto value = serialize(data, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer writer(buffer);
    value.Accept(writer);

    return buffer.GetString();
}
