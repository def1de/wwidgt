#include "image.h"

GtkWidget* image_widget(xmlNode* node) {
    // Create a new image widget
    GtkWidget* image = gtk_image_new();

    // Apply attributes from the XML node to the image
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "src") == 0) {
            gtk_image_set_from_file(GTK_IMAGE(image), value);
        } else if (strcmp(name, "size") == 0) {
            gtk_image_set_pixel_size(GTK_IMAGE(image), atoi(value));
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(image, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(image, value);
        }
    }

    return image;
}