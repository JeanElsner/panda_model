"""
Simple example demonstrating the `Model` class.
Set the environment variable PANDA_MODEL_PATH
to the to your shared library file to run this example.
"""
import os

import numpy as np

from panda_model import Model, Defaults, Frame

# Path to shared library, can be relative
PATH = os.environ.get('PANDA_MODEL_PATH')

model = Model(os.path.join( os.path.dirname(os.path.abspath(__file__)), PATH))

# Starting position
q_0 = np.array([0, -np.pi / 4, 0, -3 * np.pi / 4, 0, np.pi / 2, np.pi / 4])

# Add small perturbations and print gravity compensation
for i in range(10):
  print(model.gravity(q_0 + np.random.random(7) * 0.1))

f = Frame.kEndEffector
# Uses default values
print(model.pose(f, q_0))
# Equivalent
print(model.pose(f, q_0, Defaults.F_T_EE, Defaults.EE_T_K))
# Without gripper
print(model.pose(f, q_0, np.eye(4), np.eye(4)))
