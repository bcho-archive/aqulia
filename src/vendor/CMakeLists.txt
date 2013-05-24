include_directories (.)

add_library (conio conio.c)
target_link_libraries (conio ncurses)
