ent_conf = {
  'PACKAGE': goocanvas_name,
  'PACKAGE_BUGREPORT': '',
  'PACKAGE_NAME': goocanvas_name,
  'PACKAGE_STRING': '@0@ @1@'.format(goocanvas_name, goocanvas_version),
  'PACKAGE_TARNAME': goocanvas_name,
  'PACKAGE_URL': '',
  'PACKAGE_VERSION': goocanvas_version,
}

configure_file(
  input: 'gtkdocentities.ent.in',
  output: '@BASENAME@',
  configuration: ent_conf,
)
