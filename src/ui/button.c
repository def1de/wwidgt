#include "button.h"

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    const char *command = (const char *)user_data;
    g_spawn_command_line_async(command, NULL);
}

GtkWidget* button_widget(xmlNode* node) {
    // Create a new button widget
    GtkWidget* button = gtk_button_new();

    // Apply attributes from the XML node to the button
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "label") == 0) {
            gtk_button_set_label(GTK_BUTTON(button), value);
        } else if (strcmp(name, "action") == 0) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), g_strdup(value));
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(button, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(button, value);
        }
    }

    return button;
}