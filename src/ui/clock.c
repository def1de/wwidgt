#include "clock.h"
#include <stdio.h>
#include <time.h>
#include "../utils/string_manip.h"

static void get_current_time(char *buffer, size_t buffer_size) {
    time_t raw_time;

    // Get the current time
    time(&raw_time);
    struct tm *time_info = localtime(&raw_time);

    // Format the time as HH:MM:SS
    snprintf(buffer, buffer_size, "%02d:%02d:%02d",
             time_info->tm_hour,
             time_info->tm_min,
             time_info->tm_sec);
}

static void get_current_date(char *buffer, size_t buffer_size) {
    time_t raw_time;
    struct tm *time_info;

    // Get the current time
    time(&raw_time);
    time_info = localtime(&raw_time);

    // Format the date as DD/MM/YYYY
    snprintf(buffer, buffer_size, "%02d/%02d/%04d",
             time_info->tm_mday,
             time_info->tm_mon + 1, // Months are 0-based
             time_info->tm_year + 1900); // Years since 1900
}

static void get_day_of_week(char *buffer, size_t buffer_size) {
    time_t raw_time;
    struct tm *time_info;

    // Get the current time
    time(&raw_time);
    time_info = localtime(&raw_time);

    // Array of day names
    const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    // Get the day of the week
    snprintf(buffer, buffer_size, "%s", days[time_info->tm_wday]);
}

static gboolean update_time(gpointer label) {
    char time_str[9]; // HH:MM:SS
    get_current_time(time_str, sizeof(time_str));
    gtk_label_set_text(GTK_LABEL(label), time_str);
    return G_SOURCE_CONTINUE;
}

static void update_date(GtkWidget* label) {
    char date_str[11]; // DD/MM/YYYY
    char day_str[10]; // Day of the week
    get_current_date(date_str, sizeof(date_str));
    get_day_of_week(day_str, sizeof(day_str));
    char* day_date_str = concat_str(day_str, ", ");
    day_date_str = concat_str(day_date_str, date_str);
    gtk_label_set_text(GTK_LABEL(label), day_date_str);
    free(day_date_str);
}

GtkWidget* clock_widget() {
    GtkWidget* clock_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_add_css_class(clock_box, "clock_box");
    gtk_widget_set_size_request(clock_box, 400, -1);
    gtk_widget_set_halign(clock_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(clock_box, GTK_ALIGN_CENTER);

    GtkWidget* time_label = gtk_label_new("00:00:00");
    gtk_widget_add_css_class(time_label, "time_label");
    gtk_box_append((GtkBox*)clock_box, time_label);
    GtkWidget* date_label = gtk_label_new("Thursday, 01/01/1970");
    gtk_widget_add_css_class(date_label, "date_label");
    gtk_box_append((GtkBox*)clock_box, date_label);

    update_time(time_label);
    g_timeout_add_seconds(1, (GSourceFunc)update_time, time_label);
    update_date(date_label);

    return clock_box;
}