add_subdirectory(json)
add_subdirectory(database)
add_subdirectory(server)


add_executable(TraceProject Main.cpp)

target_include_directories(TraceProject 
    PUBLIC "${PROJECT_SOURCE_DIR}/source/include"
)


target_link_libraries(TraceProject PUBLIC pthread fmt::fmt-header-only
 restinio::restinio 
 nlohmann_json::nlohmann_json
 $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
 $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_static>
 database
 json
 server
 )
 