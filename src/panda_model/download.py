#!/usr/bin/python3

import argparse

import panda_model

arch_choices = ['x64', 'x86', 'arm64', 'arm']
os_choices = ['linux', 'windows']
parser = argparse.ArgumentParser(
    description=
    'Downloads model libraries from the Franka Emika master control unit.')
parser.add_argument('hostname',
                    type=str,
                    help='Hostname/IP of the master controller')
parser.add_argument('--path',
                    '-p',
                    type=str,
                    help='Download library into this path',
                    default='./')
parser.add_argument('--arch',
                    '-a',
                    choices=arch_choices,
                    help='Processor architecture',
                    default='x64')
parser.add_argument('--os',
                    choices=os_choices,
                    help='Operating System',
                    default='linux')
parser.add_argument(
    '--version',
    '-v',
    type=int,
    metavar='N',
    help='Robot system version i.e. version of the master controller',
    default=4)
args = parser.parse_args()
panda_model.download_library(args.hostname,
                             path=args.path,
                             architecture=getattr(panda_model.Architecture,
                                                  args.arch),
                             operating_system=getattr(
                                 panda_model.OperatingSystem, args.os),
                             version=args.version)
