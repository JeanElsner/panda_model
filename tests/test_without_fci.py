import os
import tempfile
import subprocess
import unittest
from panda_model import download


class TestWithoutFCI(unittest.TestCase):

  def test_imports(self):
    try:
      from panda_model import download_library, Model, Architecture, OperatingSystem
    except ImportError as e:
      self.fail(f'Failed to import module: {e}')

  def test_command_line(self):
    self.assertRaisesRegex(RuntimeError, 'Host not found', download.run)
