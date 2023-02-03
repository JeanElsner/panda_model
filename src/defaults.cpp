#include "pandamodel/defaults.h"

const Eigen::Vector3d Defaults::gravity_earth = {0., 0., -9.81};
const Eigen::Vector3d Defaults::F_x_Ctotal = {-0.01, 0, 0.03};
const double I_total_data[9] = {0.001, 0, 0, 0, 0.0025, 0, 0, 0, 0.0017};
const Eigen::Matrix3d Defaults::I_total = Eigen::Matrix3d(I_total_data);
const Eigen::Matrix4d Defaults::EE_T_K = Eigen::Matrix4d::Identity();
const double F_T_EE_data[16] = {
    0.7071, -0.7071, 0, 0, 0.7071, 0.7071, 0, 0, 0, 0, 1, 0, 0, 0, 0.1034, 1,
};
const Eigen::Matrix4d Defaults::F_T_EE = Eigen::Matrix4d(F_T_EE_data);
