#include <iostream>
#include <string>
#include <chrono>
#include "common.hpp"
#include "json_dto.hpp"

static std::string read_all_input() {
    std::string result;
    char buffer[4096];
    while (std::cin) {
        std::cin.read(buffer, sizeof(buffer));

        result.append(buffer, std::cin.gcount());
    }

    return result;
}

int main(int, const char **) {
    using Clock = std::chrono::high_resolution_clock;
    constexpr int iterations = 100000;
    auto data = read_all_input();
    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        const SampleStruct sample = deserialize_json_dto(data);
        data = serialize_json_dto(sample);
    }

    const SampleStruct sample = deserialize_json_dto(data);

    std::cout << serialize_json_dto(sample) << '\n';

    std::chrono::duration<double, std::nano> elapsed = Clock::now() - start;

    std::cerr << "Time per iteration:" << elapsed.count() / iterations << "ns" << std::endl;

    return 0;
}
