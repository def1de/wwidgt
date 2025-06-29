#include "layout_parser.h"
#include <libxml/parser.h>
#include <string.h>
#include <stdlib.h>
#include "config_path.h"

#include "../ui/music_player.h"
#include "../ui/system_utilisation.h"
#include "../ui/clock.h"
#include "../ui/profile_widget.h"
#include "../ui/power_options.h"
#include "../ui/label.h"
#include "../ui/image.h"
#include "../ui/button.h"
#include "../ui/arc_progress_bar.h"
#include "../ui/linear_progress_bar.h"

/**
 * @brief Retrieves a custom widget based on a tag name.
 *
 * This is the mapping from tag strings like <clock> or <music_player> to
 * actual GTK widget construction functions.
 *
 * @param tag_name The XML tag name.
 * @return GtkWidget* A GTK widget created by the corresponding function.
 */
GtkWidget* create_widget_from_tag(const char* tag_name, xmlNode* node) {
    if (strcmp(tag_name, "music_player") == 0) {
        return music_player();
    } else if (strcmp(tag_name, "clock") == 0) {
        return clock_widget();
    } else if (strcmp(tag_name, "system_utilisation") == 0) {
        return system_utilisation();
    } else if (strcmp(tag_name, "profile_widget") == 0) {
        return profile_widget();
    } else if (strcmp(tag_name, "power_options_widget") == 0) {
        return power_options_widget();
    } else if (strcmp(tag_name, "label") == 0) {
        return label_widget(node);
    } else if (strcmp(tag_name, "image") == 0) {
        return image_widget(node);
    } else if (strcmp(tag_name, "button") == 0) {
        return button_widget(node);
    } else if (strcmp(tag_name, "arc-progress-bar") == 0) {
        return arc_progress_bar_widget(node);
    } else if (strcmp(tag_name, "linear-progress-bar") == 0) {
        return liner_progress_bar_widget(node);
    }
    return NULL;
}

/**
 * @brief Applies width/height and other properties to a widget based on XML attributes.
 *
 * @param widget The GTK widget to configure.
 * @param node The XML node containing attributes like width, height, class, id.
 */
static void apply_attributes(GtkWidget* widget, xmlNode* node) {
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "width") == 0 || strcmp(name, "height") == 0) {
            int width = -1, height = -1;
            if (strcmp(name, "width") == 0) {
                width = atoi(value);
                gtk_widget_set_size_request(widget, width, -1);
            } else if (strcmp(name, "height") == 0) {
                height = atoi(value);
                gtk_widget_set_size_request(widget, -1, height);
            }
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(widget, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(widget, value);
        }
    }
}

/**
 * @brief Recursively constructs a GTK widget tree from an XML node.
 *
 * Handles <hbox>, <vbox>, and custom tags like <clock> and <music_player>.
 *
 * @param node The XML node to process.
 * @return GtkWidget* The resulting GTK widget subtree.
 */
static GtkWidget* build_ui_from_node(xmlNode* node) {
    if (node->type != XML_ELEMENT_NODE) return NULL;

    const char* tag = (const char*)node->name;
    GtkWidget* widget = NULL;

    if (strcmp(tag, "hbox") == 0 || strcmp(tag, "vbox") == 0) {
        GtkOrientation orientation = strcmp(tag, "vbox") == 0
            ? GTK_ORIENTATION_VERTICAL
            : GTK_ORIENTATION_HORIZONTAL;
        widget = gtk_box_new(orientation, 5);
        apply_attributes(widget, node);

        for (xmlNode* child = node->children; child; child = child->next) {
            GtkWidget* child_widget = build_ui_from_node(child);
            if (child_widget) {
                gtk_box_append(GTK_BOX(widget), child_widget);
            }
        }
    } else if (strcmp(tag, "window") == 0) {
        // The <window> node is a placeholder. Only process its first child.
        for (xmlNode* child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                return build_ui_from_node(child);  // Root layout widget
            }
        }
    } else {
        widget = create_widget_from_tag(tag, node);
        if (widget) {
            apply_attributes(widget, node);
        }
    }

    return widget;
}

/**
 * @brief Parses an XML layout file and returns the corresponding GTK widget.
 *
 * @param path Path to the layout file (e.g., "layout.xml").
 * @return GtkWidget* GTK widget tree, or NULL on error.
 */
GtkWidget* load_layout() {
    char path[1024];
    get_config(path, "layout.xml");
    xmlDoc* doc = xmlReadFile(path, NULL, 0);
    if (!doc) {
        g_warning("Failed to read layout file: %s", path);
        return NULL;
    }

    xmlNode* root = xmlDocGetRootElement(doc);
    GtkWidget* ui = build_ui_from_node(root);
    xmlFreeDoc(doc);

    return ui;
}
