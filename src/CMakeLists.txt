include_directories (.)

add_library (utils utils.c)
add_library (_debug debug.c)

add_subdirectory (core)
add_subdirectory (generator)
