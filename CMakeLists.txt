project(json-speedtest)

cmake_minimum_required(VERSION 3.0)

# Add the libraries under test
find_package(json-dto)
find_package(RapidJSON)
find_package(nlohmann_json)

add_executable(speedtest src/main.cpp src/common.cpp src/common.hpp src/json_dto.cpp src/json_dto.hpp src/nlohmann.cpp src/nlohmann.hpp src/rapidjson.cpp src/rapidjson.hpp src/epee.cpp src/epee.hpp)

target_compile_features(speedtest PRIVATE cxx_std_17)
target_compile_options(speedtest PRIVATE -Wall -Wextra)

target_link_libraries(speedtest PRIVATE
        json-dto::json-dto
        nlohmann_json::nlohmann_json
        )

# RapidJSON doesn't behave
target_include_directories(speedtest PRIVATE "${RAPIDJSON_INCLUDE_DIRS}")

# Import epee
add_subdirectory(third_party/easylogging++)
target_compile_definitions(easylogging PUBLIC -DAUTO_INITIALIZE_EASYLOGGINGPP)
target_link_libraries(speedtest PRIVATE easylogging "${EASYLOGGING_LIBRARY_DIRS}")
target_include_directories(speedtest PRIVATE "third_party/epee/include" "${EASYLOGGING_INCLUDE}" "third_party/easylogging++")
