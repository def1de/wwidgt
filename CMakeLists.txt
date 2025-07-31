cmake_minimum_required(VERSION 3.30)
project(wwidgt)

set(CMAKE_CXX_STANDARD 14)

# Find GTK4 and gtk4-layer-shell using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GTK4_LAYER_SHELL REQUIRED gtk4-layer-shell-0)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
#configure_file(style.css style.css COPYONLY)

# Add the executable
add_executable(wwidgt
        src/main.c
        src/utils/css_loader.h
        src/utils/css_loader.c
        src/utils/music_controls.h
        src/utils/music_controls.c
        src/utils/string_manip.h
        src/utils/string_manip.c
        src/ui/clock.c
        src/ui/clock.h
        src/utils/config_path.c
        src/utils/config_path.h
        src/utils/layout_parser.c
        src/utils/layout_parser.h
        src/ui/label.c
        src/ui/label.h
        src/ui/image.c
        src/ui/image.h
        src/ui/button.c
        src/ui/button.h
        src/ui/arc_progress_bar.c
        src/ui/arc_progress_bar.h
        src/ui/linear_progress_bar.c
        src/ui/linear_progress_bar.h
        src/utils/color.c
        src/utils/color.h
        src/utils/process_str.c
        src/utils/process_str.h
        src/utils/variables.c
        src/utils/variables.h
        src/utils/file_monitor.c
        src/utils/file_monitor.h
        src/daemon.c
        src/daemon.h
        src/ipc.c
        src/ipc.h
        src/utils/logging.c
        src/utils/logging.h
)

# Include the necessary directories and link the libraries
target_include_directories(wwidgt PRIVATE ${GTK4_INCLUDE_DIRS} ${GTK4_LAYER_SHELL_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS})
link_directories(${LIBXML2_LIBRARY_DIRS})

target_link_libraries(wwidgt PRIVATE curl ${GTK4_LIBRARIES} ${GTK4_LAYER_SHELL_LIBRARIES} ${LIBXML2_LIBRARIES})

# Add compiler flags
target_compile_options(wwidgt PRIVATE ${GTK4_CFLAGS_OTHER} ${GTK4_LAYER_SHELL_CFLAGS_OTHER} ${LIBXML2_CFLAGS_OTHER})