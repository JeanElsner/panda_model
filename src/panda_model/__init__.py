"""
panda-model allows the offline use of the `Model` class from libfranka.
To do this, the shared library needs to be downloaded from an FCI enabled
Franka Emika master control unit using the included tools. You can download
the shared library using the command line tool by running:

.. code-block:: bash

    panda_model_download 192.168.0.1

See `panda_model_download --help` for additional options. You may also
directly call the :func:`download_library` function from Python.

.. code-block:: python

    from panda_model import download_library

    download_library('192.168.0.1', operating)
    # Library downloaded into:  ./libfrankamodel.linux_x64.so

Once you have the shared library downloaded you can easily instantiate
the :class:`Model` class using the included Python bindings and compute
coriolis, gravity, mass matrix etc. given a state:

.. code-block:: python

    from panda_model import Model

    model = Model('./libfrankamodel.linux_x64.so')

"""
from ._core import download_library, Model, Architecture, OperatingSystem

__all__ = ['download_library', 'Model', 'Architecture', 'OperatingSystem']
