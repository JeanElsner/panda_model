import os
import tempfile
import unittest

from panda_model import Architecture, OperatingSystem, download_library


class TestDownloadLibrary(unittest.TestCase):

  def setUp(self):
    self.host = os.environ.get('PANDA_MODEL_HOST')
    self.ver = int(os.environ.get('PANDA_MODEL_VER'))
    self.path = tempfile.gettempdir()

  def download(self, arch, osys, filename):
    path = download_library(self.host,
                            path=self.path,
                            version=self.ver,
                            architecture=arch,
                            operating_system=osys)
    predicted_path = os.path.join(self.path, filename)
    self.assertEqual(path, predicted_path)
    self.assertTrue(os.path.isfile(path))
    os.remove(path)

  def test_windows_x64(self):
    self.download(Architecture.x64, OperatingSystem.windows,
                  'libfrankamodel.win_x64.so')

  def test_windows_x86(self):
    self.download(Architecture.x86, OperatingSystem.windows,
                  'libfrankamodel.win_x86.so')

  def test_linux_x64(self):
    self.download(Architecture.x64, OperatingSystem.linux,
                  'libfrankamodel.linux_x64.so')

  def test_linux_x86(self):
    self.download(Architecture.x86, OperatingSystem.linux,
                  'libfrankamodel.linux_x86.so')

  def test_arm64(self):
    self.download(Architecture.arm64, OperatingSystem.linux,
                  'libfrankamodel.linux_arm64.so')

  def test_arm(self):
    self.download(Architecture.arm, OperatingSystem.linux,
                  'libfrankamodel.linux_arm.so')
