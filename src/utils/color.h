#ifndef COLOR_H
#define COLOR_H

typedef struct {
    double red;
    double green;
    double blue;
    double alpha;
} ColorRGBA;

void hex_to_rgba(const char *hex, ColorRGBA *rgba);

#endif //COLOR_H
