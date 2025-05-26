#include "gtk4-layer-shell.h"
#include <gtk/gtk.h>
#include "utils/music_controls.h"
#include "utils/css_loader.h"
#include "ui/music_player.h"
#include "ui/system_utilisation.h"
#include "ui/clock.h"
#include "ui/profile_widget.h"
#include "ui/power_options.h"


static void activate(GtkApplication* app, const void* _data) {
    (void)_data; // Suppress unused parameter warning

    apply_css(); // Load and apply CSS styles

    // Create a normal GTK window
    GtkWindow* gtk_window = GTK_WINDOW(gtk_application_window_new(app));

    // Set it up as a layer surface
    gtk_layer_init_for_window(gtk_window);
    gtk_layer_set_layer(gtk_window, GTK_LAYER_SHELL_LAYER_BACKGROUND);

    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    // Initialize the music player widget
    GtkWidget* music_widget = music_player();
    gtk_box_append(GTK_BOX(hbox), music_widget);

    GtkWidget* clock_utils_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(clock_utils_box, 400, -1);
    gtk_box_append(GTK_BOX(hbox), clock_utils_box);

    GtkWidget* clock = clock_widget();
    gtk_box_append(GTK_BOX(clock_utils_box), clock);

    // Initialize the system utilisation widget
    GtkWidget* system_widget = system_utilisation();
    gtk_box_append(GTK_BOX(clock_utils_box), system_widget);

    GtkWidget* power_profile_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(power_profile_box, 300, -1);
    gtk_box_append(GTK_BOX(hbox), power_profile_box);

    GtkWidget* profile_widget_box = profile_widget();
    gtk_box_append(GTK_BOX(power_profile_box), profile_widget_box);

    GtkWidget* power_widget = power_options_widget();
    gtk_box_append(GTK_BOX(power_profile_box), power_widget);

    // Set the box as the child of the window
    gtk_window_set_child(gtk_window, hbox);

    gtk_window_present(gtk_window);
}

int main(const int argc, char **argv) {
    GtkApplication* app = gtk_application_new("com.def1de.desktop_widget", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    const int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}