from __future__ import annotations
import panda_model._core
import typing
import numpy
_Shape = typing.Tuple[int, ...]

__all__ = [
    "Architecture",
    "Defaults",
    "Frame",
    "Model",
    "OperatingSystem",
    "download_library"
]


class Architecture():
    """
    Used to describe the processor architecture of the shared library.

    Members:

      x64

      x86

      arm64

      arm
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    __members__: dict # value = {'x64': <Architecture.x64: 0>, 'x86': <Architecture.x86: 1>, 'arm64': <Architecture.arm64: 3>, 'arm': <Architecture.arm: 2>}
    arm: panda_model._core.Architecture # value = <Architecture.arm: 2>
    arm64: panda_model._core.Architecture # value = <Architecture.arm64: 3>
    x64: panda_model._core.Architecture # value = <Architecture.x64: 0>
    x86: panda_model._core.Architecture # value = <Architecture.x86: 1>
    pass
class Defaults():
    """
    Default parameters for the Panda with standard gripper and no external load.
    """
    EE_T_K: numpy.ndarray # value = 
    """
    array([[1., 0., 0., 0.],
           [0., 1., 0., 0.],
           [0., 0., 1., 0.],
           [0., 0., 0., 1.]])
    """
    F_T_EE: numpy.ndarray # value = 
    """
    array([[ 0.7071,  0.7071,  0.    ,  0.    ],
           [-0.7071,  0.7071,  0.    ,  0.    ],
           [ 0.    ,  0.    ,  1.    ,  0.1034],
           [ 0.    ,  0.    ,  0.    ,  1.    ]])
    """
    F_X_CTOTAL: numpy.ndarray # value = array([-0.01,  0.  ,  0.03])
    I_TOTAL: numpy.ndarray # value = 
    """
    array([[0.001 , 0.    , 0.    ],
           [0.    , 0.0025, 0.    ],
           [0.    , 0.    , 0.0017]])
    """
    M_TOTAL = 0.73
    pass
class Frame():
    """
    Enumerates the seven joints, the flange, and the end effector of a robot.

    Members:

      kJoint1

      kJoint2

      kJoint3

      kJoint4

      kJoint5

      kJoint6

      kJoint7

      kFlange

      kEndEffector

      kStiffness
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    __members__: dict # value = {'kJoint1': <Frame.kJoint1: 0>, 'kJoint2': <Frame.kJoint2: 1>, 'kJoint3': <Frame.kJoint3: 2>, 'kJoint4': <Frame.kJoint4: 3>, 'kJoint5': <Frame.kJoint5: 4>, 'kJoint6': <Frame.kJoint6: 5>, 'kJoint7': <Frame.kJoint7: 6>, 'kFlange': <Frame.kFlange: 7>, 'kEndEffector': <Frame.kEndEffector: 8>, 'kStiffness': <Frame.kStiffness: 9>}
    kEndEffector: panda_model._core.Frame # value = <Frame.kEndEffector: 8>
    kFlange: panda_model._core.Frame # value = <Frame.kFlange: 7>
    kJoint1: panda_model._core.Frame # value = <Frame.kJoint1: 0>
    kJoint2: panda_model._core.Frame # value = <Frame.kJoint2: 1>
    kJoint3: panda_model._core.Frame # value = <Frame.kJoint3: 2>
    kJoint4: panda_model._core.Frame # value = <Frame.kJoint4: 3>
    kJoint5: panda_model._core.Frame # value = <Frame.kJoint5: 4>
    kJoint6: panda_model._core.Frame # value = <Frame.kJoint6: 5>
    kJoint7: panda_model._core.Frame # value = <Frame.kJoint7: 6>
    kStiffness: panda_model._core.Frame # value = <Frame.kStiffness: 9>
    pass
