include_directories (. ../src ../src/core)

add_executable (test_csv test_csv.c)
target_link_libraries (test_csv csv utils _debug)

add_executable (test_personal_info test_personal_info.c)
target_link_libraries (test_personal_info personal_info utils _debug)

add_executable (test_consume_record test_consume_record.c)
target_link_libraries (test_consume_record consume_record utils _debug)

add_executable (test_sort test_sort.c)
target_link_libraries (test_sort consume_record utils _debug sort)

add_executable (test_account test_account.c)
target_link_libraries (test_account account)

add_executable (test_account_query test_account_query.c)
target_link_libraries (test_account_query account)

install (TARGETS test_csv test_personal_info test_consume_record test_sort
                 test_account test_account_query
         DESTINATION "bin")
