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
from ._core import download_library, Frame, Model, Architecture, OperatingSystem

__all__ = ['download_library', 'Model', 'Frame', 'Architecture', 'OperatingSystem']
