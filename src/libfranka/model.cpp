// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
// #include <franka/model.h>
#include "pandamodel/model.h"

#include <sstream>
#include <iostream>

#include <Eigen/Core>

#include "model_library.h"
#include "service_types.h"

using namespace std::string_literals;  // NOLINT(google-build-using-namespace)

namespace panda_model {

Frame operator++(Frame& frame, int /* dummy */) noexcept {
  Frame original = frame;
  frame = static_cast<Frame>(static_cast<std::underlying_type_t<Frame>>(frame) + 1);
  return original;
}

Model::Model(const std::string &path) : library_{new ModelLibrary(path)} {}

// Has to be declared here, as the ModelLibrary type is incomplete in the header
Model::~Model() noexcept = default;
Model::Model(Model&&) noexcept = default;
Model& Model::operator=(Model&&) noexcept = default;

Eigen::Matrix4d Model::pose(
    Frame frame,
    const Eigen::Matrix<double, 7, 1>& q,
    const Eigen::Matrix4d& F_T_EE,
    const Eigen::Matrix4d& EE_T_K)
    const {
  std::array<double, 16> output;
  switch (frame) {
    case Frame::kJoint1:
      library_->joint1(q.data(), output.data());
      break;
    case Frame::kJoint2:
      library_->joint2(q.data(), output.data());
      break;
    case Frame::kJoint3:
      library_->joint3(q.data(), output.data());
      break;
    case Frame::kJoint4:
      library_->joint4(q.data(), output.data());
      break;
    case Frame::kJoint5:
      library_->joint5(q.data(), output.data());
      break;
    case Frame::kJoint6:
      library_->joint6(q.data(), output.data());
      break;
    case Frame::kJoint7:
      library_->joint7(q.data(), output.data());
      break;
    case Frame::kFlange:
      library_->flange(q.data(), output.data());
      break;
    case Frame::kEndEffector:
      library_->ee(q.data(), F_T_EE.data(), output.data());
      break;
    case Frame::kStiffness:
      library_->ee(
          q.data(),
          Eigen::Matrix4d(Eigen::Matrix4d(F_T_EE.data()) * Eigen::Matrix4d(EE_T_K.data())).data(),
          output.data());
      break;
    default:
      throw std::invalid_argument("Invalid frame given.");
  }
  return Eigen::Map<const Eigen::Matrix4d>(output.data());
}

Eigen::Matrix<double, 6, 7> Model::bodyJacobian(
    Frame frame,
    const Eigen::Matrix<double, 7, 1>& q,
    const Eigen::Matrix4d& F_T_EE,
    const Eigen::Matrix4d& EE_T_K)
    const {
  std::array<double, 42> output;
  switch (frame) {
    case Frame::kJoint1:
      library_->body_jacobian_joint1(output.data());
      break;
    case Frame::kJoint2:
      library_->body_jacobian_joint2(q.data(), output.data());
      break;
    case Frame::kJoint3:
      library_->body_jacobian_joint3(q.data(), output.data());
      break;
    case Frame::kJoint4:
      library_->body_jacobian_joint4(q.data(), output.data());
      break;
    case Frame::kJoint5:
      library_->body_jacobian_joint5(q.data(), output.data());
      break;
    case Frame::kJoint6:
      library_->body_jacobian_joint6(q.data(), output.data());
      break;
    case Frame::kJoint7:
      library_->body_jacobian_joint7(q.data(), output.data());
      break;
    case Frame::kFlange:
      library_->body_jacobian_flange(q.data(), output.data());
      break;
    case Frame::kEndEffector:
      library_->body_jacobian_ee(q.data(), F_T_EE.data(), output.data());
      break;
    case Frame::kStiffness:
      library_->body_jacobian_ee(
          q.data(),
          Eigen::Matrix4d(Eigen::Matrix4d(F_T_EE.data()) * Eigen::Matrix4d(EE_T_K.data())).data(),
          output.data());
      break;
    default:
      throw std::invalid_argument("Invalid frame given.");
  }
  return Eigen::Map<const Eigen::Matrix<double, 6, 7>>(output.data());
}

Eigen::Matrix<double, 6, 7> Model::zeroJacobian(
    Frame frame,
    const Eigen::Matrix<double, 7, 1>& q,
    const Eigen::Matrix4d& F_T_EE,
    const Eigen::Matrix4d& EE_T_K)
    const {
  std::array<double, 42> output;
  switch (frame) {
    case Frame::kJoint1:
      library_->zero_jacobian_joint1(output.data());
      break;
    case Frame::kJoint2:
      library_->zero_jacobian_joint2(q.data(), output.data());
      break;
    case Frame::kJoint3:
      library_->zero_jacobian_joint3(q.data(), output.data());
      break;
    case Frame::kJoint4:
      library_->zero_jacobian_joint4(q.data(), output.data());
      break;
    case Frame::kJoint5:
      library_->zero_jacobian_joint5(q.data(), output.data());
      break;
    case Frame::kJoint6:
      library_->zero_jacobian_joint6(q.data(), output.data());
      break;
    case Frame::kJoint7:
      library_->zero_jacobian_joint7(q.data(), output.data());
      break;
    case Frame::kFlange:
      library_->zero_jacobian_flange(q.data(), output.data());
      break;
    case Frame::kEndEffector:
      library_->zero_jacobian_ee(q.data(), F_T_EE.data(), output.data());
      break;
    case Frame::kStiffness:
      library_->zero_jacobian_ee(
          q.data(),
          Eigen::Matrix4d(Eigen::Matrix4d(F_T_EE.data()) * Eigen::Matrix4d(EE_T_K.data())).data(),
          output.data());
      break;
    default:
      throw std::invalid_argument("Invalid frame given.");
  }
  std::cout << F_T_EE << std::endl;
  return Eigen::Map<const Eigen::Matrix<double, 6, 7>>(output.data());
}

Eigen::Matrix<double, 7, 7> Model::mass(
    const Eigen::Matrix<double, 7, 1>& q,
    const Eigen::Matrix3d& I_total,
    double m_total,
    const Eigen::Vector3d& F_x_Ctotal)
    const noexcept {
  std::array<double, 49> output;
  library_->mass(q.data(), I_total.data(), m_total, F_x_Ctotal.data(), output.data());
  return Eigen::Map<const Eigen::Matrix<double, 7, 7>>(output.data());
}

Eigen::Matrix<double, 7, 1> Model::coriolis(
    const Eigen::Matrix<double, 7, 1>& q,
    const Eigen::Matrix<double, 7, 1>& dq,
    const Eigen::Matrix3d& I_total,
    double m_total,
    const Eigen::Vector3d& F_x_Ctotal)
    const noexcept {
  std::array<double, 7> output;
  library_->coriolis(q.data(), dq.data(), I_total.data(), m_total, F_x_Ctotal.data(),
                     output.data());
  return Eigen::Map<const Eigen::Matrix<double, 7, 1>>(output.data());
}

Eigen::Matrix<double, 7, 1> Model::gravity(
    const Eigen::Matrix<double, 7, 1>& q,
    double m_total,
  const Eigen::Vector3d& F_x_Ctotal,
  const Eigen::Vector3d& gravity_earth) const noexcept {
  std::array<double, 7> output;
  library_->gravity(q.data(), gravity_earth.data(), m_total, F_x_Ctotal.data(), output.data());
  return Eigen::Map<const Eigen::Matrix<double, 7, 1>>(output.data());
}

}  // namespace panda_model
