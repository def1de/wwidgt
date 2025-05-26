#include "css_loader.h"
#include "config_path.h"

GtkCssProvider* css_provider = nullptr;

void load_css() {
    // Get a css path
    char css_path[1024];
    get_config(css_path, "style.css");

    // Load the CSS file
    if (css_provider == NULL) {
        css_provider = gtk_css_provider_new();
    }

    // Load the CSS file
    gtk_css_provider_load_from_path(css_provider, css_path);
}

// Callback for file changes
static void on_file_changed(GFileMonitor* monitor, GFile* file, GFile* other_file, GFileMonitorEvent event, gpointer user_data) {
    if (event == G_FILE_MONITOR_EVENT_CHANGED || event == G_FILE_MONITOR_EVENT_CREATED) {
        load_css(); // Reload the CSS when the file changes
    }
}

// Function to set up file monitoring
void monitor_css_file() {
    // Get the directory of the current source file
    char css_path[1024];


    // Monitor the CSS file
    GFile* file = g_file_new_for_path(css_path);
    GFileMonitor* monitor = g_file_monitor_file(file, G_FILE_MONITOR_NONE, nullptr, nullptr);

    if (monitor) {
        g_signal_connect(monitor, "changed", G_CALLBACK(on_file_changed), NULL);
    }

    g_object_unref(file);
}

void apply_css() {
    css_provider = gtk_css_provider_new();

    // Load the initial CSS
    load_css();

    // Add the CSS provider to the default display
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // Monitor the CSS file for changes
    monitor_css_file();
}