add_executable(TraceProject server.cpp)

message("============================"  ${LIBMONGOCXX_INCLUDE_DIRS} )

include_directories(
    ${LIBMONGOCXX_INCLUDE_DIRS}
)

target_link_libraries(TraceProject PRIVATE pthread fmt::fmt-header-only restinio::restinio nlohmann_json::nlohmann_json mongocxx)
