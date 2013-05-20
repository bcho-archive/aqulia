include_directories (.)

add_library (csv csv.c)
target_link_libraries (csv utils _debug)

install (FILES account.h personal_info.h consume_record.h
         DESTINATION "include/aqulia")
