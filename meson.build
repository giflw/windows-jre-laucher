project('windows-jre-laucher', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

conan_pkgs = {
    'spdlog':'spdlog/1.11.0@', # <- Must contain @, otherwise Conan will think it is a path
}
deps = []

conan = find_program('conan', required: true)
conan_buildtype = 'Release'

foreach pkg_name, conan_ref : conan_pkgs
    module_path = meson.current_build_dir() / 'conan-cmake' / pkg_name
    run_command(conan,
                'install', conan_ref,
                '--install-folder',module_path,
                '--generator', 'cmake_find_package',
                '--settings:build', 'build_type=' + conan_buildtype,
                check: true)
    deps += dependency(pkg_name, method: 'cmake', cmake_module_path: module_path)
endforeach


run_command('conan', 'install', '--install-folder',
    meson.build_root(), meson.source_root(),
    '--build=missing', check: true)

executable('launcher',
           'launcher.cpp',
           dependencies : deps,
           install : false)


