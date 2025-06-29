#include "label.h"

GtkWidget* label_widget(xmlNode* node) {
    // Create a new label widget
    GtkWidget* label = gtk_label_new(NULL);

    // Apply attributes from the XML node to the label
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "text") == 0) {
            gtk_label_set_text(GTK_LABEL(label), value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(label, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(label, value);
        }
    }

    return label;
}