include_directories (.)

add_executable (generator generator.c)

install (TARGETS generator
         DESTINATION "bin")
