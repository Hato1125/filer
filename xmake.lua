add_rules('mode.debug', 'mode.release')

set_project('filer')
set_version('0.1.0')
set_languages('c++26')

add_rules('plugin.compile_commands.autoupdate', {
  outputdir = 'build'
})

-- Use the local arc checkout directly: arc is compiled from its live source as
-- part of this build, so edits in ../arc are picked up on the next `xmake build`
-- with no package reinstall. (Was a git package: git@github.com:Hato1125/arc.git)
includes('../arc')

target('filer')
  set_kind('binary')
  set_rundir('$(projectdir)')
  add_files('src/**.cc')
  add_deps('arc')
