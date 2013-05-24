include_directories (. ../vendor)

add_library (unit unit.c)
target_link_libraries (unit conio _debug)

add_library (screen screen.c)
target_link_libraries (screen conio str)
