project(
  'LevelTSDB',
  'cpp',
  default_options: [
    'warning_level=3',
    'werror=false',
    'cpp_std=gnu++20',
    'cpp_eh=none'
  ]
)

cxx = meson.get_compiler('cpp')

cxx_flags = [
        '-Warray-bounds',     # clang
        '-Warray-bounds=2',
        '-Wdate-time',
        '-Wendif-labels',
        '-Werror=format=2',
        '-Werror=format-signedness',
        '-Werror=implicit-function-declaration',
        '-Werror=implicit-int',
        '-Werror=incompatible-pointer-types',
        '-Werror=int-conversion',
        '-Werror=missing-declarations',
        '-Werror=missing-prototypes',
        '-Werror=overflow',
        '-Werror=override-init',
        '-Werror=return-type',
        '-Werror=shift-count-overflow',
        '-Werror=shift-overflow=2',
        '-Werror=undef',
        '-Wextra-semi',
        '-Wextra-semi-stmt',
        '-Wfloat-equal',
        '-Wimplicit-fallthrough=5',
        '-Winit-self',
        '-Wlogical-op',
        '-Wmissing-include-dirs',
        '-Wmissing-noreturn',
        '-Wnested-externs',
        '-Wold-style-definition',
        '-Wpointer-arith',
        '-Wredundant-decls',
        '-Wshadow',
        '-Wstrict-aliasing=2',
        '-Wstrict-prototypes',
        '-Wsuggest-attribute=noreturn',
        '-Wunused-function',
        '-Wwrite-strings',
        '-Wzero-length-bounds',

        # negative arguments are correctly detected starting with meson 0.46.
        '-Wno-error=#warnings',  # clang
        '-Wno-string-plus-int',  # clang

        '-fstrict-flex-arrays',
]

if get_option('buildtype') != 'debug'
        cxx_flags += [
                '-ffunction-sections',
                '-fdata-sections',
        ]
endif

add_project_arguments(cxx.get_supported_arguments(cxx_flags), language : 'cpp')

incdir = include_directories('include')
subdir('test')
