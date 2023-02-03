r"""
===============
Get the Library
===============

You can download the shared library from any master control unit
using the command line utility by simply calling

.. code-block:: bash

   panda_model_download <robot-ip>


with the respective IP or hostname of your robot. See
`panda_model_download -\-help` for additional options. You may
have to change the version parameter depending on the FCI version
running on your master controller.

Alternatively you can also control the download from Python code.

.. code-block:: python

   from panda_model import download_library

   path = download_library('<robot-ip>')
   print(f'Library downloaded as: {path}')


================
Access the Model
================

Once you have the shared library downloaded you can easily instantiate
the :class:`Model` class using the included Python bindings and compute
coriolis, gravity, mass matrix etc. given a state:

.. code-block:: python

   from panda_model import Model, Frame

   frame = Frame.kEndEffector
   q = [0, -np.pi / 4, 0, -3 * np.pi / 4, 0, np.pi / 2, np.pi / 4]

   model = Model('<path-to-lib>')
   model.zero_jacobian(frame, q)


================
Run the Examples
================

To run the examples included in the repository, you need to set a few
environment variables. Set `PANDA_MODEL_PATH`, `PANDA_MODEL_HOST` and
`PANDA_MODEL_VER` to the path pointing to the shared library you downloaded,
the hostname or IP of the connected control unit and the server version
running on the control unit respectively. For instance, on a Unix-like system
simply run:

.. code-block:: bash

   export PANDA_MODEL_PATH=<path-to-lib> PANDA_MODEL_HOST=<robot-ip> PANDA_MODEL_VER=<version>

"""
import numpy as np

from ._core import (Architecture, Defaults, Frame, Model, OperatingSystem,
                    download_library)

__all__ = [
    "download_library",
    "Model",
    "Frame",
    "Defaults",
    "Architecture",
    "OperatingSystem",
]
