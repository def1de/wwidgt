cmake_minimum_required(VERSION 3.30)
project(gtk_layer_shell_test)

set(CMAKE_CXX_STANDARD 14)

# Find GTK4 and gtk4-layer-shell using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GTK4_LAYER_SHELL REQUIRED gtk4-layer-shell-0)
#configure_file(style.css style.css COPYONLY)

# Add the executable
add_executable(gtk_layer_shell_test
        src/main.c
        src/utils/css_loader.h
        src/utils/css_loader.c
        src/utils/music_controls.h
        src/utils/music_controls.c
        src/ui/music_player.h
        src/ui/music_player.c
        src/ui/system_utilisation.h
        src/ui/system_utilisation.c
        src/utils/string_manip.h
        src/utils/string_manip.c
        src/ui/clock.c
        src/ui/clock.h
        src/ui/profile_widget.c
        src/ui/profile_widget.h
        src/ui/power_options.c
        src/ui/power_options.h
        src/utils/config_path.c
        src/utils/config_path.h
)

# Include the necessary directories and link the libraries
target_include_directories(gtk_layer_shell_test PRIVATE ${GTK4_INCLUDE_DIRS} ${GTK4_LAYER_SHELL_INCLUDE_DIRS})
target_link_libraries(gtk_layer_shell_test PRIVATE curl ${GTK4_LIBRARIES} ${GTK4_LAYER_SHELL_LIBRARIES})

# Add compiler flags
target_compile_options(gtk_layer_shell_test PRIVATE ${GTK4_CFLAGS_OTHER} ${GTK4_LAYER_SHELL_CFLAGS_OTHER})