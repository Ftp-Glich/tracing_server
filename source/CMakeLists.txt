add_executable(TraceProject Main.cpp)

include_directories("${PROJECT_SOURCE_DIR}/source/include")

target_link_libraries(
 TraceProject PUBLIC pthread fmt::fmt-header-only
 restinio::restinio 
 nlohmann_json::nlohmann_json
 $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
 $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_static>
 valijson
 server
 bcrypt 
)
 
find_path(DEPS_INCLUDE_DIR include REQUIRED)
include_directories(${DEPS_INCLUDE_DIR}/include ${DEPS_INCLUDE_DIR}/include/bsoncxx/v_noabi ${DEPS_INCLUDE_DIR}/include/mongocxx/v_noabi)
  

add_subdirectory(server)
add_subdirectory(bcrypt)

