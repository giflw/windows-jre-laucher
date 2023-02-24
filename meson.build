project('windows-jre-laucher', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

conan = find_program('conan', required: true)
run_command(conan, 'install',
    '--output-folder', meson.build_root(), meson.source_root(),
    '--build=missing',
    check: true)

deps = [
    dependency('spdlog', version : '1.11.0')
]

launcher = executable('launcher',
           'src/launcher.cpp',
           dependencies : deps,
           install : false)

test('Run launcher', launcher)


