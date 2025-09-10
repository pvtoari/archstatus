project('archstatus', 'c',
    version: run_command('cat', join_paths(meson.project_source_root(), 'VERSION'), check: true).stdout().strip(),
    default_options : ['c_std=gnu11', 'buildtype=release']
)

inc = include_directories('include')
cjson_dep = dependency('libcjson', required: true)
libcurl_dep = dependency('libcurl', required: true)

executable('archstatus', 
    'src/main.c', 'src/archstatus.c', 'src/front.c', 'src/logo.c', 'src/sanitize.c',
    dependencies: [cjson_dep, libcurl_dep],
    include_directories: inc,
    install: true
)
