add_executable(TraceProject server.cpp)
target_link_libraries(TraceProject PRIVATE pthread fmt::fmt-header-only restinio::restinio nlohmann_json::nlohmann_json)
