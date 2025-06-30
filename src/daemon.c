#include "daemon.h"
#include "gtk4-layer-shell.h"
#include <gtk/gtk.h>
#include "utils/music_controls.h"
#include "utils/css_loader.h"
#include "utils/layout_parser.h"
#include "utils/file_monitor.h"
#include "ipc.h"
#include <pthread.h>
#include "utils/logging.h"

static void activate(GtkApplication* app, const void* _data) {
    GtkWindow* win = GTK_WINDOW(gtk_application_window_new(app));
    gtk_layer_init_for_window(win);
    gtk_layer_set_layer(win, GTK_LAYER_SHELL_LAYER_BACKGROUND);
    log_printf("Applying css\n");
    apply_css(); // load your CSS from file
    log_printf("Loading layout\n");
    GtkWidget* layout = load_layout();
    if (layout) {
        gtk_window_set_child(win, layout);
        gtk_window_present(win);
    }
}

void run_daemon(int argc, char** argv) {
    GtkApplication* app = gtk_application_new("com.def1de.wwidgt", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    monitor_files(argv);

    pthread_t ipc_thread;
    pthread_create(&ipc_thread, NULL, ipc_start_server, NULL);
    pthread_detach(ipc_thread);

    g_application_run(G_APPLICATION(app), 1, &argv[0]);
    g_object_unref(app);
}