#include "music_controls.h"

GCallback next() {
    system("playerctl next");
}

GCallback previous() {
    system("playerctl previous");
}

GCallback toggle() {
    system("playerctl play-pause");
}

static char prev_url[512] = {0}; // Static buffer to store the previous URL
char* fetch_art_url_image() {
    FILE* fp = popen("playerctl metadata --format '{{mpris:artUrl}}'", "r");
    if (!fp) return NULL;

    char url[512];
    if (!fgets(url, sizeof(url), fp)) {
        pclose(fp);
        return NULL;
    }
    pclose(fp);

    // Remove trailing newline from the URL
    url[strcspn(url, "\n")] = '\0';

    // Define the local file path to save the image
    const char* local_path = "/tmp/album_art.jpg";

    if (strcmp(url, prev_url) == 0) {
        // If the URL is the same as the previous one, return the existing file path
        return strdup(local_path);
    }
    // Update the previous URL
    strncpy(prev_url, url, sizeof(prev_url) - 1);
    prev_url[sizeof(prev_url) - 1] = '\0'; // Ensure null termination
    // Otherwise, use libcurl to download the image
    CURL* curl = curl_easy_init();
    if (!curl) {
        // Set previous URL to empty if curl initialization fails
        prev_url[0] = '\0';
        return NULL;
    }

    FILE* file = fopen(local_path, "wb");
    if (!file) {
        curl_easy_cleanup(curl);
        prev_url[0] = '\0';
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    CURLcode res = curl_easy_perform(curl);
    fclose(file);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        remove(local_path); // Remove the file if download failed
        prev_url[0] = '\0';
        return NULL;
    }

    // Return the local file path
    char* result = strdup(local_path);
    return result;
}

char* title() {
    FILE* fp = popen("playerctl metadata --format '{{title}}'", "r");
    if (!fp) return NULL;

    char* result = malloc(256);
    if (fgets(result, 256, fp) == NULL) {
        free(result);
        result = NULL;
    }
    pclose(fp);
    return result;
}

char* artist() {
    FILE* fp = popen("playerctl metadata --format '{{artist}}'", "r");
    if (!fp) return NULL;

    char* result = malloc(256);
    if (fgets(result, 256, fp) == NULL) {
        free(result);
        result = NULL;
    }
    pclose(fp);
    return result;
}

char* length() {
    FILE* fp = popen("playerctl metadata --format '{{album}}'", "r");
    if (!fp) return NULL;

    char* result = malloc(256);
    if (fgets(result, 256, fp) == NULL) {
        free(result);
        result = NULL;
    }
    pclose(fp);
    return result;
}

GCallback status() {
    system("playerctl status");
}