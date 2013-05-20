include_directories (. ../src ../src/core)

add_executable (test_csv test_csv.c)
target_link_libraries (test_csv csv utils _debug)

install (TARGETS test_csv
         DESTINATION "bin")
