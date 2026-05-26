add_rules('mode.debug', 'mode.release')

set_project('filer')
set_version('0.1.0')
set_languages('c++26')

add_rules('plugin.compile_commands.autoupdate', {
  outputdir = 'build'
})

package('arc')
  set_kind('library')
  set_homepage('https://github.com/Hato1125/arc')
  set_description('Arc core UI framework')

  add_urls('git@github.com:Hato1125/arc.git')
  add_deps('libsdl3', 'nanovg', 'libepoxy')
  add_includedirs('include/src')

  on_install(function(package)
    import('package.tools.xmake').install(package, {}, {
      target = 'arc'
    })
    os.cp('src', package:installdir('include'))
  end)

  on_test(function(package)
    assert(package:has_cxxincludes('arc.hh', {
      configs = {
        languages = 'c++26'
      }
    }))
  end)
package_end()

add_requires('arc main', {
  debug = is_mode('debug')
})

target('filer')
  set_kind('binary')
  set_rundir('$(projectdir)')
  add_files('src/**.cc')
  add_packages('arc', {
    public = true
  })
