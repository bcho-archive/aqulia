include_directories (. ../src ../src/core)

add_executable (test_csv test_csv.c)
target_link_libraries (test_csv csv utils _debug)

add_executable (test_personal_info test_personal_info.c)
target_link_libraries (test_personal_info personal_info utils _debug)

install (TARGETS test_csv test_personal_info
         DESTINATION "bin")
