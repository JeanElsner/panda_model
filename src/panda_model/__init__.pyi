"""
===============
Get the Library
===============
You can download the shared library from any master control unit
using the command line utility by simply calling

.. code-block:: bash

    panda_model_download 192.168.0.1

with the respective IP or hostname of your robot. See
`panda_model_download -\-help` for additional options. You may
have to change the version parameter depending on the FCI version
running on your master controller.

Alternatively you can also control the download from Python code.

.. code-block:: python

    from panda_model import download_library

    download_library('192.168.0.1')
    # Library downloaded into:  ./libfrankamodel.linux_x64.so

================
Access the Model
================

Once you have the shared library downloaded you can easily instantiate
the :class:`Model` class using the included Python bindings and compute
coriolis, gravity, mass matrix etc. given a state:

.. code-block:: python

    from panda_model import Model, Frame

    frame = Frame.kEndEffector
    F_T_EE = np.eye(4).flatten()
    EE_T_K = np.eye(4).flatten()

    model = Model('./libfrankamodel.linux_x64.so')
    model.zero_jacobian(frame, q_0, F_T_EE, EE_T_K)

For more information, please refer to the examples and the API documentation
of the :class:`Model` class.
"""
from __future__ import annotations
import panda_model
import typing
from panda_model._core import Architecture
from panda_model._core import Frame
from panda_model._core import Model
from panda_model._core import OperatingSystem

__all__ = [
    "Architecture",
    "Frame",
    "Model",
    "OperatingSystem",
    "download_library"
]


def download_library(hostname: str, path: str = '', architecture: _core.Architecture = Architecture.x64, operating_system: _core.OperatingSystem = OperatingSystem.linux, version: int = 5) -> bool:
    """
    Download model library from a connected control unit.

    Args:
      hostname: Hostname or IP address of the master control unit.
      path: The path the shared library is downloaded to.
      architecture: Download the shared library built for the given
        processor architecture.
      operating_system: Download the shared library built for the given
        operating system.
      version: FCI version running on the targeted master control unit.

    Returns:
      True if download successful.
    """
__all__ = ['download_library', 'Model', 'Frame', 'Architecture', 'OperatingSystem']
