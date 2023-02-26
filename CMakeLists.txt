cmake_minimum_required(VERSION 3.15...3.25)

include(ExternalProject)

ExternalProject_Add(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/eigen
  CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/eigen-install
      -DBUILD_TESTING=OFF
)

ExternalProject_Add(
  Poco
  GIT_REPOSITORY https://github.com/pocoproject/poco.git
  GIT_TAG poco-1.11.0-release
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/poco
  CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/poco-install
      -DCMAKE_BUILD_TYPE=Release
      -DPOCO_STATIC=ON
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON
      -DENABLE_DATA_ODBC=OFF
      -DENABLE_DATA_POSTGRESQL=OFF
      -DENABLE_DATA_MYSQL=OFF
      -DENABLE_DATA_SQLITE=OFF
      -DENABLE_ACTIVERECORD=OFF
      #
      -DENABLE_ENCODINGS=OFF
      -DENABLE_ENCODINGS_COMPILER=OFF
      -DENABLE_XML=OFF
      -DENABLE_JSON=OFF
      -DENABLE_MONGODB=OFF
      -DENABLE_REDIS=OFF
      -DENABLE_PROMETHEUS=OFF
      -DENABLE_PDF=OFF
      -DENABLE_UTIL=OFF
      -DENABLE_SEVENZIP=OFF
      -DENABLE_ZIP=OFF
      -DENABLE_CPPPARSER=OFF
      -DENABLE_POCODOC=OFF
      -DENABLE_PAGECOMPILER=OFF
      -DENABLE_PAGECOMPILER_FILE2PAGE=OFF
      -DENABLE_ACTIVERECORD=OFF
      -DENABLE_ACTIVERECORD_COMPILER=OFF
)

option(BUILD_PYTHON "Build Python module" OFF)
if(BUILD_PYTHON)
  project(
    "${SKBUILD_PROJECT_NAME}"
    LANGUAGES CXX
    VERSION "${SKBUILD_PROJECT_VERSION}")

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
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/lib/libPocoFoundation.a
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/lib/libPocoNet.a
  )

  target_include_directories(_core SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libfranka>
    ${CMAKE_CURRENT_BINARY_DIR}/eigen-install/include/eigen3
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/include
  )

  target_compile_definitions(_core
    PRIVATE VERSION_INFO=${PROJECT_VERSION})

  install(DIRECTORY include DESTINATION ${SKBUILD_HEADERS_DIR})
  install(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
endif()

option(BUILD_CPP "Build C++ module" OFF)
if(BUILD_CPP)
  project(
    panda_model
    LANGUAGES CXX
    VERSION "0.1.0")

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
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/lib/libPocoFoundation.a
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/lib/libPocoNet.a
  )

  target_include_directories(pandamodel PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libfranka>
    ${CMAKE_CURRENT_BINARY_DIR}/eigen-install/include/eigen3
    ${CMAKE_CURRENT_BINARY_DIR}/poco-install/include
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
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pandamodel
)

  ## Packaging
  set(CPACK_PACKAGE_VENDOR "Jean Elsner")
  set(CPACK_GENERATOR "DEB;TGZ")
  set(CPACK_PACKAGE_VERSION "0.1.0")
  set(CPACK_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_PROCESSOR})

  # Debian versions require a dash
  set(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}-1)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jean Elsner")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpoco-dev")
  include(CPack)
endif()