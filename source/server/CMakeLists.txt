add_library(server STATIC
    ServerHandler.cpp
)

target_include_directories(server
    PUBLIC "${PROJECT_SOURCE_DIR}/source/include"
)