#include <pandamodel/model.h>

#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>

int main() {
  // Instantiate Model with shared library
  const char* path = std::getenv("PANDA_MODEL_PATH");
  if (path == NULL) {
    std::cerr << "PANDA_MODEL_PATH not set." << std::endl;
    return -1;
  }
  auto model = new panda_model::Model(path);

  // Call the member functions with state vector arguments
  Eigen::Matrix<double, 7, 1> q = {0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4};
  auto gravity = model->gravity(q);

  // Print the result
  std::cout << gravity << std::endl;
  std::cout << std::endl;
  return 0;
}