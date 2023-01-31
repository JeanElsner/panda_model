cmake_minimum_required(VERSION 3.15...3.25)
project(
    main
    LANGUAGES CXX)

find_package(PandaModel REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main ${PandaModel_LIBRARIES})
target_include_directories(main PRIVATE ${PandaModel_INCLUDE_DIRS})
