project(json-speedtest)

cmake_minimum_required(VERSION 3.0)

# Add the libraries under test
find_package(json-dto)
find_package(RapidJSON)
find_package(nlohmann_json)

add_executable(speedtest src/main.cpp)

target_compile_features(speedtest PRIVATE cxx_std_17)
target_compile_options(speedtest PRIVATE -Wall -Wextra -Werror)

target_link_libraries(speedtest PRIVATE
        json-dto::json-dto
        nlohmann_json::nlohmann_json
        )

# RapidJSON doesn't behave
target_include_directories(speedtest PRIVATE "${RAPIDJSON_INCLUDE_DIRS}")
