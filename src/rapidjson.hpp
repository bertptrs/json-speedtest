#pragma once

#include "common.hpp"

std::string serialize_rapidjson(const SampleStruct &data);
SampleStruct deserialize_rapidjson(std::string_view data);
