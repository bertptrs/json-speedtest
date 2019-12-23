#include <iostream>
#include <misc_log_ex.h>
#include <storages/portable_storage.h>
#include "epee.hpp"

EpeeSampleStruct deserialize_epee(std::string_view data) {
    epee::serialization::portable_storage ps;
    ps.load_from_json(std::string(data));
    EpeeSampleStruct result;
    result.load(ps);
    return result;
}

std::string serialize_epee(const EpeeSampleStruct &data) {
    epee::serialization::portable_storage ps;
    data.store(ps);
    std::string result_buffer;
    ps.dump_as_json(result_buffer);

    return result_buffer;
}
