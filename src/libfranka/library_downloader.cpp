// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include "library_downloader.h"

#include <exception>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

#include <Poco/SharedLibrary.h>

// #include <franka/exception.h>
// #include <research_interface/robot/service_types.h>
#include "service_types.h"

#include "platform.h"

using research_interface::robot::LoadModelLibrary;

namespace panda_model {

LibraryDownloader::LibraryDownloader(Network& network, const std::string &path,
  const LoadModelLibrary::Architecture &architecture,
  const LoadModelLibrary::System &operating_system)
    : model_library_file_{path + Poco::SharedLibrary::suffix()} {
  // using research_interface::robot::LoadModelLibrary;
//   LoadModelLibrary::Architecture architecture;
//   LoadModelLibrary::System operating_system;

// #if defined(LIBFRANKA_X64)
//   architecture = LoadModelLibrary::Architecture::kX64;
// #elif defined(LIBFRANKA_X86)
//   architecture = LoadModelLibrary::Architecture::kX86;
// #elif defined(LIBFRANKA_ARM64)
//   architecture = LoadModelLibrary::Architecture::kARM64;
// #elif defined(LIBFRANKA_ARM)
//   architecture = LoadModelLibrary::Architecture::kARM;
// #else
//   throw std::runtime_error("libfranka: Unsupported architecture!");
// #endif

// #if defined(LIBFRANKA_WINDOWS)
//   operating_system = LoadModelLibrary::System::kWindows;
// #elif defined(LIBFRANKA_LINUX)
//   operating_system = LoadModelLibrary::System::kLinux;
// #else
//   throw std::runtime_error("libfranka: Unsupported operating system!");
// #endif

  using research_interface::robot::LoadModelLibrary;

  uint32_t command_id = network.tcpSendRequest<LoadModelLibrary>(architecture, operating_system);
  std::vector<uint8_t> buffer;
  LoadModelLibrary::Response response =
      network.tcpBlockingReceiveResponse<LoadModelLibrary>(command_id, &buffer);
  if (response.status != LoadModelLibrary::Status::kSuccess) {
    throw std::runtime_error("libfranka: Server reports error when loading model library.");
  }

  try {
    std::ofstream model_library_stream(this->path().c_str(), std::ios_base::out | std::ios_base::binary);
    model_library_stream.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
  } catch (const std::exception& ex) {
    throw std::runtime_error("libfranka: Cannot save model library.");
  }
}

// LibraryDownloader::~LibraryDownloader() {
//   try {
//     if (model_library_file_.exists()) {
//       Poco::TemporaryFile::registerForDeletion(path());
//       model_library_file_.remove();
//     }
//   } catch (...) {
//   }
// }

const std::string& LibraryDownloader::path() const noexcept {
  return model_library_file_.path();
}

};  // namespace panda_model
