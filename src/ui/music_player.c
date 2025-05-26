#include "gtk4-layer-shell.h"
#include <gtk/gtk.h>
#include "../utils/music_controls.h"
#include "../utils/css_loader.h"
#include "../utils/string_manip.h"

static gboolean update_image(GtkImage* image) {
    char* image_path = fetch_art_url_image();
    if (image_path) {
        gtk_image_set_from_file(image, image_path);
        free(image_path); // Free the dynamically allocated path
    } else {
        gtk_image_set_from_icon_name(image, "image-missing");
    }
    return G_SOURCE_CONTINUE;
}

// Struct to hold labels
typedef struct {
    GtkLabel* author_label;
    GtkLabel* title_label;
} LabelData;

// Function to update the label text
static gboolean update_label_text(gpointer user_data) {
    LabelData* data = (LabelData*)user_data;

    char* song_title = title();
    char* author = artist();

    if (song_title) {
        char* title_text = concat_str("Title: ", song_title);
        gtk_label_set_text(data->title_label, title_text);
        free(title_text);
        free(song_title);
    } else {
        gtk_label_set_text(data->title_label, "Title: Unknown");
    }

    if (author) {
        char* author_text = concat_str("Author: ", author);
        gtk_label_set_text(data->author_label, author_text);
        free(author_text);
        free(author);
    } else {
        gtk_label_set_text(data->author_label, "Author: Unknown");
    }

    return G_SOURCE_CONTINUE;
}

GtkWidget* music_player() {
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Limit the size of the vbox
    gtk_widget_set_size_request(vbox, 300, 400);
    gtk_widget_add_css_class(vbox, "music_player_box");

    // Create an image widget for the album art
    GtkWidget* album_art_image = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(album_art_image), 200);
    gtk_box_append(GTK_BOX(vbox), album_art_image);

    // Create a label widgets
    GtkWidget* title_label = gtk_label_new("Title: ");
    // Set wrapping and width for the title label
    gtk_label_set_wrap(GTK_LABEL(title_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(title_label), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_width_chars(GTK_LABEL(title_label), 20);
    gtk_label_set_max_width_chars(GTK_LABEL(title_label), 20);
    gtk_box_append(GTK_BOX(vbox), title_label);

    GtkWidget* author_label = gtk_label_new("Author: ");
    gtk_label_set_wrap(GTK_LABEL(author_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(author_label), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_width_chars(GTK_LABEL(author_label), 20);
    gtk_label_set_max_width_chars(GTK_LABEL(author_label), 20);
    gtk_box_append(GTK_BOX(vbox), author_label);

    //Create button box
    GtkWidget* button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);


    // Create the previous button
    GtkWidget* previous_button = gtk_button_new_with_label("󰒮");
    gtk_box_append(GTK_BOX(button_box), previous_button);
    // Connect the button's "clicked" signal to the callback
    g_signal_connect(G_OBJECT(previous_button), "clicked", G_CALLBACK(previous), NULL);

    // Create the play/pause button
    GtkWidget* play_pause_button = gtk_button_new_with_label("");
    gtk_box_append(GTK_BOX(button_box), play_pause_button);
    g_signal_connect(play_pause_button, "clicked", G_CALLBACK(toggle), NULL);

    // Create the next button
    GtkWidget* next_button = gtk_button_new_with_label("󰒭");
    gtk_box_append(GTK_BOX(button_box), next_button);
    g_signal_connect(next_button, "clicked", G_CALLBACK(next), NULL);

    gtk_box_append(GTK_BOX(vbox), button_box);

    // Update labels every second
    LabelData* label_data = g_new(LabelData, 1);
    label_data->title_label = GTK_LABEL(title_label);
    label_data->author_label = GTK_LABEL(author_label);

    update_label_text(label_data);
    g_timeout_add_seconds(1, (GSourceFunc)update_label_text, label_data);
    update_image(GTK_IMAGE(album_art_image));
    g_timeout_add_seconds(1, (GSourceFunc)update_image, GTK_IMAGE(album_art_image));

    return vbox;
}