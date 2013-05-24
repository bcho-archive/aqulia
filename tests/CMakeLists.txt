include_directories (. ../src ../src/core ../src/utils ../src/cli)

add_executable (test_csv test_csv.c)
target_link_libraries (test_csv csv str _debug)

add_executable (test_account test_account.c)
target_link_libraries (test_account account)

add_executable (test_unit test_unit.c)
target_link_libraries (test_unit unit screen)

install (TARGETS test_unit
         DESTINATION "bin")
