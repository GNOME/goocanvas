subdir('xml')

# FIXME: the file name has not been updated
doc_module_basename = meson.project_name() + (goocanvas_major_version - 1).to_string()

doc_module_dir = gnome.gtkdoc_html_dir(goocanvas_api_name)

private_headers = [
  'goocanvasmarshal.h',
  'goocanvasprivate.h',
]

expand_content_files = [
  'architecture.xml',
  'coordinates.xml',
  'creating-items.xml',
  'model-view-canvas.xml',
  'overview.xml',
  'simple-canvas.xml',
  'wysiwyg.xml',
]

gnome.gtkdoc(
  goocanvas_api_name,
  main_xml: doc_module_basename + '-docs.sgml',
  module_version: goocanvas_major_version.to_string(),
  src_dir: src_inc,
  ignore_headers: private_headers,
  include_directories: top_inc,
  dependencies: libgoocanvas_dep,
  gobject_typesfile: files(doc_module_basename + '.types'),
  fixxref_args: '--html-dir=' + (goocanvas_prefix / doc_module_dir),
  scan_args: '--rebuild-types',
  scanobjs_args: '--query-child-properties=goo_canvas_query_child_properties',
  expand_content_files: expand_content_files,
  install: true,
  install_dir: doc_module_dir,
)
