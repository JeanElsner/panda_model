cmake_minimum_required(VERSION 3.15...3.25)

option(BUILD_PYTHON "Build Python module" OFF)
if(BUILD_PYTHON)
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

  ## _core module
  pybind11_add_module(_core
    src/_core.cpp
    src/defaults.cpp
    src/libfranka/network.cpp
    src/libfranka/library_downloader.cpp
    src/libfranka/model.cpp
    src/libfranka/model_library.cpp
    src/libfranka/library_loader.cpp
  )

  target_link_libraries(_core PRIVATE
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

  install(DIRECTORY include DESTINATION ${SKBUILD_HEADERS_DIR})
  install(PROGRAMS scripts/panda_model_download DESTINATION ${SKBUILD_SCRIPTS_DIR})
  install(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
endif()

option(BUILD_CPP "Build C++ module" OFF)
if(BUILD_CPP)
  project(
    panda_model
    LANGUAGES CXX
    VERSION "0.0.3")

  find_package(Eigen3 REQUIRED)
  find_package(Poco REQUIRED Foundation Net)

  ## pandamodel module
  add_library(pandamodel SHARED
    src/libfranka/network.cpp
    src/libfranka/library_downloader.cpp
    src/libfranka/model.cpp
    src/libfranka/model_library.cpp
    src/libfranka/library_loader.cpp
    src/defaults.cpp
  )
  add_library(PandaModel::pandamodel ALIAS pandamodel)

  target_link_libraries(pandamodel PRIVATE
    Poco::Foundation
    Poco::Net
  )

  target_include_directories(pandamodel PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libfranka>
    ${EIGEN3_INCLUDE_DIRS}
  )

  include(GNUInstallDirs)

  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    USE_SOURCE_PERMISSIONS
  )

  install(TARGETS pandamodel
    EXPORT PandaModelTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  install(EXPORT PandaModelTargets
    FILE PandaModelTargets.cmake
    NAMESPACE PandaModel::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pandamodel
  )

  include(CMakePackageConfigHelpers)
  configure_package_config_file(cmake/PandaModelConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PandaModelConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pandamodel
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/PandaModelConfig.cmake
  # ${CMAKE_CURRENT_BINARY_DIR}/FrankaConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pandamodel
)

  ## Packaging
  set(CPACK_PACKAGE_VENDOR "Jean Elsner")
  set(CPACK_GENERATOR "DEB;TGZ")
  set(CPACK_PACKAGE_VERSION "0.0.3")
  set(CPACK_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_PROCESSOR})

  # Debian versions require a dash
  set(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}-1)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jean Elsner")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpoco-dev")
  # set(CPACK_DEBIAN_PACKAGE_CONFLICTS "ros-kinetic-libfranka, ros-melodic-libfranka, ros-noetic-libfranka, ros-foxy-libfranka")
  include(CPack)
endif()