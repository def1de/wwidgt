#include "profile_widget.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../utils/string_manip.h"
#include "../utils/config_path.h"

static void get_user(GtkWidget* label) {
    // Get the username from the environment variable
    const char* user = getenv("USER");
    if (user == NULL) {
        // If the environment variable is not set, try to get the username from the passwd struct
        struct passwd* pw = getpwuid(getuid());
        if (pw != NULL) {
            user = pw->pw_name;
        } else {
            user = "User";
        }
    }
    // Set the label text to the username
    char* label_text = concat_str("Hello, ", user);
    label_text = concat_str(label_text, "!");
    gtk_label_set_text(GTK_LABEL(label), label_text);
    free(label_text);
}

GtkWidget* profile_widget() {
    GtkWidget* profile_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(profile_box, 300, -1);
    gtk_widget_add_css_class(profile_box, "profile_widget_box");
    gtk_widget_set_vexpand(profile_box, TRUE);

    char image_path[1024];
    get_config(image_path, "pfp.png");

    GtkWidget* profile_image = gtk_image_new_from_file(image_path);
    gtk_image_set_pixel_size(GTK_IMAGE(profile_image), 200);
    gtk_box_append(GTK_BOX(profile_box), profile_image);

    GtkWidget* label = gtk_label_new("Hello, User");
    gtk_box_append(GTK_BOX(profile_box), label);

    get_user(label);

    return profile_box;
}