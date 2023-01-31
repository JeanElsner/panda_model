""" Simple example demonstrating `Model` class. """
import numpy as np

from panda_model import Model, Frame

# Path to shared library, can be relative
PATH = './libfrankamodel.linux_x64.so'

model = Model(PATH)

# Starting position
q_0 = np.array([0, -np.pi / 4, 0, -3 * np.pi / 4, 0, np.pi / 2, np.pi / 4])

# Add small perturbations and print gravity compensation
for i in range(10):
  print(model.gravity(q_0 + np.random.random(7) * 0.1, 0, [0, 0, 0]))

# Get the zero jacobian for end-effector frame
frame = Frame.kEndEffector
F_T_EE = np.eye(4).flatten()
EE_T_K = np.eye(4).flatten()
print(model.zero_jacobian(frame, q_0, F_T_EE, EE_T_K))
