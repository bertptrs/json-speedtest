#pragma once

#include "common.hpp"

std::string serialize_json_dto(const SampleStruct &data);
SampleStruct deserialize_json_dto(std::string_view data);
