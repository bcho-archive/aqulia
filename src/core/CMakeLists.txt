include_directories (. ../utils)

add_library (record record.c POS.c)
target_link_libraries (record csv misc _debug linksort)

add_library (account account.c validate.c freeze.c
                     sort.c query.c
                     consume.c)
target_link_libraries (account record csv str misc _debug linksort)

install (FILES account.h record.h POS.h
         DESTINATION "include/aqulia")
