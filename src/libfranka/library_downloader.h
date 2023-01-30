// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#pragma once

#include <string>

#include <Poco/TemporaryFile.h>

#include "network.h"
#include "service_types.h"

using research_interface::robot::LoadModelLibrary;

namespace panda_model {

class LibraryDownloader {
 public:
  LibraryDownloader(Network& network, const std::string &path, 
    const LoadModelLibrary::Architecture &architecture,
    const LoadModelLibrary::System &operating_system);
  // ~LibraryDownloader();

  const std::string& path() const noexcept;

//  private:
  Poco::File model_library_file_;
};

};  // namespace panda_model
