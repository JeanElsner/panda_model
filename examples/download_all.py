"""
Download all available shared libraries.
Set the environment variables PANDA_MODEL_HOST and PANDA_MODEL_VER
to the hostname and version of your master control unit to run.
"""
import os

from panda_model import Architecture, OperatingSystem, download_library

ADDR = os.environ.get('PANDA_MODEL_HOST')
VER = int(os.environ.get('PANDA_MODEL_VER'))

args = [(Architecture.x64, OperatingSystem.windows),
        (Architecture.x86, OperatingSystem.windows),
        (Architecture.x64, OperatingSystem.linux),
        (Architecture.x86, OperatingSystem.linux),
        (Architecture.arm64, OperatingSystem.linux),
        (Architecture.arm, OperatingSystem.linux)]

for a in args:
  download_library(ADDR, version=VER, architecture=a[0], operating_system=a[1])
