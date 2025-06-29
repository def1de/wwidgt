#include "color.h"
#include <stdio.h>

void hex_to_rgba(const char *hex, ColorRGBA *rgba) {
    unsigned int ri, gi, bi, ai;
    if (hex[0] == '#') hex++;
    sscanf(hex, "%2x%2x%2x%2x", &ri, &gi, &bi, &ai);
    rgba->red = ri / 255.0;
    rgba->green = gi / 255.0;
    rgba->blue = bi / 255.0;
    rgba->alpha = (ai == 0) ? 1.0 : ai / 255.0; // Default alpha to 1.0 if not specified
}
