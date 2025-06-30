#include "button.h"
#include "../utils/process_str.h"
#include "../utils/logging.h"

typedef struct {
    GtkWidget* widget;
    char* text;
} ButtonData;

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    const char *command = (const char *)user_data;
    g_spawn_command_line_async(command, NULL);
}

static gboolean update_button_label(gpointer user_data) {
    ButtonData *data = (ButtonData *)user_data;
    if (!data->text) return G_SOURCE_CONTINUE;

    char* new_label = process_str(data->text);
    if (new_label) {
        gtk_button_set_label(GTK_BUTTON(data->widget), new_label);
        g_free(new_label);
    }
    return G_SOURCE_CONTINUE;
}

GtkWidget* button_widget(xmlNode* node) {
    // Create a new button widget
    GtkWidget* button = gtk_button_new();

    char* button_label = NULL;
    int update = 0;

    // Apply attributes from the XML node to the button
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "label") == 0) {
            button_label = g_strdup(value);
            gtk_button_set_label(GTK_BUTTON(button), process_str(value));
        } else if (strcmp(name, "action") == 0) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), g_strdup(value));
        } else if (strcmp(name, "update") == 0) {
            update = atoi(value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(button, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(button, value);
        }
    }

    if (update > 0) {
        ButtonData *data = g_new0(ButtonData, 1);
        *data = (ButtonData){
            .widget = button,
            .text = button_label
        };
        g_timeout_add(update, update_button_label, data);
    }

    return button;
}