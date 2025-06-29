#include "clock.h"
#include <gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    GtkWidget* clock;
    char* format;
    int update_interval;
} ClockConfig;

static char* replace_tokens(const char* format) {
    // Allocate buffer large enough for expanded format
    char* strftime_format = malloc(256);
    strftime_format[0] = '\0';

    const char* p = format;
    while (*p) {
        if (strncmp(p, "hh", 2) == 0) {
            strcat(strftime_format, "%H");
            p += 2;
        } else if (strncmp(p, "mm", 2) == 0) {
            strcat(strftime_format, "%M");
            p += 2;
        } else if (strncmp(p, "ss", 2) == 0) {
            strcat(strftime_format, "%S");
            p += 2;
        } else if (strncmp(p, "DAY", 3) == 0) {
            strcat(strftime_format, "%A");
            p += 3;
        } else if (strncmp(p, "DD", 2) == 0) {
            strcat(strftime_format, "%d");
            p += 2;
        } else if (strncmp(p, "MM", 2) == 0) {
            strcat(strftime_format, "%m");
            p += 2;
        } else if (strncmp(p, "YYYY", 4) == 0) {
            strcat(strftime_format, "%Y");
            p += 4;
        } else if (strncmp(p, "YY", 2) == 0) {
            strcat(strftime_format, "%y");
            p += 2;
        } else {
            strncat(strftime_format, p, 1);
            p++;
        }
    }

    return strftime_format;
}

static char* format_time(const char* format) {
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);

    char *buffer = malloc(256);
    strftime(buffer, 255, format, lt);

    return buffer;
}

static gboolean update_label(gpointer pconfig) {
    ClockConfig* config = pconfig;
    char* time_str = format_time(config->format);

    gtk_label_set_text(GTK_LABEL(config->clock), time_str);
    free(time_str);

    return G_SOURCE_CONTINUE;
}

GtkWidget* clock_widget(xmlNode *node) {
    GtkWidget* clock = gtk_label_new("");

    ClockConfig* config = g_new(ClockConfig, 1);
    *config = (ClockConfig){
        .clock = clock,
        .format = "%H:%M:%S", // default format
        .update_interval = 1000 // default update interval
    };

    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        const char* name = (const char*)attr->name;
        const char* value = (const char*)attr->children->content;

        if (strcmp(name, "format") == 0) {
            config->format = strdup(replace_tokens(value));
            printf("format: %s\n", config->format);
        } else if (strcmp(name, "update") == 0) {
            config->update_interval = atoi(value);
        } else if (strcmp(name, "class") == 0) {
            gtk_widget_add_css_class(clock, value);
        } else if (strcmp(name, "id") == 0) {
            gtk_widget_set_name(clock, value);
        }
    }

    update_label(config); // initial update
    g_timeout_add(config->update_interval, update_label, config);

    return clock;
}