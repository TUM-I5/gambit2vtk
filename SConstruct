#! /usr/bin/python

# @file
#  This file is part of gambit2vtk
#
#  For conditions of distribution and use, please see the copyright
#  notice in the file 'COPYING' at the root directory of this package
#  and the copyright notice at https://github.com/TUM-I5/gambit2vtk
# 
# @copyright 2012 Technische Universitaet Muenchen
# @author Sebastian Rettenberger <rettenbs@in.tum.de>

import os

#
# set up possible variables
#
vars = Variables()
vars.AddVariables(
    EnumVariable( 'mode', 'build mode', 'release', allowed_values=('debug', 'release')),
    PathVariable( 'prefix', 'install prefix', '/usr/local', PathVariable.PathAccept )
)

# set environment
env = Environment(ENV = os.environ,
    variables = vars)

# generate help text
Help(vars.GenerateHelpText(env))

# handle unknown, maybe misspelled variables
unknownVariables = vars.UnknownVariables()

# exit in the case of unknown variables
if unknownVariables:
  print "*** The following build variables are unknown:", unknownVariables.keys()
  Exit(1)

#
# Set compiler flags
#

# eclipse specific flag
env.Append(CCFLAGS=['-fmessage-length=0'])

# other default flags
env.Append(CCFLAGS=['-Wall'])

# switch between debug/release
if env['mode'] == 'debug':
    env.Append(CCFLAGS=['-O0', '-g3'])
else: # env['mode'] == release
    env.Append(CCFLAGS=['-O3'])

# Find libxml2
env.ParseConfig("xml2-config --cflags --libs")

# Add the src directory to the include path
env.Append(CPPPATH=['src'])

# program name
program_name = 'gambit2vtk'

# build directory
build_dir = os.path.join(env['mode'], 'build')

# all directories containing sources ('src/' will be prepended)
src_dirs = [
    '.',
    'config',
    'converter',
    'io',
    'tools'
] 

# get the src-code files
env.src_files = []
for dir in src_dirs:
    Export('env')
    SConscript(os.path.join('src', dir, 'SConscript'),
        variant_dir = os.path.join(build_dir, dir),
        duplicate = 0)
    Import('env')

# build the program
program = env.Program(os.path.join(env['mode'], program_name), env.src_files)

#
# install the program
#

# directory for executables
bin = os.path.join(env['prefix'], 'bin')

# install the executable
env.Install(bin, program)

# let "scons install" install all files
env.Alias('install', [bin])