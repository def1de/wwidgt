#include "file_monitor.h"
#include "config_path.h"
#include <unistd.h>

void relaunch_self(char **argv) {
    execv(argv[0], argv);
    perror("Cannot restart the application");
}

static void on_file_changed(GFileMonitor* monitor, GFile* file, GFile* other_file, GFileMonitorEvent event, gpointer user_data) {
    if (event == G_FILE_MONITOR_EVENT_CHANGED || event == G_FILE_MONITOR_EVENT_CREATED) {
        printf("File changed. Relaunching...\n");
        char **argv = (char **)user_data;
        relaunch_self(argv);
    }
}

// Function to set up file monitoring
void monitor_files(char **argv) {
    // Get the directory of the current source file
    char css_path[1024];
    get_config(css_path, "style.css");
    // Monitor the CSS file
    GFile* css_file = g_file_new_for_path(css_path);
    GFileMonitor* css_monitor = g_file_monitor_file(css_file, G_FILE_MONITOR_NONE, nullptr, nullptr);

    if (css_monitor) {
        g_signal_connect(css_monitor, "changed", G_CALLBACK(on_file_changed), argv);
    }

    char layout_path[1024];
    get_config(layout_path, "layout.xml");
    // Monitor the layout file
    GFile* layout_file = g_file_new_for_path(layout_path);
    GFileMonitor* layout_monitor = g_file_monitor_file(layout_file, G_FILE_MONITOR_NONE, nullptr, nullptr);
    if (layout_monitor) {
        g_signal_connect(layout_monitor, "changed", G_CALLBACK(on_file_changed), argv);
    }

    g_object_unref(css_file);
    g_object_unref(layout_file);
}