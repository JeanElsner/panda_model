#include <pandamodel/model.h>

#include <iostream>
#include <string>
#include <math.h>

int main() {
  // Instantiate Model with shared library
  auto model = new panda_model::Model("./libfrankamodel.linux_x64.so");

  // Call the member functions with state vector arguments
  std::array<double, 7> q = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
  std::array<double, 3> load = {0, 0, 0};
  auto gravity = model->gravity(q, 0, load);

  // Print the result
  for (auto g : gravity) {
    std::cout << g << " ";
  }
  std::cout << std::endl;
  return 0;
}