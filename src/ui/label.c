#include "label.h"
#include "../utils/process_str.h"

typedef struct {
    GtkWidget* widget;
    char* text;
} LabelData;

static gboolean update_label(gpointer user_data) {
    LabelData* data = user_data;
    if (!data || !data->text) return G_SOURCE_CONTINUE;

    char* new_text = process_str(data->text);
    if (new_text) {
        gtk_label_set_text(GTK_LABEL(data->widget), new_text);
        free(new_text);
    }
    return G_SOURCE_CONTINUE;
}

GtkWidget* label_widget(xmlNode* node) {
    // Create a new label widget
    GtkWidget* label = gtk_label_new(NULL);

    int update = 0;
    char* label_text = NULL;

    // Apply attributes from the XML node to the label
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "text") == 0) {
            label_text = strdup(value);
            gtk_label_set_text(GTK_LABEL(label), process_str(value));
        } else if (strcmp(name, "update") == 0) {
            update = atoi(value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(label, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(label, value);
        }
    }

    if (update>0) {
        LabelData* data = g_new(LabelData, 1);
        *data = (LabelData){
            .widget = label,
            .text = label_text
        };
        g_timeout_add(update, update_label, data);
    }

    return label;
}