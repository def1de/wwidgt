#ifndef LAYOUT_PARSER_H
#define LAYOUT_PARSER_H

#include <gtk/gtk.h>

/**
 * @brief Loads a GTK UI layout from an XML file config and builds the widget tree.
 *
 * This function will parse the specified layout file, construct the GTK widget
 * hierarchy according to the tag definitions, and return the top-level widget.
 *
 * @return GtkWidget* The constructed GTK widget hierarchy, or NULL on failure.
 */
GtkWidget* load_layout(GtkWindow* win);


#endif //LAYOUT_PARSER_H
