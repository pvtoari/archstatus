project('archstatus', 'c',
    default_options : ['c_std=gnu11', 'buildtype=release']
)

inc = include_directories('include')
cjson_dep = dependency('libcjson', required: true)
libcurl_dep = dependency('libcurl', required: true)

executable('archstatus', 
'src/main.c', 'src/archstatus.c',
    dependencies: [cjson_dep, libcurl_dep],
    include_directories: inc,
    install: true
)
