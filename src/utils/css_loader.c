#include "css_loader.h"
#include "config_path.h"

GtkCssProvider* css_provider = nullptr;

void load_css() {
    // Get a css path
    char css_path[1024];
    get_config(css_path, "style.css");

    // Load the CSS file
    if (css_provider == NULL) {
        css_provider = gtk_css_provider_new();
    }

    // Load the CSS file
    gtk_css_provider_load_from_path(css_provider, css_path);
}

void apply_css() {
    css_provider = gtk_css_provider_new();

    // Load the initial CSS
    load_css();

    // Add the CSS provider to the default display
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}