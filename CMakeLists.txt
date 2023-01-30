cmake_minimum_required(VERSION 3.15...3.25)
project(
  "${SKBUILD_PROJECT_NAME}"
  LANGUAGES CXX
  VERSION "${SKBUILD_PROJECT_VERSION}")

find_package(Eigen3 REQUIRED)
find_package(Poco REQUIRED Foundation Net)
find_package(
  Python
  COMPONENTS Interpreter Development.Module
  REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

install(DIRECTORY include DESTINATION ${SKBUILD_HEADERS_DIR})
install(PROGRAMS scripts/panda_model_download DESTINATION ${SKBUILD_SCRIPTS_DIR})

## _core module
pybind11_add_module(_core
  src/_core.cpp
  src/libfranka/network.cpp
  src/libfranka/library_downloader.cpp
  src/libfranka/model.cpp
  src/libfranka/model_library.cpp
  src/libfranka/library_loader.cpp
)

target_link_libraries(_core PUBLIC
  Poco::Foundation
  Poco::Net
)

target_include_directories(_core SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libfranka>
  ${EIGEN3_INCLUDE_DIRS}
)

target_compile_definitions(_core
  PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
