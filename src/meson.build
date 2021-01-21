src_inc = include_directories('.')

libname = '@0@-@1@'.format(goocanvas_name, goocanvas_api_version)

headers = files(
  'goocanvasellipse.h',
  'goocanvasgrid.h',
  'goocanvasgroup.h',
  'goocanvas.h',
  'goocanvasimage.h',
  'goocanvasitem.h',
  'goocanvasitemmodel.h',
  'goocanvasitemsimple.h',
  'goocanvaspath.h',
  'goocanvaspolyline.h',
  'goocanvasrect.h',
  'goocanvasstyle.h',
  'goocanvastable.h',
  'goocanvastext.h',
  'goocanvasutils.h',
  'goocanvaswidget.h',
)

install_headers(
  headers,
  install_dir: goocanvas_includedir / libname,
)

sources = files(
  'goocanvasatk.c',
  'goocanvas.c',
  'goocanvasellipse.c',
  'goocanvasgrid.c',
  'goocanvasgroup.c',
  'goocanvasimage.c',
  'goocanvasitem.c',
  'goocanvasitemmodel.c',
  'goocanvasitemsimple.c',
  'goocanvaspath.c',
  'goocanvaspolyline.c',
  'goocanvasrect.c',
  'goocanvasstyle.c',
  'goocanvastable.c',
  'goocanvastext.c',
  'goocanvasutils.c',
  'goocanvaswidget.c',
)


marshal = 'goocanvasmarshal'

sources += gnome.genmarshal(
  marshal,
  sources: marshal + '.list',
  prefix: 'goo_canvas_marshal',
  install_header: true,
  install_dir: goocanvas_includedir / libname,
)

enum_types = 'goocanvasenumtypes'

enum_sources = gnome.mkenums(
  enum_types,
  sources: headers,
  h_template: enum_types + '.h.template',
  c_template: enum_types + '.c.template',
  install_header: true,
  install_dir: goocanvas_includedir / libname,
)

private_deps = [
  glib_dep,
  m_dep,
]

deps = [
  cairo_dep,
  gtk_dep,
]

libgoocanvas = shared_library(
  libname,
  version: goocanvas_libversion,
  sources: sources + enum_sources,
  include_directories: top_inc,
  dependencies: deps + private_deps,
  c_args: '-DG_LOG_DOMAIN="GooCanvas"',
  install: true,
  install_rpath: goocanvas_libdir,
)

libgoocanvas_dep = declare_dependency(
  sources: enum_sources[1],
  include_directories: src_inc,
  dependencies: gtk_dep,
  link_with: libgoocanvas,
)

pkg.generate(
  libraries: libgoocanvas,
  name: goocanvas_api_ns,
  description: 'A GTK+ canvas widget using cairo',
  filebase: libname,
  subdirs: libname,
  requires: deps,
  variables: 'exec_prefix=${prefix}',
)

if enable_introspection
  libgoocanvas_gir = gnome.generate_gir(
    libgoocanvas,
    sources: sources + enum_sources + headers,
    includes: 'Gtk-3.0',
    nsversion: goocanvas_api_version,
    namespace: goocanvas_api_ns,
    identifier_prefix: goocanvas_api_prefix,
    symbol_prefix: goocanvas_api_prefix.to_lower(),
    export_packages: '@0@-@1@'.format(goocanvas_api_ns, goocanvas_api_version),
    install: true,
  )
endif
