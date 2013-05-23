include_directories (. ../src ../src/core ../src/utils)

add_executable (test_csv test_csv.c)
target_link_libraries (test_csv csv str _debug)

add_executable (test_account test_account.c)
target_link_libraries (test_account account)

install (TARGETS test_csv test_account
         DESTINATION "bin")
