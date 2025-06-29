#include "arc_progress_bar.h"
#include "../utils/color.h"

typedef struct {
    double *value_ptr;
    const char *icon;
    ColorRGBA fg_color;
    ColorRGBA bg_color;
    GtkWidget *drawing_area;
    double icon_alignment[2];
    char *script_path;
} ProgressArcData;

static void draw_progress_arc(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    ProgressArcData *data = (ProgressArcData *)user_data;

    const char *icon = data->icon;
    double percent = *(data->value_ptr) / 100.0;

    // Circle config
    double radius = MIN(width, height) / 2.5;
    double line_width = 12.0;
    double start_angle = -G_PI / 2.0;
    double end_angle = start_angle + percent * TWO_PI;

    // Coordinates
    double cx = width / 2.0;
    double cy = height / 2.0;

    // Background arc (full circle)
    cairo_set_source_rgba(cr, data->bg_color.red, data->bg_color.green, data->bg_color.blue, data->bg_color.alpha);
    cairo_set_line_width(cr, line_width);
    cairo_arc(cr, cx, cy, radius, 0, TWO_PI);
    cairo_stroke(cr);

    // Foreground arc (progress)
    cairo_set_source_rgba(cr, data->fg_color.red, data->fg_color.green, data->fg_color.blue, data->fg_color.alpha);
    cairo_arc(cr, cx, cy, radius, start_angle, end_angle);
    cairo_stroke(cr);

    // Draw CPU icon in the center
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, icon, -1);

    // Set font (Nerd Font required)
    PangoFontDescription *desc = pango_font_description_from_string("JetBrainsMono Nerd Font 28");
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

    int text_width, text_height;
    pango_layout_get_size(layout, &text_width, &text_height);
    text_width /= PANGO_SCALE;
    text_height /= PANGO_SCALE;

    // Manual offset adjustment to visually center the icon
    cairo_move_to(cr, cx - text_width / 2.0 + data->icon_alignment[0], cy - text_height / 2.0 + data->icon_alignment[1]);
    cairo_set_source_rgb(cr, 1, 1, 1); // white color
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
    pango_font_description_free(desc);
}

// Timer function to update the value
static gboolean update_progress(gpointer user_data) {
    ProgressArcData *data = (ProgressArcData *)user_data;
    if (!data->script_path) return G_SOURCE_CONTINUE;

    FILE *fp = popen(data->script_path, "r");
    if (fp) {
        char buf[32];
        if (fgets(buf, sizeof(buf), fp)) {
            double val = atof(buf);
            *(data->value_ptr) = val;
            if (data->drawing_area)
                gtk_widget_queue_draw(data->drawing_area);
        }
        pclose(fp);
    }
    return G_SOURCE_CONTINUE;
}

GtkWidget* arc_progress_bar_widget(xmlNode* node) {
    GtkWidget* arc_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget* label = gtk_label_new(nullptr);
    GtkWidget* drawing_area = gtk_drawing_area_new();

    double *value_data = g_new(double, 1);
    *value_data = 50.0;
    int size = 150;
    char *icon = "";
    double icon_alignment[2] = {0, 0}; // Default to center alignment
    ColorRGBA fg_color;
    ColorRGBA bg_color;
    int update_timer = 1000;
    char *script_path = g_new(char, 1);

    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "action") == 0) {
            script_path = g_strdup(value);
        } else if (strcmp(name, "label") == 0) { // label above the progressbar
            gtk_label_set_text(GTK_LABEL(label), value);
        } else if (strcmp(name, "size") == 0) { // size of the progress arc
            size = atoi(value);
        } else if (strcmp(name, "icon") == 0) { // icon in the center of the progress arc
            icon = value;
        } else if (strcmp(name, "fg-color") == 0) { // color of the progress arc
            hex_to_rgba(value, &fg_color);
        } else if (strcmp(name, "bg-color") == 0) { // color of the background arc
            hex_to_rgba(value, &bg_color);
        } else if (strcmp(name, "update") == 0) { // update interval in milliseconds
            update_timer = atoi(value);
        } else if (strcmp(name, "x-align") == 0) { // icon alignment in x-direction
            icon_alignment[0] = atof(value);
        } else if (strcmp(name, "y-align") == 0) { // icon alignment in y-direction
            icon_alignment[1] = atof(value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(arc_box, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(arc_box, value);
        }
    }
    gtk_box_append(GTK_BOX(arc_box), label);

    gtk_widget_set_size_request(drawing_area, size, size);
    gtk_box_append(GTK_BOX(arc_box), drawing_area);

    ProgressArcData *data = g_new(ProgressArcData, 1);
    *data = (ProgressArcData){
        .icon = g_strdup(icon),
        .value_ptr = value_data,
        .fg_color = fg_color,
        .bg_color = bg_color,
        .drawing_area = drawing_area,
        .icon_alignment = {icon_alignment[0], icon_alignment[1]},
        .script_path = script_path // No script path used in this widget
    };

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_progress_arc, data, NULL);
    g_timeout_add(update_timer, update_progress, data);
    return arc_box;
}