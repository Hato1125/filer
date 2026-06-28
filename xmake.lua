add_rules('mode.debug', 'mode.release')
set_project('filer')
set_version('0.1.0')
set_languages('c++26')

add_requires('hash-library')
add_requires('libsdl3')
add_requires('nanovg')
add_requires('fontconfig')
add_requires('wayland')
add_requires('sdbus-c++')
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

local arc_dir = path.join(os.projectdir(), "..", "arc")
local arc_src = path.join(arc_dir, "src")

-- Generate client-side glue for the wayland protocols under arc/protocols/*.xml.
rule('wayland.client')
  set_extensions('.xml')
  on_config(function (target)
    import('lib.detect.find_tool')

    local scanner = assert(
      find_tool('wayland-scanner'),
      'wayland-scanner not found! please install wayland'
    )

    local gendir = path.join(target:autogendir(), 'rules', 'wayland.client')
    os.mkdir(gendir)
    target:add('includedirs', gendir)

    local batch = target:sourcebatches()['wayland.client']
    if not batch or not batch.sourcefiles then
      return
    end

    for _, xml in ipairs(batch.sourcefiles) do
      local base = path.basename(xml)
      local header = path.join(gendir, base .. '-client-protocol.h')
      local code = path.join(gendir, base .. '-client-protocol.c')

      if not os.isfile(header) or os.mtime(xml) > os.mtime(header) then
        os.vrunv(scanner.program, { 'client-header', xml, header })
        -- `namespace` is a C++ keyword but a valid C identifier that
        -- wlr-layer-shell uses as an argument name; rename it so the
        -- generated header can be included from C++.
        local content = io.readfile(header)
        content = content:gsub('([^%w_])namespace([^%w_])', '%1_namespace%2')
        io.writefile(header, content)
      end
      if not os.isfile(code) or os.mtime(xml) > os.mtime(code) then
        os.vrunv(scanner.program, { 'private-code', xml, code })
      end

      target:add('files', code, { always_added = true })
    end
  end)

target('filer')
  set_kind('binary')
  set_rundir('$(projectdir)')
  add_rules('wayland.client')
  add_files('src/**.cc')

  add_files(path.join(arc_src, "**.cc"))
  add_files(path.join(arc_dir, "protocols", "*.xml"))
  add_includedirs(arc_src)
  add_syslinks("epoxy", "ssl", "crypto")
  add_packages('hash-library', "libsdl3", "nanovg", "fontconfig", "glad", "wayland", "sdbus-c++")
