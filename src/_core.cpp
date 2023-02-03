#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <map>

#include "library_downloader.h"
#include "network.h"
#include "pandamodel/defaults.h"
#include "pandamodel/model.h"
#include "service_types.h"

using research_interface::robot::Connect;
using research_interface::robot::kCommandPort;
using research_interface::robot::LoadModelLibrary;

namespace py = pybind11;

std::string downloadLibrary(const std::string &hostname,
                            const std::string &path = "",
                            const LoadModelLibrary::Architecture &architecture =
                                LoadModelLibrary::Architecture::kX64,
                            const LoadModelLibrary::System &operating_system =
                                LoadModelLibrary::System::kLinux,
                            const uint16_t version = 5) {
  std::unique_ptr<panda_model::Network> network =
      std::make_unique<panda_model::Network>(hostname, kCommandPort);
  uint16_t ri_version;
  panda_model::connect<Connect>(*network, version, &ri_version);
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
  auto downloader = new panda_model::LibraryDownloader(
      *network, final_path, architecture, operating_system);
  if (downloader->model_library_file_.exists()) {
    return downloader->path();
  }
  throw std::runtime_error("Failed to write library file. Does the path \"" +
                           path + "\" exist?");
}

PYBIND11_MODULE(_core, m) {
  py::options options;
  options.disable_enum_members_docstring();
  options.disable_function_signatures();

  py::class_<Defaults>(m, "Defaults",
                       "Default parameters for the Panda with standard "
                       "gripper and no external load.")
      .def_readonly_static("F_X_CTOTAL", &Defaults::F_x_Ctotal)
      .def_readonly_static("M_TOTAL", &Defaults::m_total)
      .def_readonly_static("I_TOTAL", &Defaults::I_total)
      .def_readonly_static("EE_T_K", &Defaults::EE_T_K)
      .def_readonly_static("F_T_EE", &Defaults::F_T_EE);

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
          Path pointing to the downloaded library.
        )delim");

  py::enum_<panda_model::Frame>(
      m, "Frame",
      "Enumerates the seven joints, the flange, and the "
      "end effector of a robot.")
      .value("kJoint1", panda_model::Frame::kJoint1)
      .value("kJoint2", panda_model::Frame::kJoint2)
      .value("kJoint3", panda_model::Frame::kJoint3)
      .value("kJoint4", panda_model::Frame::kJoint4)
      .value("kJoint5", panda_model::Frame::kJoint5)
      .value("kJoint6", panda_model::Frame::kJoint6)
      .value("kJoint7", panda_model::Frame::kJoint7)
      .value("kFlange", panda_model::Frame::kFlange)
      .value("kEndEffector", panda_model::Frame::kEndEffector)
      .value("kStiffness", panda_model::Frame::kStiffness);

  py::class_<panda_model::Model>(
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
           py::overload_cast<panda_model::Frame,
                             const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix4d &, const Eigen::Matrix4d &>(
               &panda_model::Model::pose, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE") = Defaults::F_T_EE,
           py::arg("EE_T_K") = Defaults::EE_T_K, R"delim(
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
           py::overload_cast<panda_model::Frame,
                             const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix4d &, const Eigen::Matrix4d &>(
               &panda_model::Model::bodyJacobian, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE") = Defaults::F_T_EE,
           py::arg("EE_T_K") = Defaults::EE_T_K, R"delim(
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
           py::overload_cast<panda_model::Frame,
                             const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix<double, 4, 4> &,
                             const Eigen::Matrix<double, 4, 4> &>(
               &panda_model::Model::zeroJacobian, py::const_),
           py::arg("frame"), py::arg("q"), py::arg("F_T_EE") = Defaults::F_T_EE,
           py::arg("EE_T_K") = Defaults::EE_T_K, R"delim(
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
           py::overload_cast<const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix3d &, double,
                             const Eigen::Vector3d &>(&panda_model::Model::mass,
                                                      py::const_),
           py::arg("q"), py::arg("I_total") = Defaults::I_total,
           py::arg("m_total") = Defaults::m_total,
           py::arg("F_x_Ctotal") = Defaults::F_x_Ctotal, R"delim(
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
           py::overload_cast<const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix<double, 7, 1> &,
                             const Eigen::Matrix3d &, double,
                             const Eigen::Vector3d &>(
               &panda_model::Model::coriolis, py::const_),
           py::arg("q"), py::arg("dq"), py::arg("I_total") = Defaults::I_total,
           py::arg("m_total") = Defaults::m_total,
           py::arg("F_x_Ctotal") = Defaults::F_x_Ctotal, R"delim(
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
           py::overload_cast<const Eigen::Matrix<double, 7, 1> &, double,
                             const Eigen::Vector3d &, const Eigen::Vector3d &>(
               &panda_model::Model::gravity, py::const_),
           py::arg("q"), py::arg("m_total") = Defaults::m_total,
           py::arg("F_x_Ctotal") = Defaults::F_x_Ctotal,
           py::arg("gravity_earth") = Defaults::gravity_earth, R"delim(
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
