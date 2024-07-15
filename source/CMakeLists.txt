add_executable(TraceProject server.cpp)
target_link_libraries(TraceProject PRIVATE pthread fmt::fmt-header-only
 restinio::restinio nlohmann_json::nlohmann_json
 $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
 $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)
 