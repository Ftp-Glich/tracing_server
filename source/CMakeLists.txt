cmake_minimum_required(VERSION 3.8.0)
project (TraceProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)

set(CMAKE_TOOLCHAIN_FILE $ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_PREFIX_PATH $ENV{HOME}/project/vcpkg_installed/x64-linux/share)
find_package(nlohmann_json 3.2.0 REQUIRED)
include_directories($ENV{HOME}/project/vcpkg_installed/x64-linux/include)
link_directories($ENV{HOME}/project/vcpkg_installed/x64-linux/lib)
add_executable(TraceProject server.cpp)
# RESTinio itself
#find_package(restinio REQUIRED)
# Make your project dependent on restinio,
# and let cmake deal with all the headers paths and linked libs.
#target_link_libraries(TraceProject PRIVATE restinio::restinio)
target_link_libraries(TraceProject PRIVATE pthread fmt llhttp  nlohmann_json::nlohmann_json)
#add_subdirectory(<restinio_dir>/dev/restionio)
