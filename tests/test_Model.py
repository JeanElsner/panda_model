import os
import unittest

import numpy as np
import numpy.testing as nt

from panda_model import Frame, Model

from .data import (BODY_JACOBIAN, CORIOLIS, DQ, GRAVITY, MASS, POSE,
                   ZERO_JACOBIAN, Q)


class TestModel(unittest.TestCase):

  def setUp(self):
    self.model = Model(os.environ.get('PANDA_MODEL_PATH'))
    self.atol = 10e-5

  def test_gravity(self):
    computed_gravity = self.model.gravity(Q)
    nt.assert_allclose(GRAVITY, computed_gravity, atol=self.atol)

  def test_coriolis(self):
    computed_coriolis = self.model.coriolis(Q, np.zeros(7))
    nt.assert_allclose(CORIOLIS, computed_coriolis, atol=self.atol)

  def test_mass(self):
    computed_mass = self.model.mass(Q)
    nt.assert_allclose(MASS, computed_mass, atol=self.atol)

  def test_pose(self):
    computed_pose = self.model.pose(Frame.kEndEffector, Q)
    nt.assert_allclose(POSE, computed_pose, atol=self.atol)

  def test_body_jacobian(self):
    computed_body_jacobian = self.model.body_jacobian(Frame.kEndEffector, Q)
    nt.assert_allclose(BODY_JACOBIAN, computed_body_jacobian, atol=self.atol)

  def test_zero_jacobian(self):
    computed_zero_jacobian = self.model.zero_jacobian(Frame.kEndEffector, Q)
    nt.assert_allclose(ZERO_JACOBIAN, computed_zero_jacobian, atol=self.atol)
