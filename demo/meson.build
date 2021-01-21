tests = [
  ['generic-position-demo', {'sources': files('generic-position-demo.c')}],
  ['mv-generic-position-demo', {'sources': files('mv-generic-position-demo.c')}],
  ['mv-scalability-demo', {'sources': files('mv-scalability-demo.c')}],
  ['mv-simple-demo', {'sources': files('mv-simple-demo.c')}],
  ['mv-table-demo', {'sources': files('mv-table-demo.c')}],
  ['scalability-demo', {'sources': files('scalability-demo.c')}],
  ['simple-demo', {'sources': files('simple-demo.c')}],
  ['table-demo', {'sources': files('table-demo.c')}],
  ['units-demo', {'sources': files('units-demo.c')}],
  ['widgets-demo', {'sources': files('widgets-demo.c')}],
]

sources = files(
  'demo-animation.c',
  'demo-arrowhead.c',
  'demo.c',
  'demo-clipping.c',
  'demo-events.c',
  'demo-features.c',
  'demo-fifteen.c',
  'demo-focus.c',
  'demo-grabs.c',
  'demo-item.c',
  'demo-large-items.c',
  'demo-large-line.c',
  'demo-large-rect.c',
  'demo-paths.c',
  'demo-scalability.c',
  'demo-table.c',
)

tests += [['demo', {'sources': sources}]]

sources = files(
  'mv-demo-animation.c',
  'mv-demo-arrowhead.c',
  'mv-demo.c',
  'mv-demo-clipping.c',
  'mv-demo-events.c',
  'mv-demo-features.c',
  'mv-demo-fifteen.c',
  'mv-demo-focus.c',
  'mv-demo-grabs.c',
  'mv-demo-paths.c',
  'mv-demo-scalability.c',
  'mv-demo-table.c',
)

tests += [['mv-demo', {'sources': sources}]]

deps = [
  libgoocanvas_dep,
  m_dep,
]

foreach test: tests
  executable(
    test[0],
    include_directories: top_inc,
    dependencies: deps,
    kwargs: test[1],
  )
endforeach
