add_rules('mode.debug', 'mode.release')
set_project('filer')
set_version('0.1.0')
set_languages('c++26')

add_requires('hash-library')
add_requires('sdbus-c++')


includes('../arc')

add_rules('plugin.compile_commands.autoupdate', {
  outputdir = 'build'
})

target('filer')
  set_kind('binary')
  set_rundir('$(projectdir)')
  add_rules('wayland.client')
  add_files('src/**.cc')
  add_deps('arc')
  add_syslinks("epoxy", "ssl", "crypto")
  add_packages('hash-library', "sdbus-c++")
