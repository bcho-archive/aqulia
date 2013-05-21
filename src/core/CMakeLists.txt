include_directories (.)

add_library (csv csv.c)
target_link_libraries (csv utils _debug)

add_library (sort sort.c)

add_library (personal_info personal_info.c)
target_link_libraries (personal_info csv utils _debug)

add_library (consume_record consume_record.c)
target_link_libraries (consume_record csv utils _debug)

install (FILES account.h personal_info.h consume_record.h
         DESTINATION "include/aqulia")
