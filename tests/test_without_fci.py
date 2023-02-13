import os
import tempfile
import subprocess
import unittest


class TestWithoutFCI(unittest.TestCase):

  def test_imports(self):
    try:
      from panda_model import download_library, Model, Architecture, OperatingSystem
    except ImportError as e:
      self.fail(f'Failed to import module: {e}')

  def test_command_line(self):
    try:
      result = subprocess.run(['panda_model_download', '--help'],
                              stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE,
                              check=True)
      self.assertEqual(result.returncode, 0)
    except subprocess.CalledProcessError as e:
      self.fail(f"Command failed with return code {e.returncode}: {e.stderr}")
