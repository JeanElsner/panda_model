# panda-model

![robot_model logo](https://raw.githubusercontent.com/JeanElsner/panda_model/main/logo.jpg)

[![Documentation Status](https://readthedocs.org/projects/panda-model/badge/?version=latest)](https://panda-model.readthedocs.io/en/latest/?badge=latest)
[![build workflow](https://github.com/JeanElsner/panda_model/actions/workflows/build.yml/badge.svg)](https://github.com/JeanElsner/panda_model/actions/workflows/build.yml)

panda-model allows the offline use of the `Model` class from [libfranka](https://github.com/frankaemika/libfranka) without a connection to the master control unit. To do this, a shared library needs to be downloaded from an FCI enabled Franka Emika master control unit using the included tools.

To get startet install panda-model as described below and check out the [Documentation](https://panda-model.readthedocs.io) as well as the examples.

# Installation
## Requirements
panda-model requires POCO C++ libraries at runtime, additionally Eigen3 is required to build the project. You can install all necessary requirements on Ubuntu by running:
```
sudo apt-get install python3-pip build-essential cmake libpoco-dev libeigen3-dev
```
## Using pip
```
pip install panda_model
```
## From Source
### Python
Clone the repository and install the package using pip by executing the following from the root directory:
```
pip install .
```
This will install the command line script `panda_model_download` as well as Python bindings for the modified `Model` class.
### C++
To use the model in C++ you can build the necessary library by running:
```
mkdir build && cd build
cmake .. -DBUILD_CPP=ON
cmake --build .
```
You can then install the library using `sudo make install` or by building a deb package:
```
cpack -G DEB
sudo dpkg -i panda_model*.deb
```