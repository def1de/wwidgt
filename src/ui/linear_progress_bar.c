#include "linear_progress_bar.h"

typedef struct {
    GtkWidget* widget;
    char* script_path;
} ProgressData;

static gboolean update_progress(gpointer user_data) {
    ProgressData *data = (ProgressData *)user_data;
    if (!data->script_path) return G_SOURCE_CONTINUE;

    FILE *fp = popen(data->script_path, "r");
    if (fp) {
        char buf[32];
        if (fgets(buf, sizeof(buf), fp)) {
            double val = atof(buf);
            printf("Progress: %f\n", val);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->widget), val/100.0);
        }
        pclose(fp);
    }
    return G_SOURCE_CONTINUE;
}

GtkWidget* liner_progress_bar_widget(xmlNode* node) {
    GtkWidget* progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);

    char *script_path = NULL;
    int update_interval = 1000; // Default update interval in milliseconds

    // Apply attributes from XML node
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "action") == 0) { // Script path for action
            script_path = strdup(value);
        } else if (strcmp(name, "update") == 0) {
            update_interval = atoi(value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(progress_bar, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(progress_bar, value);
        }
    }

    ProgressData *data = g_new(ProgressData, 1);
    *data = (ProgressData){
        .widget = progress_bar,
        .script_path = script_path
    };

    g_timeout_add(update_interval, update_progress, data);

    return progress_bar;
}