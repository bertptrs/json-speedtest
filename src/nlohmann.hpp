#pragma once

#include "common.hpp"

std::string serialize_nlohmann(const SampleStruct &data);
SampleStruct deserialize_nlohmann(std::string_view data);
