include_directories (.)

add_library (csv csv.c)
target_link_libraries (csv utils _debug)

install (FILES account.h
         DESTINATION "include/aqulia")
