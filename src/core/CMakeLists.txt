include_directories (. ../utils)

add_library (consume_record consume_record.c POS.c)
target_link_libraries (consume_record csv misc _debug link_sort)

add_library (account account.c account_validate.c account_freeze.c
                     account_sort.c account_query.c
                     account_consume.c)
target_link_libraries (account consume_record csv str misc _debug link_sort)

install (FILES account.h consume_record.h POS.h
         DESTINATION "include/aqulia")