class Model():
    """
    Calculates poses of joints and dynamic properties of the robot.
    """
    def __init__(self, path: str) -> None: 
        """
        Construct a new `Model` instance given a shared library.

        Args:
          path: Path to the shared library downloaded with `download_library`.
            The library must be compatible with the host system, i.e. in terms
            of processor architecture and operating system.
        """
    def gravity(self, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], m_total: float = 0.73, F_x_Ctotal: numpy.ndarray[numpy.float64, _Shape[3, 1]] = array([-0.01,  0.  ,  0.03]), gravity_earth: numpy.ndarray[numpy.float64, _Shape[3, 1]] = array([ 0.  ,  0.  , -9.81])) -> numpy.ndarray[numpy.float64, _Shape[7, 1]]: 
        """
        Calculates the gravity vector. Unit: :math:`[Nm]`.

        Args:
          q: Joint position.
          m_total: Weight of the attached total load including end effector.
            Unit: :math:`[kg]`.
          F_x_Ctotal: Translation from flange to center of mass of the attached total load.
            Unit: :math:`[m]`.
          gravity_earth: Earth's gravity vector. Unit: :math:`\frac{m}{s^2}`.
            Default to {0.0, 0.0, -9.81}.

        Returns:
          Gravity vector.
        """
    def pose(self, frame:panda_model._core.Frame, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], F_T_EE: numpy.ndarray[numpy.float64, _Shape[4, 4]] = array([[0.7071, 0.7071, 0, 0], [-0.7071, 0.7071, 0, 0], [0, 0, 1, 0.1034], [0, 0, 0, 1]]), EE_T_K: numpy.ndarray[numpy.float64, _Shape[4, 4]] = numpy.eye(4)) -> numpy.ndarray[numpy.float64, _Shape[4, 4]]:
        """
        Gets the 4x4 pose matrix for the given frame in base frame.
        The pose is represented as a 4x4 matrix in column-major format.

        Args:
          frame: The desired frame.
          q: Joint position.
          F_T_EE: End effector in flange frame.
          EE_T_K: Stiffness frame K in the end effector frame.

        Returns:
          Vectorized 4x4 pose matrix, column-major.
        """
    def body_jacobian(self, frame:panda_model._core.Frame, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], F_T_EE: numpy.ndarray[numpy.float64, _Shape[4, 4]] = array([[0.7071, 0.7071, 0, 0], [-0.7071, 0.7071, 0, 0], [0, 0, 1, 0.1034], [0, 0, 0, 1]]), EE_T_K: numpy.ndarray[numpy.float64, _Shape[4, 4]] = numpy.eye(4)) -> numpy.ndarray[numpy.float64, _Shape[6, 7]]:
        """
        Gets the 6x7 Jacobian for the given frame, relative to that frame.
        The Jacobian is represented as a 6x7 matrix in column-major format.

        Args:
          frame: The desired frame.
          q: Joint position.
          F_T_EE: End effector in flange frame.
          EE_T_K: Stiffness frame K in the end effector frame.

        Returns:
        """
    def zero_jacobian(self, frame:panda_model._core.Frame, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], F_T_EE: numpy.ndarray[numpy.float64, _Shape[4, 4]] = array([[0.7071, 0.7071, 0, 0], [-0.7071, 0.7071, 0, 0], [0, 0, 1, 0.1034], [0, 0, 0, 1]]), EE_T_K: numpy.ndarray[numpy.float64, _Shape[4, 4]] = numpy.eye(4)) -> numpy.ndarray[numpy.float64, _Shape[6, 7]]:
        """
        Gets the 6x7 Jacobian for the given joint relative to the base frame.
        The Jacobian is represented as a 6x7 matrix in column-major format.

        Args:
          frame: The desired frame.
          q: Joint position.
          F_T_EE: End effector in flange frame.
          EE_T_K: Stiffness frame K in the end effector frame.

        Returns:
          Vectorized 6x7 Jacobian, column-major.
        """
    def mass(self, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], I_total: numpy.ndarray[numpy.float64, _Shape[4, 4]] = array([0.001, 0, 0], [0, 0.0025, 0], [0, 0, 0.0017]), m_total: float = 0.73, F_x_Ctotal: numpy.ndarray[numpy.float64, _Shape[3, 1]] = array([-0.01, 0, 0.03])) -> numpy.ndarray[numpy.float64, _Shape[7, 7]]:
        """
        Calculates the 7x7 mass matrix. Unit: :math:`[kg \times m^2]`.

        Args:
          q: Joint position.
          I_total: Inertia of the attached total load including end effector, relative to
            center of mass, given as vectorized 3x3 column-major matrix. Unit: :math:`[kg \times m^2]`.
          m_total: Weight of the attached total load including end effector. Unit: :math:`[kg]`.
          F_x_Ctotal: Translation from flange to center of mass of the attached total load.
            Unit: :math:`[m]`.

        Returns:
          Vectorized 7x7 mass matrix, column-major.
        """
    def coriolis(self, q: numpy.ndarray[numpy.float64, _Shape[7, 1]], dq: numpy.ndarray[numpy.float64, _Shape[7, 1]], I_total: numpy.ndarray[numpy.float64, _Shape[4, 4]] = array([0.001, 0, 0], [0, 0.0025, 0], [0, 0, 0.0017]), m_total: float = 0.73, F_x_Ctotal: numpy.ndarray[numpy.float64, _Shape[3, 1]] = array([-0.01, 0, 0.03])) -> numpy.ndarray[numpy.float64, _Shape[7, 1]]:
        """
        Calculates the Coriolis force vector (state-space equation): :math:` c= C \times
        dq`, in :math:`[Nm]`.

        Args:
          q: Joint position.
          dq: Joint velocity.
          I_total: Inertia of the attached total load including end effector, relative to
            center of mass, given as vectorized 3x3 column-major matrix. Unit: :math:`[kg \times m^2]`.
          m_total: Weight of the attached total load including end effector.
            Unit: :math:`[kg]`.
          F_x_Ctotal: Translation from flange to center of mass of the attached total load.
            Unit: :math:`[m]`.

        Returns:
          Coriolis force vector.
        """
    pass
class OperatingSystem():
    """
    Used to describe the operating System of the shared library.

    Members:

      linux

      windows
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    __members__: dict # value = {'linux': <OperatingSystem.linux: 0>, 'windows': <OperatingSystem.windows: 1>}
    linux: panda_model._core.OperatingSystem # value = <OperatingSystem.linux: 0>
    windows: panda_model._core.OperatingSystem # value = <OperatingSystem.windows: 1>
    pass
def download_library(hostname: str, path: str = '', architecture: Architecture = Architecture.x64, operating_system: OperatingSystem = OperatingSystem.linux, version: int = 5) -> str:
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
      Path pointing to the downloaded library.
    """
