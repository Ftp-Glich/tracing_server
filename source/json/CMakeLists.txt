add_library(json STATIC
    jsonParser.cpp
)

target_include_directories(json
    PUBLIC "${PROJECT_SOURCE_DIR}/source/include"
)