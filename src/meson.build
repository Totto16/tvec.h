inc_dirs += include_directories('.')

include_dir = get_option('prefix') / get_option('includedir')

install_headers(
    files('tvec.h'),
    install_dir: include_dir / 'tvec',
    preserve_path: true,
)

