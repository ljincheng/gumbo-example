project('bebe-gumbo','c')


dep_qumbo_query=dependency('libgq',required: false)
pkg = import('pkgconfig')
dep_gumbo=dependency('gumbo')
dep_curl=dependency('libcurl')


bebeApp_dep=[dep_gumbo,dep_curl,dep_qumbo_query]

executable('bebeApp','main.c',dependencies:bebeApp_dep)
