include_directories (. ../vendor)

add_executable (test test.c)
target_link_libraries (test conio)

install (TARGETS test
         DESTINATION "bin")
