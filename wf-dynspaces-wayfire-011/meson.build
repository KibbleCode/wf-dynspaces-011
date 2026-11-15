project('wf-dynspaces-wayfire-011-experimental', 'cpp',
  version: '0.1.0',
  default_options: ['cpp_std=c++17', 'warning_level=2'])

wayfire = dependency('wayfire', version: '>=0.11.0')

# Build-only: no installation target. Do not load this into Wayfire yet.
shared_module('dynspaces011', 'dynspaces011.cpp',
  dependencies: [wayfire],
  install: false)
