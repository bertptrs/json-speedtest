#include "json_dto.hpp"

#include <json_dto/pub.hpp>

namespace json_dto {
    template<typename Json_Io>
    static void json_io(Json_Io &io, SampleStruct &message) {
        io & mandatory("is_valid", message.is_valid)
            & mandatory("name", message.name)
            & mandatory("value", message.value)
            & mandatory("parts", message.parts);
    }

    template<typename Json_Io>
    static void json_io(Json_Io &io, SamplePart &message) {
        io & mandatory("a", message.a)
            & optional("b", message.b, 42);
    }
}

SampleStruct deserialize_json_dto(std::string_view data) {
    return json_dto::from_json<SampleStruct>(json_dto::make_string_ref(data.data(), data.length()));
}

std::string serialize_json_dto(const SampleStruct &data) {
    return json_dto::to_json(data);
}
