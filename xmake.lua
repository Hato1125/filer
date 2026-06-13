add_rules('mode.debug', 'mode.release')
set_project('filer')
set_version('0.1.0')
set_languages('c++26')

add_requires('hash-library')
add_requires('libsdl3')
add_requires('nanovg')
add_requires('fontconfig')
add_requires('glad', {
  configs = {
    profile = 'core',
    api = 'gl=3.3',
    spec = 'gl',
  },
})

add_rules('plugin.compile_commands.autoupdate', {
  outputdir = 'build'
})

local arc_src = path.join(os.projectdir(), "..", "arc", "src")

target('filer')
  set_kind('binary')
  set_rundir('$(projectdir)')
  add_files('src/**.cc')

  add_files(path.join(arc_src, "**.cc"))
  add_includedirs(arc_src)
  add_syslinks("epoxy", "ssl", "crypto")
  add_packages('hash-library', "libsdl3", "nanovg", "fontconfig", "glad")
