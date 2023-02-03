import os
import unittest


class TestEnviron(unittest.TestCase):

  def test_PATH(self):
    path = os.environ.get('PANDA_MODEL_PATH')
    self.assertIsNotNone(
        path,
        'PANDA_MODEL_PATH must be set for tests.',
    )
    self.assertTrue(
        os.path.isfile(path),
        'PANDA_MODEL_PATH must be set to a valid shared library' +
        ' for this platform for the tests to run.',
    )

  def test_PANDA_MODEL_VER(self):
    self.assertIsNotNone(
        os.environ.get("PANDA_MODEL_VER"),
        'PANDA_MODEL_VER must be set to the version of the' +
        ' connected master control unit for all tests to run.',
    )

  def test_PANDA_MODEL_HOST(self):
    self.assertIsNotNone(
        os.environ.get('PANDA_MODEL_HOST'),
        'PANDA_MODEL_HOST must be set to the hostname of the' +
        ' connected master control unit for all tests to run.')
