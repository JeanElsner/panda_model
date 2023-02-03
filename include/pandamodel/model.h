// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#pragma once

#include <array>
#include <memory>
#include <Eigen/Core>

#include "pandamodel/defaults.h"

/**
 * @file model.h
 * Contains model library types.
 */

namespace panda_model {

/**
 * Enumerates the seven joints, the flange, and the end effector of a robot.
 */
enum class Frame {
  kJoint1,
  kJoint2,
  kJoint3,
  kJoint4,
  kJoint5,
  kJoint6,
  kJoint7,
  kFlange,
  kEndEffector,
  kStiffness
};

/**
 * Post-increments the given Frame by one.
 *
 * For example, Frame::kJoint2++ results in Frame::kJoint3.
 *
 * @param[in] frame Frame to increment.
 *
 * @return Original Frame.
 */
Frame operator++(Frame& frame, int /* dummy */) noexcept;

class ModelLibrary;
// class Network;

/**
 * Calculates poses of joints and dynamic properties of the robot.
 */
class Model {
 public:

  explicit Model(const std::string &path);

  /**
   * Move-constructs a new Model instance.
   *
   * @param[in] model Other Model instance.
   */
  Model(Model&& model) noexcept;

  /**
   * Move-assigns this Model from another Model instance.
   *
   * @param[in] model Other Model instance.
   *
   * @return Model instance.
   */
  Model& operator=(Model&& model) noexcept;

  /**
   * Unloads the model library.
   */
  ~Model() noexcept;

  /**
   * Gets the 4x4 pose matrix for the given frame in base frame.
   *
   * The pose is represented as a 4x4 matrix in column-major format.
   *
   * @param[in] frame The desired frame.
   * @param[in] q Joint position.
   * @param[in] F_T_EE End effector in flange frame.
   * @param[in] EE_T_K Stiffness frame K in the end effector frame.
   *
   * @return Vectorized 4x4 pose matrix, column-major.
   */
  Eigen::Matrix4d pose(
      Frame frame,
      const Eigen::Matrix<double, 7, 1>& q,
      const Eigen::Matrix4d& F_T_EE = Defaults::F_T_EE,
      const Eigen::Matrix4d& EE_T_K = Defaults::EE_T_K)
      const;

  /**
   * Gets the 6x7 Jacobian for the given frame, relative to that frame.
   *
   * The Jacobian is represented as a 6x7 matrix in column-major format.
   *
   * @param[in] frame The desired frame.
   * @param[in] q Joint position.
   * @param[in] F_T_EE End effector in flange frame.
   * @param[in] EE_T_K Stiffness frame K in the end effector frame.
   *
   * @return Vectorized 6x7 Jacobian, column-major.
   */
  Eigen::Matrix<double, 6, 7> bodyJacobian(
      Frame frame,
      const Eigen::Matrix<double, 7, 1>& q,
      const Eigen::Matrix4d& F_T_EE = Defaults::F_T_EE,
      const Eigen::Matrix4d& EE_T_K = Defaults::EE_T_K)
      const;

  /**
   * Gets the 6x7 Jacobian for the given joint relative to the base frame.
   *
   * The Jacobian is represented as a 6x7 matrix in column-major format.
   *
   * @param[in] frame The desired frame.
   * @param[in] q Joint position.
   * @param[in] F_T_EE End effector in flange frame.
   * @param[in] EE_T_K Stiffness frame K in the end effector frame.
   *
   * @return Vectorized 6x7 Jacobian, column-major.
   */
  Eigen::Matrix<double, 6, 7> zeroJacobian(
      Frame frame,
      const Eigen::Matrix<double, 7, 1>& q,
      const Eigen::Matrix4d& F_T_EE = Defaults::EE_T_K,
      const Eigen::Matrix4d& EE_T_K = Defaults::EE_T_K)
      const;

  /**
   * Calculates the 7x7 mass matrix. Unit: \f$[kg \times m^2]\f$.
   *
   * @param[in] q Joint position.
   * @param[in] I_total Inertia of the attached total load including end effector, relative to
   * center of mass, given as vectorized 3x3 column-major matrix. Unit: \f$[kg \times m^2]\f$.
   * @param[in] m_total Weight of the attached total load including end effector.
   * Unit: \f$[kg]\f$.
   * @param[in] F_x_Ctotal Translation from flange to center of mass of the attached total load.
   * Unit: \f$[m]\f$.
   *
   * @return Vectorized 7x7 mass matrix, column-major.
   */
  Eigen::Matrix<double, 7, 7> mass(
      const Eigen::Matrix<double, 7, 1>& q,
      const Eigen::Matrix3d& I_total = Defaults::I_total,
      double m_total = Defaults::m_total,
      const Eigen::Vector3d& F_x_Ctotal = Defaults::F_x_Ctotal)
      const noexcept;

  /**
   * Calculates the Coriolis force vector (state-space equation): \f$ c= C \times
   * dq\f$, in \f$[Nm]\f$.
   *
   * @param[in] q Joint position.
   * @param[in] dq Joint velocity.
   * @param[in] I_total Inertia of the attached total load including end effector, relative to
   * center of mass, given as vectorized 3x3 column-major matrix. Unit: \f$[kg \times m^2]\f$.
   * @param[in] m_total Weight of the attached total load including end effector.
   * Unit: \f$[kg]\f$.
   * @param[in] F_x_Ctotal Translation from flange to center of mass of the attached total load.
   * Unit: \f$[m]\f$.
   *
   * @return Coriolis force vector.
   */
  Eigen::Matrix<double, 7, 1> coriolis(
      const Eigen::Matrix<double, 7, 1>& q,
      const Eigen::Matrix<double, 7, 1>& dq,
      const Eigen::Matrix3d& I_total = Defaults::I_total,
      double m_total = Defaults::m_total,
      const Eigen::Vector3d& F_x_Ctotal = Defaults::F_x_Ctotal)
      const noexcept;

  /**
   * Calculates the gravity vector. Unit: \f$[Nm]\f$.
   *
   * @param[in] q Joint position.
   * @param[in] m_total Weight of the attached total load including end effector.
   * Unit: \f$[kg]\f$.
   * @param[in] F_x_Ctotal Translation from flange to center of mass of the attached total load.
   * Unit: \f$[m]\f$.
   * @param[in] gravity_earth Earth's gravity vector. Unit: \f$\frac{m}{s^2}\f$.
   * Default to {0.0, 0.0, -9.81}.
   *
   * @return Gravity vector.
   */
  Eigen::Matrix<double, 7, 1> gravity(
      const Eigen::Matrix<double, 7, 1>& q,
      double m_total = Defaults::m_total,
      const Eigen::Vector3d& F_x_Ctotal = Defaults::F_x_Ctotal,
      const Eigen::Vector3d& gravity_earth = {0., 0., -9.81}) const noexcept;

  /// @cond DO_NOT_DOCUMENT
  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;
  /// @endcond

 private:
  std::unique_ptr<ModelLibrary> library_;
};

}  // namespace panda_model
