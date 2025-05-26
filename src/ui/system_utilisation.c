#include "system_utilisation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pango/pangocairo.h>

#define TWO_PI (2 * G_PI)

static unsigned long long last_total_time = 0;
static unsigned long long last_idle_time = 0;



typedef struct {
    double *value_ptr;
    const char *icon;
    GtkWidget *drawing_area;
} ProgressArcData;
// Function to draw the circular progress
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
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.2); // light transparent background
    cairo_set_line_width(cr, line_width);
    cairo_arc(cr, cx, cy, radius, 0, TWO_PI);
    cairo_stroke(cr);

    // Foreground arc (progress)
    cairo_set_source_rgba(cr, 0.8, 0.6, 1.0, 1.0); // purple foreground
    cairo_arc(cr, cx, cy, radius, start_angle, end_angle);
    cairo_stroke(cr);

    // Draw CPU icon () in the center
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
    cairo_move_to(cr, cx - text_width / 2.0 - 8, cy - text_height / 2.0);  // the +2 adjusts vertical misalignment
    cairo_set_source_rgb(cr, 1, 1, 1); // white color
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
    pango_font_description_free(desc);
}

// ===== CPU USAGE RELATED FUNCTIONS =====
static double get_cpu_usage() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        return -1.0;
    }

    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), file)) {
        perror("Failed to read /proc/stat");
        fclose(file);
        return -1.0;
    }
    fclose(file);

    // Parse the first line starting with "cpu"
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    if (sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) < 4) {
        fprintf(stderr, "Failed to parse /proc/stat\n");
        return -1.0;
               }

    // Calculate total and idle times
    unsigned long long total_time = user + nice + system + idle + iowait + irq + softirq + steal;
    unsigned long long idle_time = idle + iowait;

    // Calculate CPU usage percentage
    unsigned long long total_diff = total_time - last_total_time;
    unsigned long long idle_diff = idle_time - last_idle_time;

    last_total_time = total_time;
    last_idle_time = idle_time;

    if (total_diff == 0) {
        return 0.0;
    }

    return (double)(total_diff - idle_diff) / total_diff * 100.0;
}

// Timer function to update CPU usage
static gboolean update_cpu_usage(gpointer user_data) {
    ProgressArcData *data = (ProgressArcData *)user_data;
    *(data->value_ptr) = get_cpu_usage();
    gtk_widget_queue_draw(data->drawing_area);
    return G_SOURCE_CONTINUE;
}

GtkWidget* cpu_utilisation() {
    GtkWidget* cpubox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget* label = gtk_label_new("CPU Usage");
    gtk_box_append(GTK_BOX(cpubox), label);

    GtkWidget* drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 150, 150);
    gtk_box_append(GTK_BOX(cpubox), drawing_area);

    static double cpu_usage = 0.0;
    ProgressArcData *cpu_data = g_new(ProgressArcData, 1);
    *cpu_data = (ProgressArcData){
        .icon = "",
        .value_ptr = &cpu_usage,
        .drawing_area = drawing_area
    };

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_progress_arc, cpu_data, NULL);
    update_cpu_usage(cpu_data);
    g_timeout_add_seconds(1, update_cpu_usage, cpu_data);
    return cpubox;
}


// ===== MEMORY USAGE RELATED FUNCTIONS =====
static double get_memory_usage() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Failed to open /proc/meminfo");
        return -1.0;
    }

    char buffer[256];
    unsigned long mem_total = 0, mem_available = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "MemTotal: %lu kB", &mem_total) == 1) {
            continue;
        }
        if (sscanf(buffer, "MemAvailable: %lu kB", &mem_available) == 1) {
            break;
        }
    }
    fclose(file);

    if (mem_total == 0) {
        fprintf(stderr, "Failed to read memory information\n");
        return -1.0;
    }

    double mem_used = mem_total - mem_available;
    return (mem_used / mem_total) * 100.0;
}

static gboolean update_memory_usage(gpointer user_data) {
    ProgressArcData *data = (ProgressArcData *)user_data;
    *(data->value_ptr) = get_memory_usage();
    gtk_widget_queue_draw(data->drawing_area);
    return G_SOURCE_CONTINUE;
}

GtkWidget* memory_utilisation() {
    GtkWidget* membox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget* label = gtk_label_new("Memory Usage");
    gtk_box_append(GTK_BOX(membox), label);

    GtkWidget* drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 150, 150);
    gtk_box_append(GTK_BOX(membox), drawing_area);

    static double mem_usage = 0.0;
    ProgressArcData *mem_data = g_new(ProgressArcData, 1);
    *mem_data = (ProgressArcData){
        .icon = "",
        .value_ptr = &mem_usage,
        .drawing_area = drawing_area
    };

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_progress_arc, mem_data, NULL);
    update_memory_usage(mem_data);
    g_timeout_add_seconds(1, update_memory_usage, mem_data);
    return membox;
}

// ===== UPTIME =====
void get_uptime(char *buffer, size_t buffer_size) {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        perror("Failed to open /proc/uptime");
        snprintf(buffer, buffer_size, "Error");
        return;
    }

    double uptime_seconds;
    if (fscanf(file, "%lf", &uptime_seconds) != 1) {
        perror("Failed to read uptime");
        snprintf(buffer, buffer_size, "Error");
        fclose(file);
        return;
    }
    fclose(file);

    int hours = uptime_seconds / 3600;
    uptime_seconds = (int)uptime_seconds % 3600;
    int minutes = uptime_seconds / 60;
    int seconds = (int)uptime_seconds % 60;

    snprintf(buffer, buffer_size, "Uptime: %02d:%02d:%02d", hours, minutes, seconds);
}

static gboolean update_uptime_label(gpointer label) {
    char uptime_str[64];
    get_uptime(uptime_str, sizeof(uptime_str));
    gtk_label_set_text(GTK_LABEL(label), uptime_str);
    return G_SOURCE_CONTINUE;
}


// Create the widget
GtkWidget* system_utilisation() {
    srand(time(NULL));

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_add_css_class(vbox, "system_util_box");
    gtk_widget_set_vexpand(vbox, TRUE);

    GtkWidget* util_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(util_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(vbox), util_box);

    GtkWidget* cpubox = cpu_utilisation();
    gtk_box_append(GTK_BOX(util_box), cpubox);

    GtkWidget* membox = memory_utilisation();
    gtk_box_append(GTK_BOX(util_box), membox);

    GtkWidget* info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign(info_box, GTK_ALIGN_START);
    gtk_widget_add_css_class(info_box, "util_info_box");
    gtk_box_append(GTK_BOX(vbox), info_box);

    GtkWidget* uptime_label = gtk_label_new("Uptime: ");
    update_uptime_label(uptime_label);
    g_timeout_add_seconds(1, update_uptime_label, uptime_label);

    gtk_box_append(GTK_BOX(info_box), uptime_label);



    return vbox;
}