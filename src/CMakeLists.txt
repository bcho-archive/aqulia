include_directories (.)

add_subdirectory (core)
add_subdirectory (utils)
add_subdirectory (cli)
add_subdirectory (vendor)
# add_subdirectory (generator)

add_executable (app app.c)
target_link_libraries (app account conio screen unit)

install (TARGETS app
         DESTINATION "bin")
