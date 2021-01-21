project(
  'goocanvas', 'c',
  version: '4.0.0',
  license: 'LGPL',
  default_options: 'buildtype=debugoptimized',
  meson_version: '>= 0.49.0',
)

goocanvas_name = meson.project_name()

goocanvas_version = meson.project_version()
version_array = goocanvas_version.split('.')
goocanvas_major_version = version_array[0].to_int()
goocanvas_minor_version = version_array[1].to_int()
goocanvas_micro_version = version_array[2].to_int()

# If the source code has changed at all, increment goocanvasui_revision
# If any interfaces have been added, removed, or changed, increment goocanvasui_current, and set goocanvasui_revision to 0.
# If any interfaces have been added since the last public release, then increment goocanvasui_age.
# If any interfaces have been removed since the last public release, then set goocanvasui_age to 0.
# Reference: http://www.gnu.org/software/libtool/manual/libtool.html#Versioning

goocanvas_revision = 0
goocanvas_current = 14
goocanvas_age = 0
goocanvas_current_minus_age = goocanvas_current - goocanvas_age
goocanvas_libversion = '@0@.@1@.@2@'.format(goocanvas_current_minus_age, goocanvas_age, goocanvas_revision)

goocanvas_api_name = meson.project_name() + goocanvas_major_version.to_string()
goocanvas_api_version = '@0@.@1@'.format(goocanvas_major_version, goocanvas_minor_version)
goocanvas_api_prefix = 'Goo'

goocanvas_api_ns = 'GooCanvas'

goocanvas_prefix = get_option('prefix')
goocanvas_includedir = get_option('includedir')
goocanvas_libdir = get_option('libdir')

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')
pymod = import('python')

source_root = meson.current_source_dir()
build_root = meson.current_build_dir()

its_dir = source_root / 'data/gettext'
po_dir = source_root / 'po'

top_inc = include_directories('.')

cc = meson.get_compiler('c')

config_h = configuration_data()
config_h.set_quoted('GETTEXT_PACKAGE', goocanvas_api_name)

# Check required libraries
cairo_dep = dependency('cairo', version: '>= 1.10.0')
glib_dep = dependency('glib-2.0', version: '>= 2.28.0')
gtk_dep = dependency('gtk4', version: '>= 3.89.0')

m_dep = cc.find_library('m')

enable_introspection = dependency('gobject-introspection-1.0', version: '>= 0.6.7', required: get_option('introspection')).found()

subdir('po')
subdir('src')
subdir('demo')

py_installation = pymod.find_installation('python3', required: get_option('python'))
pygobject_dep = dependency('pygobject-3.0', version: '>= 2.90.4', required: get_option('python'))
if py_installation.found() and pygobject_dep.found()
  subdir('bindings/python')
endif

enable_gtk_doc = get_option('gtk_doc')
if enable_gtk_doc
  subdir('docs')
endif

configure_file(
  output: 'config.h',
  configuration: config_h,
)
