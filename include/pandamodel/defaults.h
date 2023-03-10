#pragma once

#include <Eigen/Core>

class Defaults {
 public:
  static const Eigen::Vector3d gravity_earth;
  static const Eigen::Vector3d F_x_Ctotal;
  static const double m_total;
  static const Eigen::Matrix3d I_total;
  static const Eigen::Matrix4d EE_T_K;
  static const Eigen::Matrix4d F_T_EE;
};
