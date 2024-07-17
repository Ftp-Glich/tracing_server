add_library(database STATIC
    DbHandler.cpp
)

target_include_directories(database
    PUBLIC "${PROJECT_SOURCE_DIR}/source/include"
)