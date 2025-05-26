#include "power_options.h"

GtkWidget* power_options_widget() {
    GtkWidget* power_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_add_css_class(power_box, "power_options_box");
    gtk_widget_set_size_request(power_box, 300, -1);
    gtk_widget_set_halign(power_box, GTK_ALIGN_CENTER);

    GtkWidget* power_button = gtk_button_new_with_label("⏻");
    gtk_widget_add_css_class(power_button, "power_button");
    gtk_box_append(GTK_BOX(power_box), power_button);

    GtkWidget* hibernate_button = gtk_button_new_with_label("󰤄");
    gtk_widget_add_css_class(hibernate_button, "hibernate_button");
    gtk_box_append(GTK_BOX(power_box), hibernate_button);

    GtkWidget* logout_button = gtk_button_new_with_label("󰍃");
    gtk_widget_add_css_class(logout_button, "hibernate_button");
    gtk_box_append(GTK_BOX(power_box), logout_button);

    return power_box;
}