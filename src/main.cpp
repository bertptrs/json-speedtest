#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "common.hpp"
#include "json_dto.hpp"
#include "nlohmann.hpp"

typedef std::string serializer_t(const SampleStruct&);
typedef SampleStruct deserializer_t(std::string_view);

static std::string read_all_input(std::istream& input) {
    std::string result;
    char buffer[4096];
    while (input) {
        input.read(buffer, sizeof(buffer));

        result.append(buffer, input.gcount());
    }

    return result;
}

int main(int argc, const char ** argv) {
    serializer_t *serializer;
    deserializer_t *deserializer;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " IMPLEMENTATION INPUT_FILE" << std::endl;
        return 1;
    }

    std::string_view implementation_name = argv[1];

    if (implementation_name == "json_dto") {
        serializer = serialize_json_dto;
        deserializer = deserialize_json_dto;
    } else if (implementation_name == "nlohmann") {
        serializer = serialize_nlohmann;
        deserializer = deserialize_nlohmann;
    } else {
        std::cerr << "Unknown implementation: " << implementation_name << std::endl;
        return 1;
    }

    std::ifstream input(argv[2]);


    using Clock = std::chrono::high_resolution_clock;
    constexpr int iterations = 100000;
    auto data = read_all_input(input);
    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        const SampleStruct sample = deserializer(data);
        data = serializer(sample);
    }

    std::cout << data << std::endl;

    std::chrono::duration<double, std::nano> elapsed = Clock::now() - start;

    std::cerr << "Time per iteration:" << elapsed.count() / iterations << "ns" << std::endl;

    return 0;
}
