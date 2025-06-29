#ifndef CSS_LOADER_H
#define CSS_LOADER_H

#include <libgen.h>
#include <string.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

extern GtkCssProvider* css_provider;

void load_css();
static void on_file_changed(GFileMonitor* monitor, GFile* file, GFile* other_file,
                            GFileMonitorEvent event, gpointer user_data);
void apply_css();

#endif //CSS_LOADER_H
