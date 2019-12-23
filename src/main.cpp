#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "common.hpp"
#include "json_dto.hpp"
#include "nlohmann.hpp"
#include "rapidjson.hpp"
#include "epee.hpp"

typedef std::string serializer_t(const SampleStruct &);

typedef SampleStruct deserializer_t(std::string_view);

using Clock = std::chrono::high_resolution_clock;

static constexpr int ITERATIONS = 100000;

static std::string read_all_input(std::istream &input) {
    std::string result;
    char buffer[4096];
    while (input) {
        input.read(buffer, sizeof(buffer));

        result.append(buffer, input.gcount());
    }

    return result;
}

void run_epee(std::string data) {
    // Do the same thing as the main loop, but specifically for the epee implementation, since it infects everything.

    auto start = Clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        const EpeeSampleStruct sample = deserialize_epee(data);
        data = serialize_epee(sample);
    }

    std::cout << data << std::endl;

    std::chrono::duration<double, std::nano> elapsed = Clock::now() - start;

    std::cerr << "Time per iteration:" << elapsed.count() / ITERATIONS << "ns" << std::endl;
}

int main(int argc, const char **argv) {
    serializer_t *serializer;
    deserializer_t *deserializer;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " IMPLEMENTATION INPUT_FILE" << std::endl;
        return 1;
    }

    std::string_view implementation_name = argv[1];
    std::ifstream input(argv[2]);
    auto data = read_all_input(input);

    if (implementation_name == "json_dto") {
        serializer = serialize_json_dto;
        deserializer = deserialize_json_dto;
    } else if (implementation_name == "nlohmann") {
        serializer = serialize_nlohmann;
        deserializer = deserialize_nlohmann;
    } else if (implementation_name == "rapidjson") {
        serializer = serialize_rapidjson;
        deserializer = deserialize_rapidjson;
    } else if (implementation_name == "epee") {
        run_epee(std::move(data));
        return 0;
    } else {
        std::cerr << "Unknown implementation: " << implementation_name << std::endl;
        return 1;
    }

    auto start = Clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        const SampleStruct sample = deserializer(data);
        data = serializer(sample);
    }

    std::cout << data << std::endl;

    std::chrono::duration<double, std::nano> elapsed = Clock::now() - start;

    std::cerr << "Time per iteration:" << elapsed.count() / ITERATIONS << "ns" << std::endl;

    return 0;
}
