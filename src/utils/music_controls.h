#ifndef MUSIC_CONTROLS_H
#define MUSIC_CONTROLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <gtk/gtk.h>

// Function prototypes for commands
GCallback next();
GCallback previous();
GCallback toggle();
char* fetch_art_url_image();
char* title();
char* artist();
char* length();
GCallback status();

#endif //MUSIC_CONTROLS_H
