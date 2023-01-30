// #include <pybind11/chrono.h>
// #include <pybind11/eigen.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <map>

#include "library_downloader.h"
#include "model.h"
#include "network.h"
#include "service_types.h"

using research_interface::robot::Connect;
using research_interface::robot::kCommandPort;
using research_interface::robot::LoadModelLibrary;

namespace py = pybind11;

bool downloadLibrary(const std::string &hostname, const std::string &path = "",
                     const LoadModelLibrary::Architecture &architecture =
                         LoadModelLibrary::Architecture::kX64,
                     const LoadModelLibrary::System &operating_system =
                         LoadModelLibrary::System::kLinux,
                     const uint16_t version = 5) {
  std::unique_ptr<franka::Network> network =
      std::make_unique<franka::Network>(hostname, kCommandPort);
  uint16_t ri_version;
  franka::connect<Connect>(*network, version, &ri_version);
  std::map<LoadModelLibrary::Architecture, std::string> enumToString = {
      {LoadModelLibrary::Architecture::kX64, "x64"},
      {LoadModelLibrary::Architecture::kX86, "x86"},
      {LoadModelLibrary::Architecture::kARM64, "arm64"},
      {LoadModelLibrary::Architecture::kARM, "arm"}};
  std::string filename = "libfrankamodel.";
  if (operating_system == LoadModelLibrary::System::kLinux) {
    filename += "linux_" + enumToString[architecture];
  } else {
    filename += "win_" + enumToString[architecture];
  }
  pybind11::module_ os_path = pybind11::module::import("os.path");
  std::string final_path =
      os_path.attr("join")(path, filename).cast<std::string>();
  auto downloader = new franka::LibraryDownloader(
      *network, final_path, architecture, operating_system);
  if (downloader->model_library_file_.exists()) {
    py::print("Library downloaded into: ", downloader->path());
    return true;
  }
  py::print("Failed to write library file. Does the path exist?", path);
  return false;
}

const std::array<double, 3> gravity_earth = {0., 0., -9.81};

PYBIND11_MODULE(_core, m) {
  py::options options;
  // options.disable_enum_members_docstring();
  // options.disable_function_signatures();

  py::enum_<LoadModelLibrary::Architecture>(
      m, "Architecture",
      "Used to describe the processor architecture of the shared library.")
      .value("x64", LoadModelLibrary::Architecture::kX64)
      .value("x86", LoadModelLibrary::Architecture::kX86)
      .value("arm64", LoadModelLibrary::Architecture::kARM64)
      .value("arm", LoadModelLibrary::Architecture::kARM);

  py::enum_<LoadModelLibrary::System>(
      m, "OperatingSystem",
      "Used to describe the operating System of the shared library.")
      .value("linux", LoadModelLibrary::System::kLinux)
      .value("windows", LoadModelLibrary::System::kWindows);

  m.def("download_library", &downloadLibrary, py::arg("hostname"),
        py::arg("path") = "",
        py::arg("architecture") = LoadModelLibrary::Architecture::kX64,
        py::arg("operating_system") = LoadModelLibrary::System::kLinux,
        py::arg("version") = 5, R"delim(
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
        )delim");

  py::enum_<franka::Frame>(m, "Frame",
                           "Enumerates the seven joints, the flange, and the "
                           "end effector of a robot.")
      .value("kJoint1", franka::Frame::kJoint1)
      .value("kJoint2", franka::Frame::kJoint2)
      .value("kJoint3", franka::Frame::kJoint3)
      .value("kJoint4", franka::Frame::kJoint4)
      .value("kJoint5", franka::Frame::kJoint5)
      .value("kJoint6", franka::Frame::kJoint6)
      .value("kJoint7", franka::Frame::kJoint7)
      .value("kFlange", franka::Frame::kFlange)
      .value("kEndEffector", franka::Frame::kEndEffector)
      .value("kStiffness", franka::Frame::kStiffness);

  py::class_<franka::Model>(
      m, "Model",
      "Calculates poses of joints and dynamic properties of the robot.")
      .def(py::init<const std::string &>(), py::arg("path"), R"delim(
      Construct a new `Model` instance given a shared library.

      Args:
        path: Path to the shared library downloaded with `download_library`.
          The library must be compatible with the host system, i.e. in terms
          of processor architecture and operating system.
      )delim")
      .def("pose",
           py::overload_cast<franka::Frame, const std::array<double, 7> &,
                             const std::array<double, 16> &,
                             const std::array<double, 16> &>(
               &franka::Model::pose, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE"), py::arg("EE_T_K"),
           R"delim(
           Gets the 4x4 pose matrix for the given frame in base frame.
           The pose is represented as a 4x4 matrix in column-major format.

           Args:
             frame: The desired frame.
             q: Joint position.
             F_T_EE: End effector in flange frame.
             EE_T_K: Stiffness frame K in the end effector frame.

           Returns:
             Vectorized 4x4 pose matrix, column-major.
           )delim")
      .def("body_jacobian",
           py::overload_cast<franka::Frame, const std::array<double, 7> &,
                             const std::array<double, 16> &,
                             const std::array<double, 16> &>(
               &franka::Model::bodyJacobian, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE"), py::arg("EE_T_K"),
           R"delim(
           Gets the 6x7 Jacobian for the given frame, relative to that frame.
           The Jacobian is represented as a 6x7 matrix in column-major format.

           Args:
             frame: The desired frame.
             q: Joint position.
             F_T_EE: End effector in flange frame.
             EE_T_K: Stiffness frame K in the end effector frame.

           Returns:
             Vectorized 6x7 Jacobian, column-major.
           )delim")
      .def("zero_jacobian",
           py::overload_cast<franka::Frame, const std::array<double, 7> &,
                             const std::array<double, 16> &,
                             const std::array<double, 16> &>(
               &franka::Model::zeroJacobian, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE"), py::arg("EE_T_K"),
           R"delim(
           Gets the 6x7 Jacobian for the given joint relative to the base frame.
           The Jacobian is represented as a 6x7 matrix in column-major format.

           Args:
             frame: The desired frame.
             q: Joint position.
             F_T_EE: End effector in flange frame.
             EE_T_K: Stiffness frame K in the end effector frame.

           Returns:
             Vectorized 6x7 Jacobian, column-major.
           )delim")
      .def("mass",
           py::overload_cast<const std::array<double, 7> &,
                             const std::array<double, 9> &, double,
                             const std::array<double, 3> &>(
               &franka::Model::mass, py::const_),
           py::arg("q"), py::arg("I_total"), py::arg("m_total"),
           py::arg("F_x_Ctotal"), R"delim(
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
           )delim")
      .def("coriolis",
           py::overload_cast<const std::array<double, 7> &,
                             const std::array<double, 7> &,
                             const std::array<double, 9> &, double,
                             const std::array<double, 3> &>(
               &franka::Model::coriolis, py::const_),
           py::arg("q"), py::arg("dq"), py::arg("I_total"), py::arg("m_total"),
           py::arg("F_x_Ctotal"), R"delim(
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
           )delim")
      .def("gravity",
           py::overload_cast<const std::array<double, 7> &, double,
                             const std::array<double, 3> &,
                             const std::array<double, 3> &>(
               &franka::Model::gravity, py::const_),
           py::arg("q"), py::arg("m_total"), py::arg("F_x_Ctotal"),
           py::arg("gravity_earth") = gravity_earth, R"delim(
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
           )delim");
}
