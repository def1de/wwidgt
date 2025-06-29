#include "config_path.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void get_config(char* buffer, const char* filename) {
    // Get the path to the XDG_CONFIG_HOME directory
    const char* xdg_config_home = getenv("XDG_CONFIG_HOME");
    if (xdg_config_home == NULL) {
        // If XDG_CONFIG_HOME is not set, use the default path
        xdg_config_home = getenv("HOME");
        if (xdg_config_home == NULL) {
            xdg_config_home = ".config"; // Fallback to a relative path
        } else {
            xdg_config_home = g_strconcat(xdg_config_home, "/.config", NULL);
        }
    }
    // Construct the full path to the file
    printf("Accessing config file: %s/%s/%s\n", xdg_config_home, "wwidgt", filename);
    snprintf(buffer, 1024, "%s/%s/%s", xdg_config_home, "wwidgt", filename);
}