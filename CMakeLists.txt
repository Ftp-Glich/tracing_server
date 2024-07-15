cmake_minimum_required(VERSION 3.8.0)
project (TraceProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(restinio REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
add_subdirectory(source)
