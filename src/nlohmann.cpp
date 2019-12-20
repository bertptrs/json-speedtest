#include <nlohmann/json.hpp>
#include "nlohmann.hpp"

// Example to show how optional arguments could be simplified.
template<typename KeyType, typename TargetType>
void nlohmann_optional_arg(const nlohmann::json &j, const KeyType &keyType, TargetType &target, TargetType fallback={}) {
    // Using find saves one map search over .count() combined with .at()
    if (auto it = j.find(keyType); it != j.end()) {
        it->get_to(target);
    } else {
        target = std::move(fallback);
    }
}

static void to_json(nlohmann::json &j, const SamplePart &data) {
    j = nlohmann::json{{"a", data.a},
                       {"b", data.b}};
}

static void to_json(nlohmann::json &j, const SampleStruct &data) {
    j = nlohmann::json{{"is_valid", data.is_valid},
                       {"name",     data.name},
                       {"value",    data.value},
                       {"parts",    data.parts}};
}

static void from_json(const nlohmann::json &j, SamplePart &data) {
    j.at("a").get_to(data.a);
    nlohmann_optional_arg(j, "b", data.b, 42);
}

static void from_json(const nlohmann::json &j, SampleStruct &data) {
    j.at("is_valid").get_to(data.is_valid);
    j.at("name").get_to(data.name);
    j.at("value").get_to(data.value);
    j.at("parts").get_to(data.parts);
}

std::string serialize_nlohmann(const SampleStruct &data) {
    nlohmann::json j = data;

    return j.dump();
}

SampleStruct deserialize_nlohmann(std::string_view data) {
    auto j = nlohmann::json::parse(data.begin(), data.end());

    return j.get<SampleStruct>();
}
