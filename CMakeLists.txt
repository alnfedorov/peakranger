cmake_minimum_required(VERSION 3.11)

file(STRINGS "VERSION" VERSION)
project(
        peakranger
        VERSION ${VERSION}
        DESCRIPTION "The peak caller for ChIP-Seq data"
        LANGUAGES CXX
)

set(Boost_USE_STATIC_LIBS FALSE)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.56 COMPONENTS program_options thread system REQUIRED)

# peakranger app
file(GLOB SOURCES
        app/*.cpp bam_app/*.cpp bamtools/*.cpp bar/*.cpp common/*.cpp concepts/*.cpp doc/*.cpp ggplay/*.cpp
        math/*.cpp option_parser/*.cpp paired_reads_parser/*.cpp parser/*.cpp region_detector/*.cpp
        region_profile/*.cpp result_reporter/*.cpp short_reads/*.cpp tab_file/*.cpp threading/*.cpp
        utils/*.cpp wiggle/*.cpp)

add_executable(peakranger ${SOURCES})
target_include_directories(peakranger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(peakranger PRIVATE Boost::program_options Boost::thread Boost::system libz.a pthread m)
target_compile_definitions(peakranger PRIVATE VERSION=${VERSION})

target_compile_options(peakranger PRIVATE -Wall -Wextra -Wno-unused-parameter)
target_compile_options(peakranger PRIVATE -std=c++17 -flto -static -O3)
target_link_options(peakranger PRIVATE -flto)

# python bindings - pybind
add_subdirectory(third_party)

set(SOURCES ${SOURCES} pybindings/bcp.cpp)

pybind11_add_module(pybcp MODULE THIN_LTO ${SOURCES})
target_include_directories(pybcp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(pybcp PRIVATE VERSION=${VERSION})

target_compile_options(pybcp PRIVATE -Wall -Wextra -Wno-unused-parameter)
target_compile_options(pybcp PRIVATE -std=c++17 -flto -O3)

target_link_libraries(pybcp PRIVATE Boost::program_options Boost::thread Boost::system z pthread m)
