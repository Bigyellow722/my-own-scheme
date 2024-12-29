project('scheme', 'c')

if get_option('enable_debug')
    add_project_arguments('-ggdb', language: 'c')
endif


lib = library('baselib', 'log.c', 'memory.c', 'darray.c')

executable('scheme', 'main.c', link_with : lib)
