""" Download all available shared libraries. """
from panda_model import download_library, Architecture, OperatingSystem

ADDR = '172.16.0.2'
VER = 4

# x64
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.x64,
                 operating_system=OperatingSystem.windows)
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.x64,
                 operating_system=OperatingSystem.linux)
# x86
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.x86,
                 operating_system=OperatingSystem.windows)
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.x86,
                 operating_system=OperatingSystem.linux)
# arm64 and arm
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.arm64,
                 operating_system=OperatingSystem.linux)
download_library(ADDR,
                 version=VER,
                 architecture=Architecture.arm,
                 operating_system=OperatingSystem.linux)
